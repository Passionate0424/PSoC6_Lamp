/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "led_brightness_control.h"
#include "uart1.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <time.h>
#include <drivers/alarm.h>
#include <gimbal_pid.h>
#include "cyhal_rtc.h"
#include "gui_guider.h"
#include "llm_contory.h"
#include "gimbal_pid.h"
extern cyhal_rtc_t rtc_obj;

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

bool btn1_status = false; // false处于关灯状态，true处于开灯状态
bool guard_status = 0;    // 0:未进入guard模式 1:进入guard模式

int bar_energy_value;
char bar_energy_char[5];
int bar_carefulness_value;
char bar_carefulness_char[5];
static bool work_btn_status = false;

static void home_btn_led_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        // 还需适配灯的开关
        if (!btn1_status)
        {
            led_request_on(); // 开灯
            lv_obj_set_style_bg_color(guider_ui.home_btn_led, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.home_btn_led_label, "ON");
            btn1_status = true;
        }
        else
        {
            led_request_off(); // 关灯
            lv_obj_set_style_bg_color(guider_ui.home_btn_led, lv_color_hex(0x8c8d8e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.home_btn_led_label, "OFF");
            btn1_status = false;
        }

        break;
    }
    default:
        break;
    }
}

static void home_bright_slider_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        // 根据滑条调节亮度
        led_brightness = lv_slider_get_value(guider_ui.home_bright_slider);
        // rt_kprintf("[lvgl]LED Brightness: %d\n", led_brightness);
        break;
    }
    default:
        break;
    }
}

static void home_sw_2_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        lv_obj_t *status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? true : false;
        // 还需适配自动调光和手动调光

        switch (status)
        {
        case (true):
        {
            // 开关打开：显示滑块
            // lv_obj_add_flag(guider_ui.screen_bright_slider, LV_OBJ_FLAG_DISABLED);     // 4. 添加隐藏标记，让滑块不可见
            lv_obj_clear_flag(guider_ui.home_bright_slider, LV_OBJ_FLAG_CLICKABLE);                                                // 禁止点击
            lv_obj_clear_flag(guider_ui.home_bright_slider, LV_OBJ_FLAG_PRESS_LOCK);                                               // 禁止按压/拖动
            lv_obj_set_style_bg_color(guider_ui.home_bright_slider, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);      // 轨道
            lv_obj_set_style_bg_color(guider_ui.home_bright_slider, lv_color_hex(0x808080), LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器
            lv_obj_set_style_bg_color(guider_ui.home_bright_slider, lv_color_hex(0x808080), LV_PART_KNOB | LV_STATE_DEFAULT);      // 旋钮
            led_bright_mode = 0;                                                                                                   // 设置为自动调光模式
            break;
        }
        case (false):
        {
            // 开关关闭：隐藏滑块
            // lv_obj_clear_flag(guider_ui.screen_bright_slider, LV_OBJ_FLAG_DISABLED);    // 1. 先清除隐藏标记，让滑块可见
            lv_obj_add_flag(guider_ui.home_bright_slider, LV_OBJ_FLAG_CLICKABLE);                                                  // 禁止点击
            lv_obj_add_flag(guider_ui.home_bright_slider, LV_OBJ_FLAG_PRESS_LOCK);                                                 // 禁止按压/拖动
            lv_obj_set_style_bg_color(guider_ui.home_bright_slider, lv_color_hex(0x0c6aff), LV_PART_MAIN | LV_STATE_DEFAULT);      // 轨道
            lv_obj_set_style_bg_color(guider_ui.home_bright_slider, lv_color_hex(0x0c6aff), LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器
            lv_obj_set_style_bg_color(guider_ui.home_bright_slider, lv_color_hex(0x0c6aff), LV_PART_KNOB | LV_STATE_DEFAULT);      // 旋钮
            led_bright_mode = 1;                                                                                                   // 设置为手动调光模式
            break;
        }
        }
        break;
    }
    default:
        break;
    }
}

