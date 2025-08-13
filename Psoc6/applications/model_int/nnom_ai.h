#ifndef NNOM_LSTM_DEMO_H
#define NNOM_LSTM_DEMO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // 预测值外部变量声明
    extern float temp_pre;   // 温度预测值
    extern float hum_pre;    // 湿度预测值
    extern float env_pre;    // 环境模型预测值
    extern float stress_pre; // 压力预测值

    // 原始数据采集与LSTM序列更新接口
    void update_raw_buffer(float temp, float hum);
    void update_lstm_sequence(void);

    // LSTM主线程启动接口
    typedef struct
    {
        float temp_seq[6];
        float hum_seq[6];
    } lstm_input_seq_t;

    int nnom_lstm_demo_thread(void);

#ifdef __cplusplus
}
#endif

#endif // NNOM_LSTM_DEMO_H
