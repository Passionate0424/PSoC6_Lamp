#include "rtthread.h"
#include <rtdbg.h>
#include "llm.h"
#include "rtdevice.h"
#include "llm_contory.h"
// ================== 传感器数据接口声明 ==================
// 这些变量在其他模块定义，这里用extern声明以便直接读取实时数据
extern uint8_t human_presence_status; // 人体存在状态（0:无人，1:有人）
extern uint8_t respiration_value;     // 呼吸频率（次/分钟）
extern uint8_t heart_rate_value;      // 心率值（bpm）
extern float current_light_intensity; // 当前光照强度（lux）
extern float Air_Data[3];             // [0]:温度(°C) [1]:气压(hPa) [2]:海拔(m)
extern float T_H_Data[2];             // [0]:温度(°C) [1]:湿度(%)
extern int tvoc;                      // 总挥发性有机物浓度（ppb）
extern int eco2;                      // 二氧化碳浓度（ppm）
rt_mutex_t llm_mutex = RT_NULL;

static llm_t llm_handle = RT_NULL;
const char LED_PROMPT[] =
    "prompt:\n"
    "协议:MCU指令中枢,解析指令→生成信号;指令表:开灯=0x00,关灯=0x01,查询室内温度=0x02,查询室内湿度=0x03,查询室内二氧化碳浓度=0x04,查询室内气压=0x05,查询室内温度预测值=0x06,查询室内湿度预测值=0x07,查询室内环境评分=0x08;\n"
    "处理:检测开/关灯相关语义→返回CMD,否则正常对话;\n"
    "约束:指令与对话分离,不解释指令;如果未检测到指令表中的语义,输出内容中严禁出现0x00、0x01等命令字;任何对话回答均限制在100字节内。\n"
    "根据上述要求运行，接下来是我的输入字符:{%s}";
const char ENV_PROMPT[] =
    "prompt:\n"
    "现在你是一位智能家居助手，你需要根据我提供的传感器数据给出室内环境的评价和调整建议，注意你要拟人化风格回答，单位用用中文表示，比如%%输出为百分之，便于我进行语音合成。\n"
    "约束：回答简短，不超过150字，保证你输出的内容就是语音输出的内容，不要有括号等备注。仅针对传感器数据做出评价，禁止继续询问用户任何问题，**禁止有任何引导用户回复你的内容**。\n"
    "**比如禁止回复“需要我帮您调低空调温度或打开加湿器吗”等引导性问题**\n"
    "下面是我的传感器数据：\n"
    "温度：%.2f 摄氏度\n"
    "湿度：%.2f 百分比\n"
    "二氧化碳浓度：%d ppm\n"
    "tvoc浓度：%d ppb\n"
    "气压：%.2f hPa\n";
const char CHAT_PROMPT[] =
    "prompt:\n"
    "现在你是一位语音助手，我们要进行语音对话，请你的回复内容不要出现\\n等转义字符，便于我合成语音。\n"
    "下面是我的输入:{%s}\n";
static int led_state = PIN_LOW;

/* 解析结果 */
void deal_llm_answer(llm_t handle)
{
    rt_kprintf("====================================LLM handle: %p\n", handle);
    char *answer = RT_NULL;
    rt_mb_recv(handle->outputbuff_mb, (rt_uint32_t *)&answer, RT_WAITING_FOREVER);
    /* you can modify */

    int len = rt_strlen(answer);
    rt_kprintf("LLM :\n");
    for (int i = 0; i < len; i++)
    {
        rt_kprintf("%c", answer[i]);
    }
    rt_kprintf("\n");
    rt_kprintf("%s\n", answer);
    uart_send_speak(answer);

    if (rt_strstr(answer, "0x00"))
    {
        led_state = PIN_HIGH;
        rt_kprintf("灯已打开:%x", led_state);
    }
    else if (rt_strstr(answer, "0x01"))
    {
        led_state = PIN_LOW;
        rt_kprintf("灯已关闭:%x", led_state);
    }
    else if (rt_strstr(answer, "0x02"))
    {
        // 室内温度
        // rt_kprintf("室内温度:25°C");
    }
    else if (rt_strstr(answer, "0x03"))
    {
        // 室内湿度
        // rt_kprintf("室内湿度:60%%");
    }
    else if (rt_strstr(answer, "0x04"))
    {
        // 室内二氧化碳浓度
        // rt_kprintf("室内二氧化碳浓度:400ppm");
    }
    else if (rt_strstr(answer, "0x05"))
    {
        // 室内气压
        // rt_kprintf("室内气压:1013hPa");
    }
    else if (rt_strstr(answer, "0x06"))
    {
        // 室内温度预测值
        // rt_kprintf("室内温度预测值:26°C");
    }
    else if (rt_strstr(answer, "0x07"))
    {
        // 室内湿度预测值
        // rt_kprintf("室内湿度预测值:62%%");
    }
    else if (rt_strstr(answer, "0x08"))
    {
        // 室内环境评分
        // rt_kprintf("室内环境评分:85分");
    }

    rt_kprintf("\n");

    /* end */
    rt_free(answer);
}

/* 发送环境信息 */
void llm_send_env(void)
{
    char prompt[PKG_LLM_CMD_BUFFER_SIZE];

    if (llm_handle == RT_NULL)
    {
        LOG_E("llm_handle is null\n");
        return;
    }

    rt_snprintf(prompt, sizeof(prompt), ENV_PROMPT, T_H_Data[0], T_H_Data[1], eco2, tvoc, Air_Data[1]);
    rt_mutex_take(llm_mutex, 100);
    send_llm_mb(llm_handle, prompt);
}

// 测试llm_send_env
static void test_llm_send_env()
{
    llm_send_env();
}
MSH_CMD_EXPORT(test_llm_send_env, llm_send_env);

/*创建llm*/
static void entry_llm()
{
    llm_mutex = rt_mutex_create("llm_mutex", RT_IPC_FLAG_FIFO);
    if (llm_mutex == RT_NULL)
    {
        LOG_E("llm_mutex create failed\n");
        return;
    }
    llm_handle = create_llm_t();
}
INIT_APP_EXPORT(entry_llm);
MSH_CMD_EXPORT(entry_llm, llm_entry);

/*
 *外部发送接口
 */
void llm_send_chat(char *buffer)
{
    static char content[2048];
    if (llm_handle == RT_NULL)
    {
        LOG_E("llm_handle is null\n");
        return;
    }
    if (buffer == RT_NULL)
    {
        LOG_E("buffer is null\n");
        return;
    }
    rt_snprintf(content, sizeof(content), CHAT_PROMPT, buffer);
    rt_mutex_take(llm_mutex, 100);
    send_llm_mb(llm_handle, content);
}

/* 发送信息 */
static void send(int argc, char *argv[])
{
    char prompt[PKG_LLM_CMD_BUFFER_SIZE];
    if (argc < 2)
    {
        LOG_I("Usage: llm_send <message>\n");
        return;
    }

    if (llm_handle == RT_NULL)
    {
        LOG_E("llm_handle is null\n");
        return;
    }

    rt_snprintf(prompt, sizeof(prompt), LED_PROMPT, argv[1]);
    send_llm_mb(llm_handle, prompt);
}

MSH_CMD_EXPORT(send, llm_send);

/* 删除llm */
static void delete_llm()
{
    delete_llm_t(llm_handle);
}

MSH_CMD_EXPORT(delete_llm, delete_llm);
