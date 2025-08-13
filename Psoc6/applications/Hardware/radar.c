/* radar.c */
#include "radar.h"
#include <string.h>
#include <stdint.h>
#include "../lcd_rtthread.h" // 包含 LCD 显示函数头文件
#include "gui_guider.h"      // 包含 LVGL GUI 相关头文件

#define useLCD 1 // 使用 LCD 显示

/* 将静态变量改为全局变量 */
uint8_t human_presence_status = 0;     // 人体存在状态
uint8_t respiration_value = 0;         // 呼吸频率
uint8_t heart_rate_value = 0;          // 心率值
uint16_t human_distance = 0;           // 人体距离（单位：cm）
int16_t human_position[3] = {0, 0, 0}; // 人体方位（x, y, z，单位：cm）
uint8_t body_motion_index = 0;         // 体动指数（0-100）

/* 将静态全局变量改为全局变量 */
rt_device_t uart_dev;
radar_data_callback user_callback;
uint8_t rx_buffer[RX_BUFFER_SIZE];
uint16_t rx_index = 0;
rt_mutex_t uart_mutex = RT_NULL;
rt_sem_t radar_rx_sem;
rt_size_t uart_rx_len = 0;

// LCD 显示相关参数
#define LCD_RADAR_START_X 10   // 雷达信息显示起始 X 坐标
#define LCD_RADAR_START_Y 50   // 雷达信息显示起始 Y 坐标
#define LCD_LINE_HEIGHT 20     // 行高
#define LCD_TEXT_SIZE 16       // 字体大小
#define LCD_TEXT_COLOR WHITE   // 文本颜色
#define LCD_BG_COLOR BLACK     // 背景颜色
#define LCD_TITLE_COLOR GREEN  // 标题颜色
#define LCD_VALUE_COLOR YELLOW // 数值颜色
#define LCD_ALERT_COLOR RED    // 警告颜色

// radar 显示buffer
static char radar_disp_buffer[15];

/**
 * @brief 计算校验和
 *
 * @param data 需要计算校验和的数据
 * @param len 数据长度
 * @return uint8_t 计算得到的校验和
 */
static uint8_t calculate_checksum(uint8_t *data, uint16_t len)
{
    uint8_t sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += data[i];
    }
    return sum;
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
        // rt_kprintf("int\n");
        uart_rx_len += size;
        rt_sem_release(radar_rx_sem);
        // 关闭串口中断
        rt_device_control(uart_dev, RT_DEVICE_CTRL_CLR_INT, RT_NULL);
    }
    return RT_EOK;
}

/**
 * @brief 初始化雷达模块
 *
 * @param uart_name 串口设备名称
 * @param callback 数据回调函数
 * @return int 返回操作结果，成功为RT_EOK
 */
int radar_init(const char *uart_name, radar_data_callback callback)
{
    uart_dev = rt_device_find(uart_name);
    if (!uart_dev)
        return -RT_ERROR;

    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    config.baud_rate = BAUD_RATE_115200;
    config.data_bits = DATA_BITS_8;
    config.stop_bits = STOP_BITS_1;
    config.parity = PARITY_NONE;
    rt_device_control(uart_dev, RT_DEVICE_CTRL_CONFIG, &config);

    uart_mutex = rt_mutex_create("radar_uart", RT_IPC_FLAG_FIFO);
    user_callback = callback;

    rt_device_open(uart_dev, RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_RDWR);
    rt_device_set_rx_indicate(uart_dev, uart_rx_ind);

    return RT_EOK;
}

/**
 * @brief 发送指令通用函数
 *
 * @param ctrl 控制字
 * @param cmd 命令字
 * @param data 数据区指针
 * @param len 数据区长度
 * @return int 返回操作结果，成功为RT_EOK
 */
static int radar_send_cmd(uint8_t ctrl, uint8_t cmd, uint8_t *data, uint16_t len)
{
    rt_mutex_take(uart_mutex, RT_WAITING_FOREVER);

    uint8_t frame[128] = {0x53, 0x59, ctrl, cmd};
    uint16_t index = 4;

    // 长度标识（大端模式）
    frame[index++] = (len >> 8) & 0xFF;
    frame[index++] = len & 0xFF;

    if (data && len > 0)
    {
        memcpy(&frame[index], data, len);
        index += len;
    }

    // 校验和
    frame[index++] = calculate_checksum(frame, index);

    // 帧尾
    frame[index++] = 0x54;
    frame[index++] = 0x43;

    rt_device_write(uart_dev, 0, frame, index);
    rt_mutex_release(uart_mutex);
    return RT_EOK;
}

