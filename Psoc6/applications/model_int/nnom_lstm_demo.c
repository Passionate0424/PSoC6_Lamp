#include <rtthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "fcnn_env.h"
#include "lstm_temperature.h"
#include "lstm_humidity.h"
#include "nnom.h"
#include "fcnn_stress.h"
#include <rtdbg.h>
#include "onnx.h"
#include "gui_guider.h"

#define ONNX_MODEL_NAME "env_score_fcnn_nnom.onnx"

//extern rt_sem_t sem_sgp; // SGP30信号量

// pre 显示buffer
static char disp_nnom_buffer[15]; // 预测值显示缓冲

// 外部获取预测量
float temp_pre;   // 温度预测值
float hum_pre;    // 湿度预测值
float env_pre;    // 环境模型预测值
float stress_pre; // 压力预测值

extern float current_light_intensity; // 当前光照强度
extern float Air_Data[3];             // 存储温度、气压、海拔
extern int tvoc;                      // 总挥发性有机物浓度
extern int eco2;                      // 二氧化碳浓度
extern float T_H_Data[2];             // 温湿度数据数组，T_H_Data[0]为温度，T_H_Data[1]为湿度
extern uint8_t respiration_value;     // 呼吸频率
extern uint8_t heart_rate_value;      // 心率值

// temperature（温度）
// co2（二氧化碳浓度）
// tvoc（总挥发性有机物）
// pressure（气压）
// humidity（湿度）
// env模型归一化参数
// 注意此模型使用MinMaxScaler归一化
// 输入计算公式为：x_norm = (x - scaler_min) * scaler_scale
// 环境评分归一化参数 (MinMaxScaler)
const float env_scaler_min[5] = {22.33, 424.946, 0.0, 970.082, 26.759};
const float env_scaler_scale[5] = {0.15420198, 0.0020677561, 0.0008335487, 0.028491633, 0.024939522};
// const float env_scaler_scale[5] = {0.15420198, 0.0020677561, 0.00021889502 * 3, 0.028491633, 0.024939522};

static const float temp_scaler_min = 5.34999990f;
static const float temp_scaler_scale = 0.03210273f;

static const float hum_scaler_min = 20.00000000;
static const float hum_scaler_scale = 0.02994012f;

// HR（心率）
// respr（呼吸率）
// HR-respr（心率减去呼吸率）
// HR/respr（心率与呼吸率的比值，respr为0时为0）
// 压力模型归一化参数 (MinMaxScaler)
const float stress_scaler_min[4] = {49.00000000, 5.20472813, 3.06189974};
const float stress_scaler_scale[4] = {0.01022704, 0.07716761, 0.05755355};

static int8_t temp_nnom_input_transform(float x)
{
    float x_norm = (x - temp_scaler_min) * temp_scaler_scale * 128.0f;
    if (x_norm > 127)
        x_norm = 127;
    if (x_norm < -128)
        x_norm = -128;
    return (int8_t)x_norm;
}

static int8_t hum_nnom_input_transform(float x)
{
    float x_norm = (x - hum_scaler_min) * hum_scaler_scale * 128.0f;
    if (x_norm > 127)
        x_norm = 127;
    if (x_norm < -128)
        x_norm = -128;
    return (int8_t)x_norm;
}

// env模型输入归一化（5输入）
static int8_t env_nnom_input_transform(float x, int idx)
{
    float x_norm = (x - env_scaler_min[idx]) * env_scaler_scale[idx] * 128.0f;
    if (x_norm > 127)
        x_norm = 127;
    if (x_norm < -128)
        x_norm = -128;
    rt_kprintf("env_nnom_input_transform:x = %f, x_norm = %f\n", x, x_norm);
    return (int8_t)x_norm;
}

// env模型分数输出计算函数
static float env_nnom_out(int8_t *inputs)
{

    float output = inputs[3] * 0.78f + inputs[2] * 0.45f + inputs[1] * 0.23f; // 计算环境评分

    if (output > 100.0f)
        output = 100.0f; // 限制最大值为100

    float norm = output / 100.0f; // output为线性分数，0~100
    float mapped = 0.5f * powf(2.0f * norm, 1.5f) * (norm < 0.5f) +
                   (1.0f - 0.5f * powf(2.0f * (1.0f - norm), 1.5f)) * (norm >= 0.5f);
    mapped = mapped * 100.0f; // 映射到0~100范围

    return mapped; // 返回映射后的环境评分
}

