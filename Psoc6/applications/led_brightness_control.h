
// led_brightness_control.h
#ifndef __LED_BRIGHTNESS_CONTROL_H__
#define __LED_BRIGHTNESS_CONTROL_H__

#include <rtthread.h>

// ================== 全局变量与宏定义 ==================
// 自动/手动调光模式定义
extern rt_bool_t led_bright_mode; // 0:自动调光, 1:手动调光
extern int led_sw_mode; // 0:手动开关, 1:自动开关
extern rt_uint8_t led_brightness;
extern rt_bool_t led_status; // 0:关闭, 1:开启

/* 事件集接口，供外部调用 */
void led_request_on(void);
void led_request_off(void);

#endif // __LED_BRIGHTNESS_CONTROL_H__