static void home_btn_monitor_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.monitor, guider_ui.monitor_del, &guider_ui.home_del, setup_scr_monitor, LV_SCR_LOAD_ANIM_NONE, 20, 20, true, false);
        break;
    }
    default:
        break;
    }
}

static void home_btn_guard_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.guader, guider_ui.guader_del, &guider_ui.home_del, setup_scr_guader, LV_SCR_LOAD_ANIM_NONE, 20, 20, true, false);
        // 适配守护模式开关
        guard_status = 1;
        uart_send_protocol("c", 17);        // 发送monitor模式关
        rt_thread_mdelay(100);              // 等待100ms
        uart_send_protocol("c", 2);         // 发送追踪模式关
        gimbal_stop_tracking();             // 停止追踪线程
        rt_thread_mdelay(100);              // 等待100ms
        uart_send_protocol("c", 16);        // 发送推流关
        rt_thread_mdelay(100);              // 等待100ms
        sg90_set_angle(gimbal.servo_x, 90); // 设置初始角度
        sg90_set_angle(gimbal.servo_y, 80); // 设置初始角度
        uart_send_protocol("c", 1);         // 发送守护模式开指令
        break;
    }
    default:
        break;
    }
}

static void home_btn_setting_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.settings, guider_ui.settings_del, &guider_ui.home_del, setup_scr_settings, LV_SCR_LOAD_ANIM_NONE, 20, 20, true, false);
        break;
    }
    default:
        break;
    }
}

static void home_btn_ai_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_LONG_PRESSED:
    {
        // 长按开始录音

        lv_obj_set_style_bg_color(guider_ui.home_btn_ai, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(guider_ui.home_btn_ai_label, "录音中...");
        uart_send_protocol("c", 05);

        break;
    }
    case LV_EVENT_RELEASED:
    {
        // 松开结束录音

        lv_obj_set_style_bg_color(guider_ui.home_btn_ai, lv_color_hex(0x00E0FF), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(guider_ui.home_btn_ai_label, "按住说话");
        uart_send_protocol("c", 15); // 发送结束录音指令
        break;
    }
    default:
        break;
    }
}

void events_init_home(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->home_btn_led, home_btn_led_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_bright_slider, home_bright_slider_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_sw_2, home_sw_2_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_btn_monitor, home_btn_monitor_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_btn_guard, home_btn_guard_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_btn_setting, home_btn_setting_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->home_btn_ai, home_btn_ai_event_handler, LV_EVENT_ALL, ui);
}