// 压力模型输入归一化（4输入）
static int8_t stress_nnom_input_transform(float x, int idx)
{
    float x_norm = (x - stress_scaler_min[idx]) * stress_scaler_scale[idx] * 128.0f;
    if (x_norm > 127)
        x_norm = 127;
    if (x_norm < -128)
        x_norm = -128;
    rt_kprintf("stress_nnom_input_transform:x = %f, x_norm = %f\n", x, x_norm);
    return (int8_t)x_norm;
}

// ----------- 新增：温湿度时间序列采集与降噪逻辑 -----------
// 说明：
// 1. 每分钟采集一次原始温湿度，存入循环缓冲区（90分钟）
// 2. 每15分钟计算一次均值，更新LSTM输入序列（共6个点，覆盖90分钟）
// 3. 上电5分钟后即可用已有数据均值填充LSTM输入，保证可以预测
// 4. 上电90分钟后，LSTM输入序列全部为真实15分钟均值
#define RAW_BUF_SIZE 90 // 90分钟，每分钟采样一次
#define LSTM_SEQ_LEN 6  // LSTM输入序列长度（6个15分钟点）
#define SAMPLE_INTERVAL_MIN 1
#define LSTM_INTERVAL_MIN 15

static float temp_raw_buf[RAW_BUF_SIZE] = {0}; // 原始温度采样缓冲区
static float hum_raw_buf[RAW_BUF_SIZE] = {0};  // 原始湿度采样缓冲区
static int raw_buf_index = 0;                  // 缓冲区写入索引
// 更安全的分钟计数方式，防止溢出
static int total_minutes = 0;  // 仅用于统计总运行时间
static int minutes_in_buf = 0; // 用于索引，最大为RAW_BUF_SIZE

static float temp_lstm_seq[LSTM_SEQ_LEN] = {0}; // LSTM输入序列（温度）
static float hum_lstm_seq[LSTM_SEQ_LEN] = {0};  // LSTM输入序列（湿度）

// 每分钟调用一次，采集原始温湿度，写入循环缓冲区
// temp/hum: 当前采集到的温度和湿度
void update_raw_buffer(float temp, float hum)
{
    temp_raw_buf[raw_buf_index] = temp;                 // 存入温度
    hum_raw_buf[raw_buf_index] = hum;                   // 存入湿度
    raw_buf_index = (raw_buf_index + 1) % RAW_BUF_SIZE; // 循环写入
}

// 每15分钟调用一次，更新LSTM输入序列（降噪：仅取区间内连续3个点均值）
// 只取每15分钟区间内的连续三个点（如有），求均值填充到LSTM输入序列
void update_lstm_sequence()
{
    int available_points = minutes_in_buf / LSTM_INTERVAL_MIN; // 实际可用的15分钟区间数
    for (int i = 0; i < LSTM_SEQ_LEN; ++i)
    {
        int start_min = (available_points - LSTM_SEQ_LEN + i) * LSTM_INTERVAL_MIN;
        if (start_min < 0)
            start_min = 0;
        int end_min = start_min + LSTM_INTERVAL_MIN;
        if (end_min > minutes_in_buf)
            end_min = minutes_in_buf;
        int count = 0;
        float temp_sum = 0, hum_sum = 0;
        // 取区间内的连续三个点（优先取区间前3个点）
        for (int m = start_min; m < start_min + 3 && m < end_min; ++m)
        {
            int idx = (raw_buf_index - (minutes_in_buf - m) + RAW_BUF_SIZE) % RAW_BUF_SIZE;
            temp_sum += temp_raw_buf[idx];
            hum_sum += hum_raw_buf[idx];
            count++;
        }
        if (count > 0)
        {
            temp_lstm_seq[i] = temp_sum / count;
            hum_lstm_seq[i] = hum_sum / count;
        }
        else
        {
            // 上电初期数据不足时，用已有数据均值填充
            float temp_sum = 0, hum_sum = 0;
            for (int k = 0; k < minutes_in_buf; ++k)
            {
                int idx = (raw_buf_index + RAW_BUF_SIZE - (minutes_in_buf - k)) % RAW_BUF_SIZE;
                temp_sum += temp_raw_buf[idx];
                hum_sum += hum_raw_buf[idx];
            }
            temp_lstm_seq[i] = temp_sum / (minutes_in_buf > 0 ? minutes_in_buf : 1);
            hum_lstm_seq[i] = hum_sum / (minutes_in_buf > 0 ? minutes_in_buf : 1);
        }
    }
}

