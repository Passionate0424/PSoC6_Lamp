/*
 * 串口发送协议
 * 传输协议：控制字 + 命令字
 * 例：c:01(注意使用英文冒号)
 * 控制字：c，命令字： 01 进入guard模式，11 退出guard模式
 *                   02 追踪模式关 03 追踪手 04 追踪书本
 *                   05 开始录音   15结束录音
 *                   06 开始推流   16结束推流
 *                   07 工作监测模式开始 17 工作监测模式结束
 *                   08 提醒休息语音 18 提醒工作语音
 *                   09 提醒喝水语音
 * 控制字:s:说话内容:p
 * 例如 s:...:p
 *
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <stdlib.h>
#include <string.h>
#include "smtp_mail.h"
#include "uart1.h"
#include "gui_guider.h"  // 引入LVGL GUI相关头文件
#include "llm_contory.h" // 引入工作监测相关头文件

#define SAMPLE_UART_NAME "uart1"

#define DBG_TAG "uart1"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* 用于接收消息的信号量 */
static rt_mq_t ai_dialog_mq;
static rt_sem_t rx_sem;
static rt_device_t serial;
#define UART1_RX_BUFFER_SIZE 30000           // 定义接收缓冲区大小
static char rx_buffer[UART1_RX_BUFFER_SIZE]; // 接收缓冲区

// 拼包相关定义和函数
#define PKT_BUF_SIZE 8192
static char pkt_buf[PKT_BUF_SIZE];
static int pkt_len = 0;
static int pkt_collecting = 0; // 0:未收集 1:t包 2:i包

typedef enum
{
    PKT_NONE = 0,
    PKT_T,
    PKT_I
} pkt_type_t;

static pkt_type_t check_pkt_head(const char *s)
{
    if (rt_strncmp(s, "t:", 2) == 0)
        return PKT_T;
    if (rt_strncmp(s, "i:", 2) == 0)
        return PKT_I;
    return PKT_NONE;
}

static int check_pkt_tail(pkt_type_t type, const char *s, int len)
{
    LOG_D("check_pkt_tail: type=%d, len=%d, last2='%c%c'", type, len, s[len - 2], s[len - 1]);
    if (type == PKT_T && strstr(s, ":p") != RT_NULL)
    {
        *(strstr(s, ":p") + 2) = '\0'; // 确保字符串结束
        return 1;
    }
    if (type == PKT_I && strstr(s, ":l") != RT_NULL)
    {
        *(strstr(s, ":l") + 2) = '\0';
        return 1;
    }
    return 0;
}

/**
 * @brief UART接收回调函数
 *
 * @param dev 设备句柄
 * @param size 接收数据大小
 * @return rt_err_t 错误码
 */
static rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size)
{
    if (size > 0)
    {
        //rt_kprintf("---------------------------int\n");
        // uart_rx_len += size;
        rt_sem_release(rx_sem);
        // 关闭串口中断
        rt_device_control(serial, RT_DEVICE_CTRL_CLR_INT, RT_NULL);
    }
    return RT_EOK;
}

// 高效HEX字符串转字节流
static inline unsigned char hex2byte(char h, char l)
{
    unsigned char v = 0;
    v = (h >= '0' && h <= '9') ? (h - '0') : ((h & ~0x20) - 'A' + 10);
    v <<= 4;
    v |= (l >= '0' && l <= '9') ? (l - '0') : ((l & ~0x20) - 'A' + 10);
    return v;
}

void hexstr_to_bytes_fast(const char *hexstr, uint8_t *out, size_t *out_len)
{
    size_t len = strlen(hexstr);
    *out_len = len / 2;
    for (size_t i = 0; i + 1 < len; i += 2)
    {
        out[i / 2] = hex2byte(hexstr[i], hexstr[i + 1]);
    }
    out[*out_len] = '\0';
}

/*数据处理线程入口
 *传输协议：
 *控制字:c，命令字:01检测到人存在 02 人正在喝水 03 人正在工作 04 人正在玩手机
 *控制字:d，三位整数表示工作持续时间
 *控制字:a，三位整数表示玩手机持续时间
 *例 c:01 c:02 d:135 a:120
 *控制字:x 三位整数表示坐标
 *控制字:y 三位整数表示坐标
 *例如 x:100 y:200
 *控制字:t:中文字段:p
 *例如 t:你好:p
 *控制字 i:jpg数据:l
 *例如 i:...:l
 */