static void monitor_temperature_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clear_flag(guider_ui.monitor_win_temperature, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void monitor_humidity_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clear_flag(guider_ui.monitor_win_humidity, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void monitor_heart_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clear_flag(guider_ui.monitor_win_heart, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void monitor_breath_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        lv_obj_clear_flag(guider_ui.monitor_win_breath, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void monitor_btn_env_advice_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        // 适配调用ai获取环境建议

        llm_send_env();
        // rt_mutex_release(llm_mutex);

        break;
    }
    default:
        break;
    }
}

static void monitor_bar_energy_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        bar_energy_value = lv_bar_get_value(guider_ui.monitor_bar_energy);
        // sprintf(bar_energy_char, "%d%%", bar_energy_value);
        // lv_label_set_text(guider_ui.monitor_label_energy, bar_energy_char);
        // 动态变色
        if (bar_energy_value < 30)
        {
            lv_obj_set_style_bg_color(guider_ui.monitor_bar_energy, lv_color_hex(0xff0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_bg_color(guider_ui.monitor_bar_energy, lv_color_hex(0x2195f6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        break;
    }
    default:
        break;
    }
}

static void monitor_bar_carefulness_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        bar_carefulness_value = lv_bar_get_value(guider_ui.monitor_bar_carefulness);
        // sprintf(bar_carefulness_char, "%d%%", bar_carefulness_value);
        // lv_label_set_text(guider_ui.monitor_label_carefulness, bar_carefulness_char);
        // 动态变色
        if (bar_carefulness_value < 30)
        {
            lv_obj_set_style_bg_color(guider_ui.monitor_bar_carefulness, lv_color_hex(0xff0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_bg_color(guider_ui.monitor_bar_carefulness, lv_color_hex(0x2195f6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        break;
    }
    default:
        break;
    }
}

#include <rtthread.h>

static rt_timer_t work_timer = RT_NULL;
static rt_timer_t rest_timer = RT_NULL;
rt_timer_t water_timer = RT_NULL;
static float work_time = 0; // 工作时间
static float rest_time = 0;
static rt_int8_t loop_count = 0; // 循环次数
static float water_interval = 0; // 喝水提醒间隔时间
static rt_int8_t work_count = 0; // 工作次数计数
static rt_int8_t rest_count = 0; // 休息次数计数

static void work_timer_callback(void *parameter)
{
    rt_kprintf("[lvgl]休息时间到！\n");
    // 这里可以添加工作结束后的UI、串口等业务逻辑
    monitor_data.is_working = false; // 设置为休息状态
    if (rest_count > 0)
    {
        rest_count--;
        rt_kprintf("[lvgl]启动休息定时器，剩余休息次数: %d\n", rest_count);
        uart_send_protocol("c", 8); // 发送提醒休息指令
        rt_timer_start(rest_timer);
    }
}

static void rest_timer_callback(void *parameter)
{
    // 这里可以添加休息结束后的UI、串口等业务逻辑
    if (work_count > 0)
    {
        monitor_data.is_working = true; // 设置为工作状态
        rt_kprintf("[lvgl]工作时间到！\n");
        work_count--;
        rt_kprintf("[lvgl]启动工作定时器，剩余工作次数: %d\n", work_count);
        uart_send_protocol("c", 18); // 发送提醒工作指令
        rt_timer_start(work_timer);
    }
    else
    {
        uart_send_protocol("c", 17);     // 发送工作模式结束指令
        rt_sem_release(lv_rest_end_sem); // 休息结束，发送信号通知
    }
}

static void water_timer_callback(void *parameter)
{
    rt_kprintf("[lvgl]喝水提醒！\n");
    // 这里可以添加喝水提醒的UI、蜂鸣器等业务逻辑
    uart_send_protocol("c", 9);  // 发送喝水提醒指令
    rt_timer_start(water_timer); // 循环提醒
}

void drink_timer_restart(void)
{
    if (water_timer)
    {
        // 如果喝水定时器已存在，先删除它
        rt_timer_delete(water_timer);
        water_timer = RT_NULL; // 重置喝水定时器指针
    }
    if (water_timer == RT_NULL)
    {
        water_timer = rt_timer_create("water_timer", water_timer_callback, RT_NULL, (water_interval * 60 * RT_TICK_PER_SECOND), RT_TIMER_FLAG_PERIODIC);
        if (water_timer == RT_NULL)
        {
            rt_kprintf("[lvgl]创建喝水定时器失败！\n");
            return;
        }
    }
    rt_timer_start(water_timer);
}

static void monitor_work_btn_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        // 适配监测模式的开关
        if (!work_btn_status)
        {
            // 开
            // 工作时间
            if (lv_obj_has_state(guider_ui.monitor_sw_demo, LV_STATE_CHECKED) != true)
            {
                switch (lv_dropdown_get_selected(guider_ui.monitor_ddlist_work_time))
                {
                case 0:
                    work_time = 30; // 30min
                    break;
                case 1:
                    work_time = 60; // 1h
                    break;
                case 2:
                    work_time = 90; // 1.5h
                    break;
                case 3:
                    work_time = 120; // 2h
                    break;
                case 4:
                    work_time = 2; // 2min
                    break;
                default:
                    work_time = 0;
                    rt_kprintf("[lvgl]Invalid work time selection.\n");
                    break;
                }
                // 休息时间
                switch (lv_dropdown_get_selected(guider_ui.monitor_ddlist_rest_time))
                {
                case 0:
                    rest_time = 5; // 5min
                    break;
                case 1:
                    rest_time = 10; // 10min
                    break;
                case 2:
                    rest_time = 15; // 15min
                    break;
                case 3:
                    rest_time = 30; // 30min
                    break;
                case 4:
                    rest_time = 60; // 1h
                    break;
                case 5:
                    rest_time = 1; // 1min
                    break;
                default:
                    rest_time = 0;
                    rt_kprintf("[lvgl]Invalid rest time selection.\n");
                    break;
                }
                // 循环次数
                switch (lv_dropdown_get_selected(guider_ui.monitor_ddlist_loop))
                {
                case 0:
                    loop_count = 1; // 1次
                    break;
                case 1:
                    loop_count = 2; // 2次
                    break;
                case 2:
                    loop_count = 3; // 3次
                    break;
                case 3:
                    loop_count = 4; // 4次
                    break;
                case 4:
                    loop_count = 5;
                    break;
                }
                // 喝水提醒间隔时间
                switch (lv_dropdown_get_selected(guider_ui.monitor_ddlist_water))
                {
                case 0:
                    water_interval = 30; // 30min
                    break;
                case 1:
                    water_interval = 60; // 1h
                    break;
                case 2:
                    water_interval = 90; // 1.5h
                    break;
                case 3:
                    water_interval = 2; // 2min
                    break;
                case 4:
                    water_interval = 0.5; // 30s
                    break;
                default:
                    water_interval = 0;
                    rt_kprintf("[lvgl]Invalid water reminder interval selection.\n");
                    break;
                }

                // 更新分数计算结构体
                monitor_setting.work_time = work_time; // 工作时间（分钟）
                monitor_setting.rest_time = rest_time; // 休息时间（分钟）
                monitor_setting.loop = loop_count;     // 循环次数
                monitor_data.work_duration = 0;
                monitor_data.phone_duration = 0; // 初始化工作和玩手机持续时间
                monitor_data.sum_time = 0;       // 初始化总时间
                monitor_data.is_working = true;  // 设置为工作状态
                energy = 100.0f;                 // 初始化能量值
                work_score = 100.0f;             // 初始化工作分数
                carefulness = 100.0f;            // 初始化专注度

                // 设置时钟计数
                work_count = loop_count - 1;
                rest_count = loop_count;

                // 删除旧定时器
                if (work_timer)
                {
                    rt_timer_delete(work_timer);
                    work_timer = RT_NULL; // 重置定时器指针
                }
                if (rest_timer)
                {
                    rt_timer_delete(rest_timer);
                    rest_timer = RT_NULL; // 重置定时器指针
                }
                if (water_timer)
                {
                    rt_timer_delete(water_timer);
                    water_timer = RT_NULL; // 重置喝水定时器指针
                }

                // 创建新定时器
                work_timer = rt_timer_create("work", work_timer_callback, RT_NULL, work_time * 60 * RT_TICK_PER_SECOND, RT_TIMER_FLAG_ONE_SHOT);
                rest_timer = rt_timer_create("rest", rest_timer_callback, RT_NULL, rest_time * 60 * RT_TICK_PER_SECOND, RT_TIMER_FLAG_ONE_SHOT);
                // work_timer = rt_timer_create("work", work_timer_callback, RT_NULL, 6 * RT_TICK_PER_SECOND, RT_TIMER_FLAG_ONE_SHOT);
                // rest_timer = rt_timer_create("rest", rest_timer_callback, RT_NULL, 6 * RT_TICK_PER_SECOND, RT_TIMER_FLAG_ONE_SHOT);
                water_timer = rt_timer_create("water", water_timer_callback, RT_NULL, (int)(water_interval * 60 * RT_TICK_PER_SECOND), RT_TIMER_FLAG_ONE_SHOT);

                // 启动工作和喝水定时器
                rt_timer_start(work_timer);
                rt_timer_start(water_timer);
                drink_count = 0; // 初始化喝水计数
                rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
                // 更新喝水计数显示
                lv_label_set_text_fmt(guider_ui.monitor_label_drink, "%d", drink_count);
                rt_mutex_release(lv_mutex);

                uart_send_protocol("c", 07); // 发送工作模式启动指令

                // 启动工作计算线程
                monitor_calc_start();

                rt_kprintf("[lvgl]工作定时器启动: %.1f分钟，休息定时器: %.1f分钟，喝水提醒: %.1f分钟\n", work_time, rest_time, water_interval);
            }
            else
            {
                // 演示模式，直接启动工作定时器
                work_time = 0.6;        // 30s
                rest_time = 0.16;      // 10秒
                water_interval = 0.33; // 20秒

                monitor_setting.work_time = work_time; // 工作时间（分钟）
                monitor_setting.rest_time = rest_time; // 休息时间（分钟）
                monitor_setting.loop = 2;              // 循环次数

                monitor_data.work_duration = 0;
                monitor_data.phone_duration = 0; // 初始化工作和玩手机持续时间
                monitor_data.sum_time = 0;       // 初始化总时间
                monitor_data.is_working = true;  // 设置为工作状态
                energy = 100.0f;                 // 初始化能量值
                work_score = 100.0f;             // 初始化工作分数
                carefulness = 100.0f;            // 初始化专注度

                work_count = 1;
                rest_count = 2; // 演示模式，工作1次，休息2次

                if (work_timer)
                {
                    rt_timer_delete(work_timer);
                    work_timer = RT_NULL; // 重置定时器指针
                }
                if (rest_timer)
                {
                    rt_timer_delete(rest_timer);
                    rest_timer = RT_NULL; // 重置定时器指针
                }
                if (water_timer)
                {
                    rt_timer_delete(water_timer);
                    water_timer = RT_NULL; // 重置喝水定时器指针
                }

                work_timer = rt_timer_create("work", work_timer_callback, RT_NULL, (int)(work_time * 60 * RT_TICK_PER_SECOND), RT_TIMER_FLAG_ONE_SHOT);
                rest_timer = rt_timer_create("rest", rest_timer_callback, RT_NULL, (int)(rest_time * 60 * RT_TICK_PER_SECOND), RT_TIMER_FLAG_ONE_SHOT);
                water_timer = rt_timer_create("water", water_timer_callback, RT_NULL, (int)(water_interval * 60 * RT_TICK_PER_SECOND), RT_TIMER_FLAG_ONE_SHOT);

                rt_timer_start(work_timer);
                rt_timer_start(water_timer);
                drink_count = 0; // 初始化喝水计数
                rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
                // 更新喝水计数显示
                lv_label_set_text_fmt(guider_ui.monitor_label_drink, "%d", drink_count);
                rt_mutex_release(lv_mutex);

                uart_send_protocol("c", 07); // 发送工作模式启动指令

                monitor_calc_start(); // 启动工作计算线程

                rt_kprintf("==================演示模式开始==================\n");
            }
            lv_obj_set_style_bg_color(guider_ui.monitor_work_btn, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.monitor_work_btn_label, "STOP");
            lv_obj_clear_flag(guider_ui.monitor_spinner_1, LV_OBJ_FLAG_HIDDEN);
            work_btn_status = true;
        }
        else
        {
            // 关闭定时器
            if (work_timer)
            {
                rt_timer_delete(work_timer);
            }
            work_timer = RT_NULL; // 重置定时器指针}
            if (rest_timer)
            {
                rt_timer_delete(rest_timer);
                rest_timer = RT_NULL; // 重置定时器指针
            }
            if (water_timer)
            {
                rt_timer_delete(water_timer);
                water_timer = RT_NULL; // 重置定时器指针
            }
            rt_kprintf("[lvgl]所有定时器已关闭\n");
            // 关
            lv_obj_set_style_bg_color(guider_ui.monitor_work_btn, lv_color_hex(0x8c8d8e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.monitor_work_btn_label, "START");
            lv_obj_add_flag(guider_ui.monitor_spinner_1, LV_OBJ_FLAG_HIDDEN);
            monitor_calc_stop();         // 停止工作计算线程
            uart_send_protocol("c", 17); // 发送工作模式结束指令
            work_btn_status = false;
        }
        break;
    }
    default:
        break;
    }
}

static void monitor_ddlist_work_time_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        uint16_t id = lv_dropdown_get_selected(guider_ui.monitor_ddlist_work_time);
        // 适配监测模式连续工作时间设置
        // 30min-0 1h-1 1.5h-2 2h-3 2min-4
        rt_kprintf("[lvgl]Selected work time: %d\n", id);
        break;
    }
    default:
        break;
    }
}