// ----------- 修改主入口，支持上电5分钟后预测，90分钟后用真实数据 -----------
// 每分钟采集温湿度，15分钟更新一次LSTM输入，5分钟后即可预测
static void nnom_lstm_demo_entry(void *parameter)
{
    // 初始化NNOM模型
    nnom_model_t *model_env = env_nnom_model_create();
    nnom_model_t *model_temp = temperature_nnom_model_create();
    nnom_model_t *model_hum = humidity_nnom_model_create();
    nnom_model_t *model_stress = nnom_model_create();

    //    // 0. Load Model
    //    Onnx__ModelProto *model = onnx_load_model(ONNX_MODEL_NAME);
    //    if (model == NULL)
    //    {
    //        printf("Failed to load model %s\n", ONNX_MODEL_NAME);
    //        return;
    //    }

    int min = 0;                     // 计数分钟数
    //rt_sem_take(sem_sgp, 60 * 1000); // 等待SGP30初始化完成
    rt_thread_mdelay(500);
    while (1)
    {
        // TODO: 替换为实际采集到的温湿度数据
        float temp = T_H_Data[0];
        rt_kprintf("temperature: %f\n", T_H_Data[0]);
        float hum = T_H_Data[1];
        update_raw_buffer(temp, hum); // 每分钟采集一次
        total_minutes++;
        if (minutes_in_buf < RAW_BUF_SIZE)
            minutes_in_buf++; // 累计分钟数，最多90分钟
        // 上电5分钟后即可预测
        if (minutes_in_buf >= 1)
        {
            // 若未满90分钟，LSTM序列用已有数据均值填充（降噪，保证能预测）
            // minutes_in_buf没有环形更新，一旦达到90则保持在90
            if (minutes_in_buf < 90)
            {
                float temp_sum = 0, hum_sum = 0;
                int count = minutes_in_buf;
                for (int i = 0; i < count; ++i)
                {
                    int idx = (raw_buf_index + RAW_BUF_SIZE - (minutes_in_buf - i)) % RAW_BUF_SIZE;
                    temp_sum += temp_raw_buf[idx];
                    hum_sum += hum_raw_buf[idx];
                }
                float temp_mean = temp_sum / count;
                float hum_mean = hum_sum / count;
                for (int i = 0; i < LSTM_SEQ_LEN; ++i)
                {
                    temp_lstm_seq[i] = temp_mean;
                    hum_lstm_seq[i] = hum_mean;
                }
            }
            else
            {
                // 已满90分钟，直接使用循环缓冲区数据更新LSTM输入序列
                update_lstm_sequence();
            }
            // 填充LSTM输入张量，进行模型推理
            for (int i = 0; i < LSTM_SEQ_LEN; ++i)
            {
                temperature_nnom_input_data[i] = temp_nnom_input_transform(temp_lstm_seq[i]);
                humidity_nnom_input_data[i] = hum_nnom_input_transform(hum_lstm_seq[i]);
            }
            model_run(model_temp);
            model_run(model_hum);

            // env模型输入准备（5输入：温度、co2、tvoc、气压、湿度）
            rt_kprintf("pressure:%f\n", Air_Data[1]);

            if (temp < 22.33)
                temp = 22.33; // 确保温度不低于22.33

            if (eco2 < 425)
                eco2 = 425; // 确保eco2不低于425

            float env_inputs[5] = {temp, eco2, tvoc, Air_Data[1] - 20, hum};
            for (int i = 0; i < 5; ++i)
            {
                env_nnom_input_data[i] = env_nnom_input_transform(env_inputs[i], i);
            }
            model_run(model_env);

            if (heart_rate_value != 0 && respiration_value != 0)
            {
                // stress模型输入准备（4输入：心率、呼吸率、心率减去呼吸率、心率与呼吸率的比值）
                rt_kprintf("heart_rate_value: %d, respiration_value: %d\n", heart_rate_value, respiration_value);
                float stress_inputs[3] = {
                    heart_rate_value,
                    respiration_value,
                    // (respiration_value != 0) ? (float)heart_rate_value / respiration_value : 0.0f
                };
                for (int i = 0; i < 2; ++i)
                {
                    nnom_input_data[i] = stress_nnom_input_transform(stress_inputs[i], i);
                }
                model_run(model_stress);
                stress_pre = ((127 - nnom_output_data[0]) / 128.0f) * 10.0f; // 反量化
            }
            else
            {
                stress_pre = -1; // 无法计算压力预测值
            }

            // 反量化输出，得到实际温湿度预测值
            temp_pre = (temperature_nnom_output_data[0] / 128.0f) / temp_scaler_scale + temp_scaler_min;
            hum_pre = (humidity_nnom_output_data[0] / 128.0f) / hum_scaler_scale + hum_scaler_min;
            // env_pre = (nnom_output_data[0] / 128.0f) * 100.0;
            env_pre = env_nnom_out(env_nnom_output_data); // 计算环境评分

            rt_kprintf("[min=%d] TEMP output: %d,  %f\n", min + 1, temperature_nnom_output_data[0], temp_pre);
            rt_kprintf("[min=%d] HUM output: %d,  %f\n", min + 1, humidity_nnom_output_data[0], hum_pre);
            // rt_kprintf("[min=%d] ENV output: %d,  %f\n", min + 1, nnom_output_data[0], env_pre);
            rt_kprintf("[min=%d] ENV output: %d,%d,%d,%d   %f\n", min + 1, env_nnom_output_data[0], env_nnom_output_data[1], env_nnom_output_data[2], env_nnom_output_data[3], env_pre);
            rt_kprintf("[min=%d] STRESS output: %d,%d,  %f\n", min + 1, nnom_output_data[0], nnom_output_data[1], stress_pre);

            // lvgl 显示数据
            rt_snprintf(disp_nnom_buffer, sizeof(disp_nnom_buffer), "%.2f°C", temp_pre);
            if (guider_ui.monitor_label_temperature_pre != RT_NULL)
            {
                rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
                lv_label_set_text(guider_ui.monitor_label_temperature_pre, disp_nnom_buffer);
                rt_mutex_release(lv_mutex); // 释放互斥锁
            }
            rt_snprintf(disp_nnom_buffer, sizeof(disp_nnom_buffer), "%.2f%%", hum_pre);
            if (guider_ui.monitor_label_humidity_pre != RT_NULL)
            {
                rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
                lv_label_set_text(guider_ui.monitor_label_humidity_pre, disp_nnom_buffer);
                rt_mutex_release(lv_mutex); // 释放互斥锁
            }
            rt_snprintf(disp_nnom_buffer, sizeof(disp_nnom_buffer), "%.2f", env_pre);
            if (guider_ui.monitor_label_env_score != RT_NULL)
            {
                rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
                lv_label_set_text(guider_ui.monitor_label_env_score, disp_nnom_buffer);
                rt_mutex_release(lv_mutex); // 释放互斥锁
            }
            if (stress_pre >= 0 && stress_pre < 3)
            {
                if (guider_ui.monitor_label_stress_score != RT_NULL)
                {
                    rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
                    lv_label_set_text(guider_ui.monitor_label_stress_score, "Low");
                    rt_mutex_release(lv_mutex); // 释放互斥锁
                }
            }
            else if (stress_pre >= 3)
            {
                if (guider_ui.monitor_label_stress_score != RT_NULL)
                {
                    rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
                    lv_label_set_text(guider_ui.monitor_label_stress_score, "High");
                    rt_mutex_release(lv_mutex); // 释放互斥锁
                }
            }
            else // stress_pre < 0，表示无法计算压力预测值
            {
                if (guider_ui.monitor_label_stress_score != RT_NULL)
                {
                    rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
                    lv_label_set_text(guider_ui.monitor_label_stress_score, "---");
                    rt_mutex_release(lv_mutex); // 释放互斥锁
                }
            }
        }

        min++;
        rt_thread_mdelay(10 * 1000); // 每分钟循环一次
    }
    // 不会执行到这里
    // model_delete(model_temp);
    // model_delete(model_hum);
}