/**
 * @brief 人体存在状态查询
 *
 * @return int 返回操作结果，成功为RT_EOK
 */
int radar_query_presence(void)
{
    uint8_t data = 0x0F;
    return radar_send_cmd(CTRL_R_HUMAN_PRESENCE, 0x81, &data, 1);
}

/**
 * @brief 呼吸数值查询
 *
 * @return int 返回操作结果，成功为RT_EOK
 */
int radar_query_respiration(void)
{
    uint8_t data = 0x0F;
    return radar_send_cmd(CTRL_R_RESPIRATION_MONITOR, 0x82, &data, 1);
}

/**
 * @brief 心率数值查询
 *
 * @return int 返回操作结果，成功为RT_EOK
 */
int radar_query_heart_rate(void)
{
    uint8_t data = 0x0F;
    return radar_send_cmd(CTRL_R_HEART_RATE_MONITOR, 0x82, &data, 1);
}

/**
 * @brief 获取人体存在状态
 *
 * @return uint8_t 人体存在状态值，0表示无人，1表示有人
 */
uint8_t get_human_presence_status(void)
{
    rt_mutex_take(uart_mutex, RT_WAITING_FOREVER);
    uint8_t status = human_presence_status;
    rt_mutex_release(uart_mutex);
    return status;
}

/**
 * @brief 获取呼吸数值
 *
 * @return uint8_t 呼吸数值，范围0-35次/分钟
 */
uint8_t get_respiration_value(void)
{
    rt_mutex_take(uart_mutex, RT_WAITING_FOREVER);
    uint8_t value = respiration_value;
    rt_mutex_release(uart_mutex);
    return value;
}

/**
 * @brief 获取心率数值
 *
 * @return uint8_t 心率数值，范围60-120bpm
 */
uint8_t get_heart_rate_value(void)
{
    rt_mutex_take(uart_mutex, RT_WAITING_FOREVER);
    uint8_t value = heart_rate_value;
    rt_mutex_release(uart_mutex);
    return value;
}

/**
 * @brief 获取人体距离值
 *
 * @return uint8_t 人体距离值，单位为cm
 */
uint8_t get_distance_value(void)
{
    rt_mutex_take(uart_mutex, RT_WAITING_FOREVER);
    uint8_t value = human_distance;
    rt_mutex_release(uart_mutex);
    return value;
}

/**
 * @brief 获取体动指数
 *
 * @return uint8_t 体动指数，范围0-100
 */
uint8_t get_body_motion_index(void)
{
    rt_mutex_take(uart_mutex, RT_WAITING_FOREVER);
    uint8_t index = body_motion_index;
    rt_mutex_release(uart_mutex);
    return index;
}

/**
 * @brief 雷达数据处理函数
 *
 * @param ctrl 控制字
 * @param cmd 命令字
 * @param data 数据区指针
 * @param len 数据区长度
 */