static void monitor_ddlist_rest_time_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        uint16_t id = lv_dropdown_get_selected(guider_ui.monitor_ddlist_rest_time);
        // 适配监测模式休息时间设置
        // 5min-0 10min-1 15min-2 30min-3 1h-4 1min-5
        rt_kprintf("[lvgl]Selected rest time: %d\n", id);
        break;
    }
    default:
        break;
    }
}

static void monitor_ddlist_loop_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        uint16_t id = lv_dropdown_get_selected(guider_ui.monitor_ddlist_loop);
        // 适配监测模式循环次数设置
        // 1 2 3 4 5
        rt_kprintf("[lvgl]Selected loop count: %d\n", id);
        break;
    }
    default:
        break;
    }
}

static void monitor_ddlist_water_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        uint16_t id = lv_dropdown_get_selected(guider_ui.monitor_ddlist_water);
        // 适配喝水提醒间隔时间
        // 30min-0 1h-1 1.5h-2 2min-3 30s-4
        rt_kprintf("[lvgl]Selected water reminder interval: %d\n", id);
        break;
    }
    default:
        break;
    }
}

static void monitor_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.monitor_del, setup_scr_home, LV_SCR_LOAD_ANIM_NONE, 20, 20, true, false);
        break;
    }
    default:
        break;
    }
}