static void uart1_thread_entry(void *parameter)
{

    while (1)
    {
        // 等待接收信号量
        rt_sem_take(rx_sem, RT_WAITING_FOREVER);
        rt_memset(rx_buffer, 0, UART1_RX_BUFFER_SIZE); // 清空接收缓冲区
        // 读取数据
        rt_int32_t len = rt_device_read(serial, -1, rx_buffer, UART1_RX_BUFFER_SIZE);
        if (len > 0)
        {
            // LOG_I("len: %d\n", len);
            rx_buffer[len] = '\0'; // 确保字符串结束
            LOG_I("Received: %s\n", rx_buffer);
            //  协议数据处理逻辑
            char *data = rx_buffer;
            int data_len = len;
            int idx = 0;
            do
            {
                // 若未在拼包，检查是否遇到t:或i:包头
                if (pkt_collecting == 0)
                {
                    // LOG_I("[检查包头] 当前数据: %s", &data[idx]);
                    pkt_type_t t = check_pkt_head(&data[idx]);
                    if (t != PKT_NONE)
                    {
                        pkt_collecting = t;
                        pkt_len = 0;
                    }
                }
                // 若在拼包，累积数据
                if (pkt_collecting)
                {
                    // 拷贝本帧剩余数据到pkt_buf
                    LOG_I("[拼包] 收到数据: %s", &data[idx]);
                    int copy_len = data_len - idx;
                    if (pkt_len + copy_len >= PKT_BUF_SIZE)
                        copy_len = PKT_BUF_SIZE - pkt_len - 1;
                    rt_memcpy(pkt_buf + pkt_len, data + idx, copy_len);
                    pkt_len += copy_len;
                    pkt_buf[pkt_len] = '\0';
                    // 检查包尾
                    if (check_pkt_tail(pkt_collecting, pkt_buf, pkt_len))
                    {
                        // 处理完整包
                        LOG_I("[拼包] 完整包: %s", pkt_buf);
                        // 复用原有协议解析逻辑
                        char *line = pkt_buf;
                        // ...原有协议解析逻辑...（可直接调用原有分支处理line）
                        // 这里只处理t/i包，其他协议仍走原逻辑
                        // 解析t包
                        char *ctrl = line;
                        char *arg1 = rt_strstr(line, ":");
                        if (arg1)
                        {
                            *arg1 = '\0';
                            arg1++;
                            char *arg2 = rt_strstr(arg1, ":");
                            if (arg2)
                            {
                                *arg2 = '\0';
                                arg2++;
                            }
                            if (rt_strcmp(ctrl, "t") == 0 && arg2)
                            {
                                // ...t包处理逻辑...
                                char *endp = rt_strstr(arg2, "p");
                                if (endp && (*(endp - 1) == ':'))
                                    *(endp - 1) = '\0';
                                LOG_I("[CMD] 控制字t, 中文字段: %s", arg1);
                                rt_mq_send(ai_dialog_mq, arg1, strlen(arg1) + 1);
                                // TODO: LVGL显示等
                            }
                            else if (rt_strcmp(ctrl, "i") == 0 && arg2)
                            {
                                // ...i包处理逻辑...
                                char *endp = rt_strstr(arg2, "l");
                                if (endp && (*(endp - 1) == ':'))
                                    *(endp - 1) = '\0';
                                LOG_I("[CMD] 控制字i, jpg数据: %s", arg1);
                                // TODO: 处理图片数据 arg1
                            }
                        }
                        pkt_collecting = 0;
                        pkt_len = 0;
                        idx += copy_len;
                        continue;
                    }
                    // 若未收完包，直接退出本帧处理，等待下帧继续拼包
                    break;
                }
                else
                {
                    // 非拼包协议，走原有协议处理
                    // ...原有协议处理逻辑...
                    // 以冒号分割
                    // LOG_I("Processing line: %s", &data[idx]);
                    char *line = &data[idx];
                    char *next_line = rt_strstr(line, "\r\n");
                    if (next_line)
                    {
                        *next_line = '\0';
                        next_line += 2;
                    }
                    if (rt_strlen(line) > 0)
                    {
                        char *ctrl = line;
                        char *arg1 = rt_strstr(line, ":");
                        if (arg1)
                        {
                            *arg1 = '\0';
                            arg1++;
                            char *arg2 = rt_strstr(arg1, ":");
                            if (arg2)
                            {
                                *arg2 = '\0';
                                arg2++;
                            }
                            if (rt_strcmp(ctrl, "c") == 0)
                            {
                                // 命令字处理
                                LOG_I("[CMD] 控制字c, 命令字: %s", arg1);
                                // TODO: 根据命令字arg1做不同处理
                                if (rt_strcmp(arg1, "01") == 0)
                                {
                                    LOG_I("guard模式,人入侵\n");
                                    // 发邮件
                                    if (rt_mutex_trytake(mail_mutex) == RT_EOK)
                                    {
                                        // send_mail(); // 发送邮件
                                        rt_sem_release(smtp_send_sem); // 释放发送信号量，触发邮件发送线程
                                    }
                                }
                                else if (strcmp(arg1, "02") == 0)
                                {
                                    LOG_I("喝水次数+1\n");
                                    // 喝水计数加1
                                    drink_count++;
                                    drink_timer_restart(); // 重启喝水定时器
                                    rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
                                    // 更新喝水计数显示
                                    lv_label_set_text_fmt(guider_ui.monitor_label_drink, "%d", drink_count);
                                    rt_mutex_release(lv_mutex);
                                }
                                else if (strcmp(arg1, "03") == 0)
                                {
                                    LOG_I("工作检测：正在工作\n");
                                }
                                else if (strcmp(arg1, "04") == 0)
                                {
                                    LOG_I("工作检测：正在玩手机\n");
                                }
                            }
                            else if (rt_strcmp(ctrl, "d") == 0)
                            {
                                //LOG_I("[CMD] 控制字d, 工作持续时间: %s", arg1);
                                // TODO: 处理工作持续时间
                                rt_int32_t work_duration = 0;
                                // 将arg1转换为整数
                                if (arg1)
                                {
                                    work_duration = (rt_int32_t)strtol(arg1, RT_NULL, 10);
                                    monitor_data.work_duration = work_duration; // 更新工作持续时间
                                    LOG_I("工作持续时间: %d 秒\n", work_duration);
                                    // 更新工作持续时间显示
                                    float work_duration_min = work_duration / 60.0f; // 转换为分钟
                                    rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
                                    lv_label_set_text_fmt(guider_ui.monitor_label_work_duration, "%.1f", work_duration_min);
                                    rt_mutex_release(lv_mutex);
                                }
                            }
                            else if (rt_strcmp(ctrl, "a") == 0)
                            {
                                //LOG_I("[CMD] 控制字a, 玩手机持续时间: %s", arg1);
                                // TODO: 处理玩手机持续时间
                                rt_int32_t phone_duration = 0;
                                // 将arg1转换为整数
                                if (arg1)
                                {
                                    phone_duration = (rt_int32_t)strtol(arg1, RT_NULL, 10);
                                    monitor_data.phone_duration = phone_duration; // 更新玩手机持续时间
                                    LOG_I("玩手机持续时间: %d 秒\n", phone_duration);
                                    // 转换为分钟并更新显示
                                    float phone_duration_min = phone_duration / 60.0f; // 转换
                                    // 更新玩手机持续时间显示
                                    rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
                                    lv_label_set_text_fmt(guider_ui.monitor_label_phone, "%.1f", phone_duration_min);
                                    rt_mutex_release(lv_mutex);
                                }
                            }
                            else if (rt_strcmp(ctrl, "t") == 0 && arg2)
                            {
                                // 处理t:xxx:p，去除结尾p
                                char *endp = rt_strstr(arg2, "p");
                                if (endp && (*(endp - 1) == ':'))
                                {
                                    *(endp - 1) = '\0';
                                }
                                LOG_I("[CMD] 控制字t, 中文字段(HEX): %s", arg1);
                                // 使用高效HEX转UTF-8
                                uint8_t utf8_buf[2560] = {0};
                                size_t utf8_len = 0;
                                hexstr_to_bytes_fast(arg1, utf8_buf, &utf8_len);
                                LOG_I("[CMD] 控制字t, 中文字段(UTF8): %s", utf8_buf);
                                // TODO: 你可以在这里将utf8_buf用于LVGL界面显示等
                            }
                            else if (rt_strcmp(ctrl, "i") == 0 && arg2)
                            {
                                // 处理i:xxx:p，去除结尾p
                                char *endp = rt_strstr(arg2, "l");
                                if (endp && (*(endp - 1) == ':'))
                                {
                                    *(endp - 1) = '\0';
                                }
                                LOG_I("[CMD] 控制字i, jpg数据: %s", arg1);
                                // TODO: 处理图片数据 arg1
                            }
                            else if (rt_strcmp(ctrl, "x") == 0 && arg1)
                            {
                                // 处理x:xxx
                                rt_int32_t x_coord = (rt_int32_t)strtol(arg1, RT_NULL, 10);
                                // LOG_I("[CMD] 控制字x, 坐标: %d", x_coord);
                                //  rt_mutex_take(gimbal_mutex, 20); // 获取云台互斥锁
                                my_target->x = x_coord; // 更新目标点坐标
                                // LOG_I("[CMD] 更新目标点坐标 x: %d", my_target->x);
                                //  rt_mutex_release(gimbal_mutex); // 释放云台互斥锁
                            }
                            else if (rt_strcmp(ctrl, "y") == 0 && arg1)
                            {
                                // 处理y:xxx
                                rt_int32_t y_coord = (rt_int32_t)strtol(arg1, RT_NULL, 10);
                                // LOG_I("[CMD] 控制字y, 坐标: %d", y_coord);
                                //  rt_mutex_take(gimbal_mutex, 20); // 获取云台互斥锁
                                my_target->y = y_coord; // 更新目标点坐标
                                // LOG_I("[CMD] 更新目标点坐标 y: %d", my_target->y);
                                //  rt_mutex_release(gimbal_mutex); // 释放云台互斥锁
                            }
                            else
                            {
                                LOG_W("[WARN] 未知协议: %s", line);
                            }
                        }
                        else
                        {
                            LOG_W("[WARN] 无冒号分割: %s", line);
                        }
                    }
                    if (next_line)
                        idx = next_line - data;
                    else
                        break;
                }
            } while (idx < data_len); // 继续处理剩余数据
            // ...existing code...
        }
        rt_device_control(serial, RT_DEVICE_CTRL_SET_INT, RT_NULL);
    }
}

