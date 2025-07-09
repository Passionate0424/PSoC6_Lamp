#ifndef UART1_H
#define UART1_H

#include <rtthread.h>
#include "work_monitor.h"
#include "events_init.h"
#include "gimbal_pid.h"

/**
 * @brief  发送协议数据，格式为 控制字:命令字\r\n
 * @param  ctrl 控制字字符串（如"c"）
 * @param  cmd  命令字int值（如1、11等）
 * @return 发送成功返回RT_EOK，失败返回RT_ERROR
 */
rt_err_t uart_send_protocol(const char *ctrl, int cmd);

/**
 * @brief  发送说话协议，格式为 s:内容:p\r\n
 * @param  content 说话内容字符串
 * @return 发送成功返回RT_EOK，失败返回RT_ERROR
 */
rt_err_t uart_send_speak(const char *content);


#endif // UART1_TEST_H