static void monitor_win_heart_item0_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.monitor_win_heart, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void monitor_win_breath_item0_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.monitor_win_breath, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void monitor_win_temperature_item0_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.monitor_win_temperature, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

static void monitor_win_humidity_item0_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        lv_obj_add_flag(guider_ui.monitor_win_humidity, LV_OBJ_FLAG_HIDDEN);
        break;
    }
    default:
        break;
    }
}

void events_init_monitor(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->monitor_temperature, monitor_temperature_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_humidity, monitor_humidity_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_heart, monitor_heart_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_breath, monitor_breath_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_btn_env_advice, monitor_btn_env_advice_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_bar_energy, monitor_bar_energy_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_bar_carefulness, monitor_bar_carefulness_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_work_btn, monitor_work_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_ddlist_work_time, monitor_ddlist_work_time_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_ddlist_rest_time, monitor_ddlist_rest_time_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_ddlist_loop, monitor_ddlist_loop_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_ddlist_water, monitor_ddlist_water_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_btn_1, monitor_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_win_heart_item0, monitor_win_heart_item0_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_win_breath_item0, monitor_win_breath_item0_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_win_temperature_item0, monitor_win_temperature_item0_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->monitor_win_humidity_item0, monitor_win_humidity_item0_event_handler, LV_EVENT_ALL, ui);
}