static void radar_data_handler(uint8_t ctrl, uint8_t cmd, uint8_t *data, uint16_t len)
{
    // 获取互斥锁保护共享资源
    rt_mutex_take(uart_mutex, RT_WAITING_FOREVER);

    switch (ctrl)
    {
    // 人体存在功能处理（控制字0x80）
    case CTRL_R_HUMAN_PRESENCE:
        switch (cmd)
        {
        // 存在状态查询回复
        case CMD_R_HUMAN_PRESENCE_STATUS:
            if (len >= 1)
            {
                human_presence_status = data[0];
                // rt_kprintf("[Presence] Status: %s\n", data[0] ? "Present" : "Absent");
            }
            break;

        // 人体距离处理
        case CMD_R_HUMAN_DISTANCE_REPORT:
        case CMD_R_HUMAN_DISTANCE_QUERY_RSP:
            if (len == 2)
            {
                human_distance = (data[0] << 8) | data[1];
                // rt_kprintf("[Distance] %d cm\n", human_distance);
            }
            break;

        // 人体方位处理
        case CMD_R_HUMAN_POSITION_REPORT:
        case CMD_R_HUMAN_POSITION_QUERY_RSP:
            if (len == 6)
            {
                // 解析x、y、z坐标（每个2字节，有符号数）
                for (int i = 0; i < 3; i++)
                {
                    uint16_t raw = (data[2 * i] << 8) | data[2 * i + 1];
                    human_position[i] = (int16_t)(raw & 0x7FFF); // 取低15位
                    if (raw & 0x8000)
                        human_position[i] *= -1; // 符号位处理
                }
                // rt_kprintf("[Position] x:%d, y:%d, z:%d cm\n",human_position[0], human_position[1], human_position[2]);
            }
            break;

        // 体动指数处理
        case CMD_R_BODY_MOTION_REPORT:
        case CMD_R_BODY_MOTION_QUERY_RSP:
            if (len == 1)
            {
                body_motion_index = data[0];
                // rt_kprintf("[Body Motion] Index: %d\n", body_motion_index);
            }
            break;

        default:
            // rt_kprintf("Unknown human presence cmd: 0x%02X\n", cmd);
            break;
        }
        break;

    // 呼吸检测功能处理（控制字0x81）
    case CTRL_R_RESPIRATION_MONITOR:
        switch (cmd)
        {
        case CMD_R_RESPIRATION_REPORT:    // 呼吸上报
        case CMD_R_RESPIRATION_QUERY_RSP: // 呼吸查询回复
            if (len >= 1)
            {
                respiration_value = data[0];
                // rt_kprintf("[Respiration] Value: %d/m\n", respiration_value);
            }
            break;

        default:
            // rt_kprintf("Unknown respiration cmd: 0x%02X\n", cmd);
            break;
        }
        break;

    // 心率检测功能处理（控制字0x85）
    case CTRL_R_HEART_RATE_MONITOR:
        switch (cmd)
        {
        case CMD_R_HEART_RATE_REPORT:    // 心率上报
        case CMD_R_HEART_RATE_QUERY_RSP: // 心率查询回复
            if (len >= 1)
            {
                heart_rate_value = data[0];
                // rt_kprintf("[Heart Rate] Value: %d bpm\n", heart_rate_value);
            }
            break;

        default:
            // rt_kprintf("Unknown heart rate cmd: 0x%02X\n", cmd);
            break;
        }
        break;

    default:
        // rt_kprintf("Unknown control: 0x%02X\n", ctrl);
        break;
    }

    // 释放互斥锁
    rt_mutex_release(uart_mutex);
}

/**************** 数据处理线程 ****************/
/**
 * @brief 雷达数据处理线程入口函数
 *
 * @details 此线程负责从串口接收雷达模块返回的数据并进行解析
 *          采用状态机方式解析协议帧结构
 *
 * @param parameter 线程参数(未使用)
 * @return 无
 */
