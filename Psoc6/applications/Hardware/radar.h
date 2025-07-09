/* radar.h */
#ifndef __RADAR_H__
#define __RADAR_H__

#include <rtthread.h>
#include <rtdevice.h>


#define RX_BUFFER_SIZE 128


/* 雷达协议帧头帧尾 */
#define FRAME_HEADER         0x5359
#define FRAME_TAIL           0x5443

/* 功能控制字定义 */
#define CTRL_R_HEARTBEAT       0x01
#define CTRL_R_PRODUCT_INFO    0x02
#define CTRL_R_SLEEP_MONITOR   0x84
#define CTRL_R_HUMAN_PRESENCE 0x80      // 人体存在功能控制字
#define CTRL_R_RESPIRATION_MONITOR 0x81 // 呼吸检测功能控制字
#define CTRL_R_HEART_RATE_MONITOR 0x85  // 心率监测功能控制字

/* 命令字定义 - 人体存在功能相关命令 */
// 人体存在功能命令字定义（来自手册8.2节）
#define CMD_R_HUMAN_PRESENCE_STATUS 0x81    // 存在状态查询回复
#define CMD_R_HUMAN_DISTANCE_REPORT 0x04    // 人体距离主动上报
#define CMD_R_HUMAN_DISTANCE_QUERY_RSP 0x84 // 人体距离查询回复
#define CMD_R_HUMAN_POSITION_REPORT 0x05    // 人体方位主动上报
#define CMD_R_HUMAN_POSITION_QUERY_RSP 0x85 // 人体方位查询回复
#define CMD_R_BODY_MOTION_REPORT 0x03       // 体动参数主动上报
#define CMD_R_BODY_MOTION_QUERY_RSP 0x83    // 体动参数查询回复
#define CMD_R_RESPIRATION_REPORT 0x01       // 呼吸参数主动上报
#define CMD_R_RESPIRATION_QUERY_RSP 0x82    // 呼吸参数查询回复
#define CMD_R_HEART_RATE_REPORT 0x02        // 心率参数主动上报
#define CMD_R_HEART_RATE_QUERY_RSP 0x82     // 心率参数查询回复

/* 状态存储变量 */
extern uint8_t human_presence_status;     // 人体存在状态
extern uint8_t respiration_value;         // 呼吸频率
extern uint8_t heart_rate_value;          // 心率值
extern uint16_t human_distance;           // 人体距离（单位：cm）
extern int16_t human_position[3];         // 人体方位（x, y, z，单位：cm）
extern uint8_t body_motion_index;         // 体动指数（0-100）

/* 数据回调函数类型 */
typedef void (*radar_data_callback)(uint8_t ctrl, uint8_t cmd, uint8_t *data, uint16_t len);

extern rt_device_t uart_dev;
extern radar_data_callback user_callback;
extern uint8_t rx_buffer[RX_BUFFER_SIZE];
extern uint16_t rx_index;
extern rt_mutex_t uart_mutex;
extern rt_sem_t radar_rx_sem;
extern rt_size_t uart_rx_len;

/* 初始化雷达模块 */
int radar_init(const char *uart_name, radar_data_callback callback);

/* 获取传感器数据函数 */
uint8_t get_human_presence_status(void);
uint8_t get_respiration_value(void);
uint8_t get_heart_rate_value(void);
uint8_t get_distance_value(void);
uint8_t get_body_motion_index(void);

/* 查询指令函数 */
int radar_query_presence(void);    // 查询人体存在状态
int radar_query_respiration(void); // 查询呼吸数值
int radar_query_heart_rate(void);  // 查询心率数值

/* 测试函数 */
int radar_test_init(void);

#endif