/*
 * 发送协议数据，格式为 控制字:命令字\r\n
 * ctrl: 控制字字符串（如"c"）
 * cmd:  命令字int值（如1、11等）
 * 串口发送协议
 * 传输协议：控制字 + 命令字
 * 例：c:01(注意使用英文冒号)
 * 控制字：c，命令字： 01 进入guard模式，11 退出guard模式
 *                   02 追踪模式关 03 追踪手 04 追踪书本
 *                   05 开始录音   15结束录音
 *                   06 开始推流   16结束推流
 *                   07 工作监测模式开始 17 工作监测模式结束
 *                   08 提醒休息语音 18 提醒工作语音
 *                   09 提醒喝水语音
 * 控制字:s:说话内容:p
 * 例如 s:...:p
 * @return 发送成功返回RT_EOK，失败返回RT_ERROR
 */
rt_err_t uart_send_protocol(const char *ctrl, int cmd)
{
    // rt_kprintf("========================1\n");
    if (!serial || !ctrl)
    {
        LOG_E("uart_send_protocol: serial or ctrl is NULL");
        return RT_ERROR;
    }
    const char *cmd_str = NULL;
    switch (cmd)
    {
    case 1:
        cmd_str = "01";
        break;
    case 11:
        cmd_str = "11";
        break;
    case 2:
        cmd_str = "02";
        break;
    case 3:
        cmd_str = "03";
        break;
    case 4:
        cmd_str = "04";
        break;
    case 5:
        cmd_str = "05";
        break;
    case 15:
        cmd_str = "15";
        break;
    case 6:
        cmd_str = "06";
        break;
    case 16:
        cmd_str = "16";
        break;
    case 7:
        cmd_str = "07";
        break;
    case 17:
        cmd_str = "17";
        break;
    case 8:
        cmd_str = "08";
        break;
    case 18:
        cmd_str = "18";
        break;
    case 9:
        cmd_str = "09";
        break;
    default:
        LOG_E("uart_send_protocol: unknown cmd %d\n", cmd);
        return RT_ERROR;
    }
    char buf[20];
    int len = rt_snprintf(buf, sizeof(buf), "%s:%s\r\n", ctrl, cmd_str);
    if (len <= 0 || len >= (int)sizeof(buf))
    {
        LOG_E("uart_send_protocol: snprintf error, len=%d\n", len);
        return RT_ERROR;
    }
    rt_kprintf("========================uart_send_protocol: %s\n", buf);
    rt_size_t sent = rt_device_write(serial, 0, buf, len);
    return (sent == len) ? RT_EOK : RT_ERROR;
}