static void radar_data_process_thread_entry(void *parameter)
{
    while (1)
    {
        // rt_thread_mdelay(100);   // 等待数据到达
        uint8_t checksum = 0;    // 校验和计算变量
        static uint8_t ctrl = 0; // 控制字存储
        static uint8_t cmd = 0;  // 命令字存储

        // 等待串口接收到数据的信号量
        rt_sem_take(radar_rx_sem, RT_WAITING_FOREVER);

        // 关闭串口中断，防止数据读取过程中被打断
        rt_device_control(uart_dev, RT_DEVICE_CTRL_CLR_INT, RT_NULL);
        // 从串口设备读取数据到接收缓冲区
        rt_size_t read_len = rt_device_read(uart_dev, 0, &rx_buffer[rx_index], RX_BUFFER_SIZE - rx_index);
        if (read_len > 0)
        {
            rx_index += read_len; // 更新缓冲区索引位置
        }

        static uint8_t state = 0;     // 解析状态机状态
        static uint16_t data_len = 0; // 数据区长度
        uint16_t parse_index = 0;

        do
        {
            uint8_t ch = rx_buffer[parse_index];
            switch (state)
            {
            case 0: // 寻找帧头0x53
                if (ch == 0x53)
                {
                    state = 1;
                    checksum = ch; // 初始化校验和（包含第一个帧头）
                    ctrl = 0;
                    cmd = 0;

                    // rt_kprintf("state: %d Frame Header 1 0x%02X Checksum Init: 0x%02X\n", state, ch, checksum);
                }
                parse_index++;
                break;

            case 1: // 寻找帧头0x59
                if (ch == 0x59)
                {
                    state = 2;
                    checksum += ch; // 累加第二个帧头
                    // rt_kprintf("state: %d Frame Header 2 0x%02X Checksum: 0x%02X\n", state, ch, checksum);
                    parse_index++;
                }
                else
                {
                    state = 0;    // 帧头不匹配，重置状态
                    checksum = 0; // 重置校验和
                }
                break;

            case 2: // 解析控制字、命令字、长度标识
                if (rx_index >= parse_index + 4)
                {
                    // 控制字（1B）+ 命令字（1B）+ 长度标识（2B）共4字节
                    ctrl = rx_buffer[parse_index];
                    cmd = rx_buffer[parse_index + 1];
                    data_len = (rx_buffer[parse_index + 2] << 8) | rx_buffer[parse_index + 3];

                    checksum += ctrl + cmd;                                              // 累加控制字和命令字
                    checksum += rx_buffer[parse_index + 2] + rx_buffer[parse_index + 3]; // 累加长度标识

                    // rt_kprintf("state: %d Control:0x%02X Command:0x%02X Data Length:0x%04X Checksum:0x%02X\n",state, ctrl, cmd, data_len, checksum);

                    state = 3;
                    parse_index += 4; // 跳过已解析的4字节（控制字+命令字+长度标识）
                }
                else
                {
                    parse_index++; // 数据不足，继续搜索
                }
                break;

            case 3: // 验证数据区、校验码和帧尾
                // 所需字节：数据区(data_len) + 校验码(1B) + 帧尾(2B) = data_len + 3字节
                if (rx_index >= parse_index + data_len + 3)
                {
                    // 累加数据区到校验和
                    for (uint16_t i = 0; i < data_len; i++)
                    {
                        checksum += rx_buffer[parse_index + i];
                    }

                    // 获取校验码和帧尾
                    uint8_t received_checksum = rx_buffer[parse_index + data_len];
                    uint8_t tail1 = rx_buffer[parse_index + data_len + 1];
                    uint8_t tail2 = rx_buffer[parse_index + data_len + 2];

                    // rt_kprintf("state: %d Calculated Checksum:0x%02X Received Checksum:0x%02X Tail:0x%02X 0x%02X\n",state, checksum, received_checksum, tail1, tail2);

                    // 校验码和帧尾检测
                    if (checksum == received_checksum && tail1 == 0x54 && tail2 == 0x43)
                    {
                        // rt_kprintf("Frame verification passed!\n");

                        // 提取帧信息（控制字和命令字在帧头之后的第3、4字节）
                        uint8_t *data = &rx_buffer[parse_index];
                        uint16_t len = data_len;

                        if (user_callback)
                        {
                            user_callback(ctrl, cmd, data, len);
                        }
                    }
                    else
                    {
                        rt_kprintf("Frame verification failed! Checksum or tail error\n");
                    }

                    // 移除已解析的完整帧
                    // #ifndef useLCD
                    //                     uint16_t frame_len = 2 + 4 + data_len + 3; // 帧头(2) + 控制字+命令字+长度(4) + 数据+校验+帧尾(data_len+3)
                    //                     memmove(rx_buffer, &rx_buffer[parse_index + data_len + 3], rx_index - (parse_index + data_len + 3));
                    //                     rx_index -= frame_len;
                    //                     parse_index = 0;
                    //                     state = 0;
                    //                     checksum = 0; // 重置校验和
                    // #endif
                    // 清空接收缓冲区 提高实时性
                    memset(rx_buffer, 0, sizeof(rx_buffer));
                    rx_index = 0; // 清空接收缓冲区
                    parse_index = 0;
                    state = 0;
                    checksum = 0; // 重置校验和
                }
                else
                {
                    parse_index++; // 数据不足，继续等待
                }
                break;
            }
        } while (parse_index > 0 && parse_index < rx_index);

        // 重新开启串口中断
        rt_device_control(uart_dev, RT_DEVICE_CTRL_SET_INT, RT_NULL);
    }
}

/**************** 测试线程实现 ****************/
/**
 * @brief 雷达测试线程入口函数
 *
 * @details 此线程用于测试雷达功能，包括初始化雷达、创建数据处理线程，
 *          并定期查询人体存在、呼吸和心率数据
 *
 * @param parameter 线程参数(未使用)
 * @return 无
 */