int nnom_lstm_demo_thread(void)
{
    rt_thread_t tid = rt_thread_create("nnom_lstm", nnom_lstm_demo_entry, NULL, 20480, 20, 10);
    if (tid)
        rt_thread_startup(tid);
    else
    {
        LOG_E("Failed to create NNOM LSTM demo thread\n");
        return -1;
    }
    return 0;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
INIT_APP_EXPORT(nnom_lstm_demo_thread);
// MSH命令：env_test temp co2 tvoc pressure hum
// 例：env_test 25 600 100 1000 50
void env_test(int argc, char **argv)
{
    if (argc != 6)
    {
        rt_kprintf("用法: env_test temp co2 tvoc pressure hum\n");
        return;
    }
    float env_inputs[5];
    for (int i = 0; i < 5; i++)
        env_inputs[i] = atof(argv[i + 1]);

    // 归一化并填充输入
    for (int i = 0; i < 5; i++)
        env_nnom_input_data[i] = env_nnom_input_transform(env_inputs[i], i);

    nnom_model_t *model_env = env_nnom_model_create();
    model_run(model_env);

    // rt_kprintf("ENV模型输出: %d %d %d %d\n", nnom_output_data[0], nnom_output_data[1], nnom_output_data[2], nnom_output_data[3]);
    // float y_env_real = (nnom_output_data[0] / 128.0f) * 100.0;
    // rt_kprintf("ENV模型输出: %d, 反量化: %f\n", nnom_output_data[0], y_env_real);
    float y_env_real = env_nnom_out(env_nnom_output_data); // 计算环境评分
    rt_kprintf("ENV模型输出: %d,%d,%d,%d, 反量化: %f\n", env_nnom_output_data[0], env_nnom_output_data[1], env_nnom_output_data[2], env_nnom_output_data[3], y_env_real);
}
MSH_CMD_EXPORT(env_test, 手动输入5参数测试ENV模型);

// stress 模型测试
void stress_test(int argc, char **argv)
{
    if (argc != 3)
    {
        rt_kprintf("用法: stress_test heart_rate respiration\n");
        return;
    }
    float stress_inputs[2];
    for (int i = 0; i < 2; i++)
        stress_inputs[i] = atof(argv[i + 1]);
    // stress_inputs[2] = (stress_inputs[1] != 0) ? (stress_inputs[0] / stress_inputs[1]) : 0.0f; // 心率与呼吸率的比值

    // 归一化并填充输入
    for (int i = 0; i < 2; i++)
        nnom_input_data[i] = stress_nnom_input_transform(stress_inputs[i], i);

    nnom_model_t *model_stress = nnom_model_create();
    model_run(model_stress);

    rt_kprintf("STRESS模型输出: %d %d\n", nnom_output_data[0], nnom_output_data[1]);
    //    float y_stress_real = (stress_nnom_output_data[1] / 128.0f) * 10.0f;
    //    rt_kprintf("STRESS模型输出: %d, 反量化: %f\n", stress_nnom_output_data[0], y_stress_real);
}
MSH_CMD_EXPORT(stress_test, 手动输入3参数测试STRESS模型);

// load bin文件
int8_t *load(const char *file, size_t *size)
{
    size_t sz;
    FILE *fp = fopen(file, "rb");
    int8_t *input;
    assert(fp);
    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    input = malloc(sz);
    fread(input, 1, sz, fp);
    fclose(fp);
    *size = sz;
    return input;
}

// 使用nnom predict api 读取env_score_fcnn_nnom_test.bin测试env模型准确度
void env_accuracy_test(int argc, char **argv)
{

    int8_t *input;
    float prob;
    uint32_t label;
    size_t size = 0;
    FILE *fp;

    input = load("env_score_fcnn_nnom_test.bin", &size); // load a continuous input dataset (test bin)
    fp = fopen("result_env.csv", "w");                   // csv file for result
    if (!fp)
    {
        LOG_E("Failed to open result_env.csv for writing\n");
        free(input);
        return;
    }
    fprintf(fp, "label, prob\n"); // header of csv
    printf("validation size: %d\n", (uint32_t)size);

    nnom_model_t *env_model = env_nnom_model_create();
    nnom_predict_t *pre = prediction_create(env_model, env_nnom_output_data, sizeof(env_nnom_output_data), 1);

    // now takes label and data from the file and data
    for (size_t seek = 0; seek < size;)
    {
        // labels
        uint8_t true_label[128];
        memcpy(true_label, input + seek, 128);
        seek += 128;
        // data
        for (int i = 0; i < 128; i++)
        {
            if (seek >= size)
                break;
            memcpy(env_nnom_input_data, input + seek, sizeof(env_nnom_input_data));
            seek += sizeof(env_nnom_input_data); // Update to use sizeof(env_nnom_input_data)

            // nnom_predict(model, &label, &prob);				// this will work independently
            prediction_run(pre, true_label[i], &label, &prob); // this provide more infor but requires prediction API

            // save results
            fprintf(fp, "%d,%f\n", label, prob);
        }
        printf("Processing %d%%\n", (uint32_t)(seek * 100 / size));
    }

    // save result
    fclose(fp);

    // print prediction result
    prediction_end(pre);
    prediction_summary(pre);
    prediction_delete(pre);

    // model
    model_stat(env_model);
    model_delete(env_model);

    free(input);
}
MSH_CMD_EXPORT(env_accuracy_test, test);

// 使用nnom predict api 读取stress_score_fcnn_nnom_test.bin测试stress模型准确度
void stress_accuracy_test(int argc, char **argv)
{

    int8_t *input;
    float prob;
    uint32_t label;
    size_t size = 0;
    FILE *fp;

    input = load("stress_score_fcnn_nnom_test.bin", &size); // load a continuous input dataset (test bin)
    fp = fopen("result_stress.csv", "w");                   // csv file for result
    if (!fp)
    {
        LOG_E("Failed to open result_stress.csv for writing\n");
        free(input);
        return;
    }
    fprintf(fp, "label, prob\n"); // header of csv
    printf("validation size: %d\n", (uint32_t)size);

    nnom_model_t *stress_model = nnom_model_create();
    nnom_predict_t *pre = prediction_create(stress_model, nnom_output_data, sizeof(nnom_output_data), 1);

    // now takes label and data from the file and data
    for (size_t seek = 0; seek < size;)
    {
        // labels
        uint8_t true_label[128];
        memcpy(true_label, input + seek, 128);
        seek += 128;
        // data
        for (int i = 0; i < 128; i++)
        {
            if (seek >= size)
                break;
            memcpy(nnom_input_data, input + seek, sizeof(nnom_input_data));
            seek += sizeof(nnom_input_data);

            // nnom_predict(model, &label, &prob);				// this will work independently
            prediction_run(pre, true_label[i], &label, &prob); // this provide more infor but requires prediction API

            // save results
            fprintf(fp, "%d,%f\n", label, prob);
        }
        printf("Processing %d%%\n", (uint32_t)(seek * 100 / size));
    }

    // save result
    fclose(fp);

    // print prediction result
    prediction_end(pre);
    prediction_summary(pre);
    prediction_delete(pre);

    // model
    model_stat(stress_model);
    model_delete(stress_model);

    free(input);
}
MSH_CMD_EXPORT(stress_accuracy_test, test);

// 使用nnom predict api 读取lstm_temprature_model_nnom_test.bin测试温度LSTM模型准确度
void temp_lstm_accuracy_test(int argc, char **argv)
{
    int8_t *input;
    float prob;
    uint32_t label;
    size_t size = 0;
    FILE *fp;

    input = load("lstm_temprature_model_nnom_test.bin", &size); // 加载测试数据
    fp = fopen("result_temp_lstm.csv", "w");                   // 结果输出到CSV
    if (!fp)
    {
        LOG_E("Failed to open result_temp_lstm.csv for writing\n");
        free(input);
        return;
    }
    fprintf(fp, "label, prob\n"); // CSV表头
    printf("validation size: %d\n", (uint32_t)size);

    nnom_model_t *temp_model = temperature_nnom_model_create();
    nnom_predict_t *pre = prediction_create(temp_model, temperature_nnom_output_data, sizeof(temperature_nnom_output_data), 1);

    // 按128个label一组，后跟128组输入数据
    for (size_t seek = 0; seek < size;)
    {
        // labels
        uint8_t true_label[128];
        memcpy(true_label, input + seek, 128);
        seek += 128;
        // data
        for (int i = 0; i < 128; i++)
        {
            if (seek >= size)
                break;
            memcpy(temperature_nnom_input_data, input + seek, sizeof(temperature_nnom_input_data));
            seek += sizeof(temperature_nnom_input_data);

            prediction_run(pre, true_label[i], &label, &prob);

            // 保存结果
            fprintf(fp, "%d,%f\n", label, prob);
        }
        printf("Processing %d%%\n", (uint32_t)(seek * 100 / size));
    }

    fclose(fp);

    prediction_end(pre);
    prediction_summary(pre);
    prediction_delete(pre);

    model_stat(temp_model);
    model_delete(temp_model);

    free(input);
}
MSH_CMD_EXPORT(temp_lstm_accuracy_test, test);

#endif