/*
 * 发送说话协议，格式为 s:内容:p\r\n
 * content: 说话内容字符串
 * @return 发送成功返回RT_EOK，失败返回RT_ERROR
 */
rt_err_t uart_send_speak(const char *content)
{
    if (!serial || !content)
    {
        LOG_E("uart_send_speak: serial or content is NULL");
        return RT_ERROR;
    }
    char buf[500];
    int len = rt_snprintf(buf, sizeof(buf), "s:%s:p\r\n", content);
    if (len <= 0 || len >= (int)sizeof(buf))
    {
        LOG_E("uart_send_speak: snprintf error, len=%d\n", len);
        return RT_ERROR;
    }
    rt_kprintf("uart_send_speak: %s\n", buf);
    rt_size_t sent = rt_device_write(serial, 0, buf, len);
    return (sent == len) ? RT_EOK : RT_ERROR;
}

/*
 * ai对话线程
 */
static void ai_dialog_thread_entry(void *parameter)
{
    static char rx_buffer[2048]; // 接收缓冲区
    while (1)
    {
        rt_mq_recv(ai_dialog_mq, rx_buffer, sizeof(rx_buffer), RT_WAITING_FOREVER);
        LOG_D("ai_dialog_thread_entry: Received message: %s", rx_buffer);
        llm_send_chat(rx_buffer); // 发送对话内容
    }
}