static void radar_test_thread_entry(void *parameter)
{
    rt_thread_mdelay(2000);

    if (radar_init("uart5", radar_data_handler) != RT_EOK)
    {
        rt_kprintf("Radar initialization failed!\n");
        return;
    }
    rt_kprintf("Radar initialized successfully\n");

    radar_rx_sem = rt_sem_create("radar_rx_sem", 0, RT_IPC_FLAG_FIFO);

    rt_thread_t process_tid = rt_thread_create("radar_proc",
                                               radar_data_process_thread_entry,
                                               RT_NULL,
                                               2048,
                                               16,
                                               20);
    if (process_tid)
        rt_thread_startup(process_tid);

    //    // 在 LCD 上显示雷达状态信息
    //    // 显示标题
    //    LCD_ShowString(10, 10, 220, 20, 16, (u8 *)"====== RADAR STATUS ======", GREEN, BLACK);
    //    LCD_ShowString(10, 30, 100, 20, 16, (u8 *)"Presence:", WHITE, BLACK);
    //    LCD_ShowString(10, 50, 100, 20, 16, (u8 *)"Respiration:", WHITE, BLACK);
    //    LCD_ShowString(130, 50, 50, 20, 16, (u8 *)"bpm", YELLOW, BLACK);
    //    LCD_ShowString(10, 70, 100, 20, 16, (u8 *)"Heart Rate:", WHITE, BLACK);
    //    LCD_ShowString(150, 70, 50, 20, 16, (u8 *)"bpm", YELLOW, BLACK);
    //    LCD_ShowString(10, 90, 100, 20, 16, (u8 *)"Distance:", WHITE, BLACK);
    //    LCD_ShowString(150, 90, 50, 20, 16, (u8 *)"cm", YELLOW, BLACK);
    //    LCD_ShowString(10, 110, 100, 20, 16, (u8 *)"Position:", WHITE, BLACK);
    //    LCD_ShowString(10, 130, 30, 20, 16, (u8 *)"X:", WHITE, BLACK);
    //    LCD_ShowString(70, 130, 30, 20, 16, (u8 *)"cm", YELLOW, BLACK);
    //    LCD_ShowString(100, 130, 30, 20, 16, (u8 *)"Y:", WHITE, BLACK);
    //    LCD_ShowString(160, 130, 30, 20, 16, (u8 *)"cm", YELLOW, BLACK);
    //    LCD_ShowString(190, 130, 30, 20, 16, (u8 *)"Z:", WHITE, BLACK);
    //    LCD_ShowString(250, 130, 30, 20, 16, (u8 *)"cm", YELLOW, BLACK);
    //    LCD_ShowString(10, 150, 100, 20, 16, (u8 *)"Motion Index:", WHITE, BLACK);
    //    LCD_ShowString(140, 150, 70, 20, 16, (u8 *)"/100", YELLOW, BLACK);
    //    // 显示底部分隔线
    //    LCD_ShowString(10, 170, 220, 20, 16, (u8 *)"========================", GREEN, BLACK);

    while (1)
    {
        radar_query_presence();    // 查询人体存在状态
        radar_query_respiration(); // 查询呼吸数值
        radar_query_heart_rate();  // 查询心率数值
        rt_thread_mdelay(1000);    // 1秒查询周期

        // --- 人体存在状态变化自动开关灯逻辑（事件集方式） ---
        static uint8_t last_filtered_status = 0;
        static uint8_t filter_count = 0;
        const uint8_t filter_threshold = 1; // 连续3次确认
        extern void led_request_on(void);
        extern void led_request_off(void);
        static uint8_t light_on = 0;

        if (human_presence_status != last_filtered_status)
        {
            filter_count++;
            if (filter_count >= filter_threshold)
            {
                last_filtered_status = human_presence_status;
                filter_count = 0;
                if (last_filtered_status == 1 && !light_on)
                {
                    led_request_on(); // 发送开灯事件
                    light_on = 1;
                }
                else if (last_filtered_status == 0 && light_on)
                {
                    led_request_off(); // 发送关灯事件
                    light_on = 0;
                }
            }
        }
        else
        {
            filter_count = 0;
        }

        // lvgl 显示数据
        rt_snprintf(radar_disp_buffer, sizeof(radar_disp_buffer), "%dppm", heart_rate_value);
        // rt_kprintf("Heart Rate: %s\n", radar_disp_buffer);
        if (guider_ui.monitor_label_heart_now != NULL && guider_ui.monitor_heart_chart != NULL && guider_ui.monitor_heart_series != NULL)
        {
            rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
            lv_label_set_text(guider_ui.monitor_label_heart_now, radar_disp_buffer);
            lv_chart_set_next_value(guider_ui.monitor_heart_chart, (lv_chart_series_t *)guider_ui.monitor_heart_series, heart_rate_value); // 更新心率图表
            lv_chart_refresh(guider_ui.monitor_heart_chart);                                                          // 刷新心率图表显示
            rt_mutex_release(lv_mutex);
        }

        rt_snprintf(radar_disp_buffer, sizeof(radar_disp_buffer), "%drpm", respiration_value);
        // rt_kprintf("Respiration: %s\n", radar_disp_buffer);
        if (guider_ui.monitor_label_breath_now != NULL && guider_ui.monitor_breath_chart != NULL && guider_ui.monitor_breath_series != NULL)
        {
            rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
            lv_label_set_text(guider_ui.monitor_label_breath_now, radar_disp_buffer);
            lv_chart_set_next_value(guider_ui.monitor_breath_chart, (lv_chart_series_t *)guider_ui.monitor_breath_series, respiration_value); // 更新压力图表
            lv_chart_refresh(guider_ui.monitor_breath_chart);
            rt_mutex_release(lv_mutex);
        }

        //        // 显示人体存在状态
        //
        //        if (human_presence_status)
        //        {
        //            LCD_ShowString(110, 30, 100, 20, 16, (u8 *)"DETECTED", YELLOW, BLACK);
        //        }
        //        else
        //        {
        //            LCD_ShowString(110, 30, 100, 20, 16, (u8 *)"NONE    ", RED, BLACK);
        //        }
        //
        //        // 显示呼吸频率
        //        LCD_ShowNum(110, 50, respiration_value, 2, 16, YELLOW, BLACK);
        //        // 显示心率
        //        LCD_ShowNum(110, 70, heart_rate_value, 3, 16, YELLOW, BLACK);
        //        // 显示距离
        //        LCD_ShowNum(110, 90, human_distance, 3, 16, YELLOW, BLACK);
        //        // 显示位置
        //        if (human_position[0] < 0)
        //        {
        //            LCD_ShowString(35, 130, 10, 20, 16, (u8 *)"-", YELLOW, BLACK);
        //            LCD_ShowNum(40, 130, -human_position[0], 3, 16, YELLOW, BLACK);
        //        }
        //        else
        //        {
        //            LCD_ShowString(35, 130, 10, 20, 16, (u8 *)" ", YELLOW, BLACK);
        //            LCD_ShowNum(30, 130, human_position[0], 4, 16, YELLOW, BLACK);
        //        }
        //        if (human_position[1] < 0)
        //        {
        //            LCD_ShowString(120, 130, 10, 20, 16, (u8 *)"-", YELLOW, BLACK);
        //            LCD_ShowNum(130, 130, -human_position[1], 3, 16, YELLOW, BLACK);
        //        }
        //        else
        //        {
        //            LCD_ShowString(120, 130, 10, 20, 16, (u8 *)" ", YELLOW, BLACK);
        //            LCD_ShowNum(120, 130, human_position[1], 4, 16, YELLOW, BLACK);
        //        }
        //        if (human_position[2] < 0)
        //        {
        //            LCD_ShowString(210, 130, 10, 20, 16, (u8 *)"-", YELLOW, BLACK);
        //            LCD_ShowNum(220, 130, -human_position[2], 3, 16, YELLOW, BLACK);
        //        }
        //        else
        //        {
        //            LCD_ShowString(210, 130, 10, 20, 16, (u8 *)" ", YELLOW, BLACK);
        //            LCD_ShowNum(210, 130, human_position[2], 4, 16, YELLOW, BLACK);
        //        }
        //        // 显示体动指数
        //        LCD_ShowNum(110, 150, body_motion_index, 3, 16, YELLOW, BLACK);
    }
}

/**
 * @brief 雷达测试初始化函数
 *
 * @details 创建并启动雷达测试线程
 *
 * @return int 返回操作结果，成功为RT_EOK
 */
int radar_test_init(void)
{
    rt_thread_t tid = rt_thread_create("radar",
                                       radar_test_thread_entry,
                                       RT_NULL,
                                       1024,
                                       15,
                                       20);
    if (tid)
    {
        rt_thread_startup(tid);
        return RT_EOK;
    }
    return -RT_ERROR;
}

INIT_APP_EXPORT(radar_test_init);