static void settings_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.settings_del, setup_scr_home, LV_SCR_LOAD_ANIM_NONE, 20, 20, true, false);
        break;
    }
    default:
        break;
    }
}

static void settings_ddlist_switch_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        uint16_t id = lv_dropdown_get_selected(guider_ui.settings_ddlist_switch);
        rt_kprintf("[lvgl]Selected switch mode: %d\n", id);
        // 适配台灯开关模式设置
        if (id == 0)
        {
            // 自动开关
            lv_obj_add_flag(guider_ui.home_btn_led, LV_OBJ_FLAG_HIDDEN);
            led_auto_switch(); // 切换为自动开关模式`
        }
        else if (id == 1)
        {
            // 手动开关
            lv_obj_clear_flag(guider_ui.home_btn_led, LV_OBJ_FLAG_HIDDEN);
            led_manual_switch(); // 切换为手动开关模式
        }
        break;
    }
    default:
        break;
    }
}

static void settings_ddlist_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        uint16_t id = lv_dropdown_get_selected(guider_ui.settings_ddlist_1);
        rt_kprintf("[lvgl]Selected tracking mode: %d\n", id);
        // 适配台灯追踪模式设置
        // 关-0 书本-1 手-2
        switch (id)
        {
        case 0:
            // 关闭追踪模式
            uart_send_protocol("c", 2); // 发送关闭追踪模式指令
            // 关闭追踪
            gimbal_stop_tracking();             // 停止追踪
            sg90_set_angle(gimbal.servo_x, 90); // 设置初始角度
            sg90_set_angle(gimbal.servo_y, 80); // 设置初始角度
            break;
        case 1:
            // 书本追踪模式
            uart_send_protocol("c", 4); // 发送书本追踪模式指令
            // 开启书本追踪
            sg90_set_angle(gimbal.servo_x, 90); // 设置初始角度
            sg90_set_angle(gimbal.servo_y, 80); // 设置初始角度
            rt_thread_mdelay(500);              // 等待舵机稳定
            gimbal_start_tracking();            // 开始追踪
            break;
        case 2:
            // 手追踪模式
            uart_send_protocol("c", 3); // 发送手追踪模式指令
            // 开启手追踪
            sg90_set_angle(gimbal.servo_x, 90); // 设置初始角度
            sg90_set_angle(gimbal.servo_y, 80); // 设置初始角度
            rt_thread_mdelay(500);              // 等待舵机稳定
            gimbal_start_tracking();            // 开始手追踪
            break;
        }
        break;
    }
    }
}