/*
 * 串口初始化
 * @return RT_EOK on success, RT_ERROR on failure
 */
rt_err_t uart_init(void)
{
    /* 查找系统中的串口设备 */
    serial = rt_device_find(SAMPLE_UART_NAME);
    if (!serial)
    {
        LOG_E("uart_init: failed to find device %s\n", SAMPLE_UART_NAME);
        return RT_ERROR;
    }
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 初始化配置参数 */
    config.baud_rate = BAUD_RATE_115200;                       /* 设置波特率为115200 */
    config.bufsz = 25600;                                      /* 缓冲区大小为12800*/
    if (rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config) != RT_EOK)
    {
        LOG_E("uart_init: failed to configure device %s\n", SAMPLE_UART_NAME);
        return RT_ERROR;
    }
    /* 初始化信号量 */
    rx_sem = rt_sem_create("rx_sem", 0, RT_IPC_FLAG_FIFO);
    if (rx_sem == RT_NULL)
    {
        LOG_E("uart_init: failed to create rx semaphore\n");
        rt_device_close(serial);
        return RT_ERROR;
    }
    /* 创建串口处理线程 */
    rt_thread_t thread = rt_thread_create("uart1_process", uart1_thread_entry, RT_NULL, 5120, 25, 10);
    if (thread == RT_NULL)
    {
        LOG_E("uart_init: failed to create uart1 thread\n");
        rt_sem_delete(rx_sem);
        rt_device_close(serial);
        return RT_ERROR;
    }
    else
    {
        rt_thread_startup(thread); // 启动线程
    }
    ai_dialog_mq = rt_mq_create("ai_dialog_mq", sizeof(char) * 2048, 10, RT_IPC_FLAG_FIFO);
    rt_thread_t ai_chat_thread = rt_thread_create("ai_chat_thread", ai_dialog_thread_entry, RT_NULL, 5120, 20, 100);
    if (ai_chat_thread == RT_NULL)
    {
        LOG_E("uart_init: failed to create ai chat thread\n");
        rt_mq_delete(ai_dialog_mq);
        rt_sem_delete(rx_sem);
        rt_device_close(serial);
        return RT_ERROR;
    }
    else
    {
        rt_thread_startup(ai_chat_thread); // 启动AI对话线程
    }
    /* 设置接收回调函数 */
    if (rt_device_set_rx_indicate(serial, uart_rx_ind) != RT_EOK)
    {
        LOG_E("uart_init: failed to set rx indicate for device %s\n", SAMPLE_UART_NAME);
        return RT_ERROR;
    }

    if (rt_device_open(serial, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        LOG_E("uart_init: failed to open device %s\n", SAMPLE_UART_NAME);
        return RT_ERROR;
    }
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(uart_init);

/*=====================================串口测试=====================================================*/
/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口5接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{
    char ch;

    while (1)
    {
        /* 从串口5读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(rx_sem, RT_WAITING_FOREVER);
        }
        /* 读取到的数据通过串口5错位输出 */
        ch = ch + 1;
        rt_device_write(serial, 0, &ch, 1);
    }
}

static int uart_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    char str[] = "hello RT-Thread!\r\n";

    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }

    /* 查找系统中的串口设备 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    rt_kprintf("find %s !\n", uart_name);

    /* 初始化信号量 */
    rx_sem = rt_sem_create("rx_sem", 0, RT_IPC_FLAG_FIFO);
    if (rx_sem == RT_NULL)
    {
        rt_kprintf("create rx_sem failed!\n");
        return RT_ERROR;
    }
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
    /* 发送字符串 */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));
    rt_kprintf("write to %s done!\n", uart_name);

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_kprintf("thread startup!\n");
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(uart_sample, uart device sample);

static int uart_send_chinese(int argc, char *argv[])
{
    // 利用s::p协议发送中文

    rt_sprintf(argv[1], "%s", argv[1]);
    uart_send_speak(argv[1]);
}
MSH_CMD_EXPORT(uart_send_chinese, uart send protocol test);