static void settings_btn_2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.wifi_connect, guider_ui.wifi_connect_del, &guider_ui.settings_del, setup_scr_wifi_connect, LV_SCR_LOAD_ANIM_NONE, 20, 20, true, false);
        break;
    }
    default:
        break;
    }
}

void events_init_settings(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->settings_btn_1, settings_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->settings_ddlist_switch, settings_ddlist_switch_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->settings_ddlist_1, settings_ddlist_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->settings_btn_2, settings_btn_2_event_handler, LV_EVENT_ALL, ui);
}

static void wifi_connect_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.settings, guider_ui.settings_del, &guider_ui.wifi_connect_del, setup_scr_settings, LV_SCR_LOAD_ANIM_NONE, 20, 20, true, false);
        break;
    }
    default:
        break;
    }
}

void events_init_wifi_connect (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->wifi_connect_btn_1, wifi_connect_btn_1_event_handler, LV_EVENT_ALL, ui);
}

static void guader_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {

        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.guader_del, setup_scr_home, LV_SCR_LOAD_ANIM_NONE, 20, 20, true, false);
        guard_status = 0;
        uart_send_protocol("c", 16); // 发送推流关
        rt_thread_mdelay(100);       // 等待100ms
        uart_send_protocol("c", 11); // 发送守护模式关闭指令
        int id = lv_dropdown_get_selected(guider_ui.settings_ddlist_1);
        switch (id)
        {
        case 0:
            uart_send_protocol("c", 2);
            break;
        case 1:
            uart_send_protocol("c", 4);
            gimbal_start_tracking(); // 开启书本追踪
            break;
        case 2:
            uart_send_protocol("c", 3);
            gimbal_start_tracking(); // 开启手追踪
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

void events_init_guader(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->guader_btn_1, guader_btn_1_event_handler, LV_EVENT_ALL, ui);
}

void events_init(lv_ui *ui)
{
}
