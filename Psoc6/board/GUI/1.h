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
#include "cyhal_rtc.h"
extern cyhal_rtc_t rtc_obj;

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

bool btn1_status = false; // false处于关灯状态，true处于开灯状态

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
        uart_send_protocol("c", 01); // 发送守护模式开关指令
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
        sprintf(bar_energy_char, "%d%%", bar_energy_value);
        lv_label_set_text(guider_ui.monitor_label_energy, bar_energy_char);

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
        sprintf(bar_carefulness_char, "%d%%", bar_carefulness_value);
        lv_label_set_text(guider_ui.monitor_label_carefulness, bar_carefulness_char);
        break;
    }
    default:
        break;
    }
}

static rt_alarm_t work_time_alarm;
static rt_alarm_t rest_time_alarm;
static rt_alarm_t water_reminder_alarm;
static struct rt_alarm_setup work_time_alarm_setup;
static struct rt_alarm_setup rest_time_alarm_setup;
static struct rt_alarm_setup water_reminder_alarm_setup;
static rt_int8_t work_time = 0; // 工作时间
static rt_int8_t rest_time = 0;
static rt_int8_t loop_count = 0; // 循环次数
static float water_interval = 0; // 喝水提醒间隔时间
static rt_int8_t work_count = 0; // 工作次数计数
static rt_int8_t rest_count = 0; // 休息次数计数

// 回调函数声明提前，防止未声明使用
static void work_time_alarm_callback(rt_alarm_t alarm, time_t timestamp);
static void rest_time_alarm_callback(rt_alarm_t alarm, time_t timestamp);
static void water_reminder_alarm_callback(rt_alarm_t alarm, time_t timestamp);

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

            // 设置时钟计数
            work_count = loop_count - 1; // 工作次数计数
            rest_count = loop_count;     // 休息次数计数

            time_t now;
            struct tm p_tm;
            now = time(NULL); // 获取当前时间戳

            // 设置闹钟参数
            // 工作时间闹钟
            // time_t t1 = now + work_time * 60;
            time_t t1 = now + 0.1 * 60;
            gmtime_r(&t1, &p_tm); // 获取当前时间
            rt_kprintf("Current time: %02d:%02d:%02d\n", p_tm.tm_hour, p_tm.tm_min, p_tm.tm_sec);
            work_time_alarm_setup.flag = RT_ALARM_ONESHOT;       // 单次闹钟
            work_time_alarm_setup.wktime.tm_year = p_tm.tm_year; // 设置闹钟时间
            work_time_alarm_setup.wktime.tm_mon = p_tm.tm_mon;
            work_time_alarm_setup.wktime.tm_mday = p_tm.tm_mday;
            work_time_alarm_setup.wktime.tm_hour = p_tm.tm_hour;
            work_time_alarm_setup.wktime.tm_min = p_tm.tm_min;
            work_time_alarm_setup.wktime.tm_sec = p_tm.tm_sec;
            // 休息时间闹钟
            time_t t2 = now + rest_time * 60;
            gmtime_r(&t2, &p_tm);                                // 获取当前时间
            rest_time_alarm_setup.flag = RT_ALARM_ONESHOT;       // 单次闹钟
            rest_time_alarm_setup.wktime.tm_year = p_tm.tm_year; // 设置闹钟时间
            rest_time_alarm_setup.wktime.tm_mon = p_tm.tm_mon;
            rest_time_alarm_setup.wktime.tm_mday = p_tm.tm_mday;
            rest_time_alarm_setup.wktime.tm_hour = p_tm.tm_hour;
            rest_time_alarm_setup.wktime.tm_min = p_tm.tm_min;
            rest_time_alarm_setup.wktime.tm_sec = p_tm.tm_sec;
            // 喝水提醒闹钟
            time_t t3 = now + water_interval * 60;
            gmtime_r(&t3, &p_tm);                               // 获取当前时间
            water_reminder_alarm_setup.flag = RT_ALARM_ONESHOT; // 单次闹钟
            water_reminder_alarm_setup.wktime.tm_year = p_tm.tm_year;
            water_reminder_alarm_setup.wktime.tm_mon = p_tm.tm_mon;
            water_reminder_alarm_setup.wktime.tm_mday = p_tm.tm_mday;
            water_reminder_alarm_setup.wktime.tm_hour = p_tm.tm_hour;
            water_reminder_alarm_setup.wktime.tm_min = p_tm.tm_min;
            water_reminder_alarm_setup.wktime.tm_sec = p_tm.tm_sec;

            // 设定闹钟
            work_time_alarm = rt_alarm_create(work_time_alarm_callback, &work_time_alarm_setup);
            if (work_time_alarm == RT_NULL)
            {
                rt_kprintf("[lvgl]Failed to create work time alarm.\n");
                return;
            }
            rest_time_alarm = rt_alarm_create(rest_time_alarm_callback, &rest_time_alarm_setup);
            if (rest_time_alarm == RT_NULL)
            {
                rt_kprintf("[lvgl]Failed to create rest time alarm.\n");
                return;
            }
            water_reminder_alarm = rt_alarm_create(water_reminder_alarm_callback, &water_reminder_alarm_setup);
            if (water_reminder_alarm == RT_NULL)
            {
                rt_kprintf("[lvgl]Failed to create water reminder alarm.\n");
                return;
            }
            if (rt_alarm_start(work_time_alarm) != RT_EOK) // 启动工作时间闹钟
            {
                rt_kprintf("[lvgl]Failed to start work time alarm.\n");
                return;
            }
            // rt_alarm_start(rest_time_alarm);      // 启动休息时间闹钟
            if (rt_alarm_start(water_reminder_alarm) != RT_EOK) // 启动喝水提醒闹
            {
                rt_kprintf("[lvgl]Failed to start water reminder alarm.\n");
                return;
            }

            rt_kprintf("[lvgl]Work time alarm set for %d minutes, rest time alarm set for %d minutes, water reminder set for %.1f minutes.\n", work_time, rest_time, water_interval);

            lv_obj_set_style_bg_color(guider_ui.monitor_work_btn, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.monitor_work_btn_label, "STOP");
            lv_obj_clear_flag(guider_ui.monitor_spinner_1, LV_OBJ_FLAG_HIDDEN);
            work_btn_status = true;
        }
        else
        {
            // 关闭闹钟
            if (work_time_alarm)
                rt_alarm_delete(work_time_alarm); // 删除工作时间闹钟
            if (rest_time_alarm)
                rt_alarm_delete(rest_time_alarm); // 删除休息时间闹钟
            if (water_reminder_alarm)
                rt_alarm_delete(water_reminder_alarm); // 删除喝水提醒闹钟
            // 关
            lv_obj_set_style_bg_color(guider_ui.monitor_work_btn, lv_color_hex(0x8c8d8e), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(guider_ui.monitor_work_btn_label, "START");
            lv_obj_add_flag(guider_ui.monitor_spinner_1, LV_OBJ_FLAG_HIDDEN);
            work_btn_status = false;
        }
        break;
    }
    default:
        break;
    }
}

/*工作时间闹钟回调*/
static void work_time_alarm_callback(rt_alarm_t alarm, time_t timestamp)
{
    work_time_handler();
}

void work_time_handler(void)
{
    rt_kprintf("===================[lvgl]Work time alarm triggered.\n");
    rt_kprintf("[lvgl]Work time: %d minutes, Rest time: %d minutes, Loop count: %d\n", work_time, rest_time, loop_count);
    rt_kprintf("[lvgl]Rest count: %d\n", rest_count);
    // 打印time(NULL)
    time_t now = time(NULL);
    struct tm now_tm;
    gmtime_r(&now, &now_tm);
    rt_kprintf("[lvgl]time(NULL): %02d:%02d:%02d\n", now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec);
    // 打印RTC硬件时间
    struct tm rtc_tm;
    cyhal_rtc_read(&rtc_obj, &rtc_tm);
    rt_kprintf("[lvgl]cyhal_rtc_read: %02d:%02d:%02d\n", rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
    if (rest_count > 0)
    {
        rt_kprintf("[lvgl]Rest count: %d\n", rest_count);
        // 如果还有休息次数，启动休息时间闹钟
        // 休息时间闹钟
        rt_thread_mdelay(1000);
        time_t temp_time;
        temp_time = time(NULL); // 获取当前时间戳
        struct tm p_tm1;
        gmtime_r(&temp_time, &p_tm1); // 获取当前时间

        rt_kprintf("[lvgl]Current time: %02d:%02d:%02d\n", p_tm1.tm_hour, p_tm1.tm_min, p_tm1.tm_sec);

        struct tm p_tm;
        // temp_time += rest_time * 60; // 获取当前时间戳加上休息时间
        temp_time += 6 + 6; // 获取当前时间戳加上0.1分钟
        gmtime_r(&temp_time, &p_tm);
        rt_kprintf("[lvgl]Rest time alarm set for %d minutes at %02d:%02d:%02d.\n", rest_time, p_tm.tm_hour, p_tm.tm_min, p_tm.tm_sec);
        rest_time_alarm_setup.flag = RT_ALARM_ONESHOT; // 单次闹钟
        rest_time_alarm_setup.wktime.tm_hour = p_tm.tm_hour;
        rest_time_alarm_setup.wktime.tm_min = p_tm.tm_min;
        rest_time_alarm_setup.wktime.tm_sec = p_tm.tm_sec;
        rest_time_alarm = rt_alarm_create(rest_time_alarm_callback, &rest_time_alarm_setup);

        if (rt_alarm_start(rest_time_alarm) != RT_EOK) // 启动休息时间闹钟
        {
            rt_kprintf("[lvgl]Failed to start rest time alarm.\n");
            return;
        }
        rt_kprintf("[lvgl]Rest time alarm set for %d minutes.\n", rest_time);
        rest_count--;
    }
    // 这里可以添加工作时间到达后的处理逻辑
}

/*休息时间闹钟回调*/
static void rest_time_alarm_callback(rt_alarm_t alarm, time_t timestamp)
{
    rt_kprintf("=====================[lvgl]Rest time alarm triggered.\n");
    // 打印callback传入的timestamp
    struct tm cb_tm;
    gmtime_r(&timestamp, &cb_tm);
    rt_kprintf("[lvgl]callback timestamp: %02d:%02d:%02d\n", cb_tm.tm_hour, cb_tm.tm_min, cb_tm.tm_sec);
    // 打印time(NULL)
    time_t now = time(NULL);
    struct tm now_tm;
    gmtime_r(&now, &now_tm);
    rt_kprintf("[lvgl]time(NULL): %02d:%02d:%02d\n", now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec);
    // 打印RTC硬件时间
    struct tm rtc_tm;
    cyhal_rtc_read(&rtc_obj, &rtc_tm);
    rt_kprintf("[lvgl]cyhal_rtc_read: %02d:%02d:%02d\n", rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
    if (work_count > 0)
    {
        // 如果还有工作次数，启动工作时间闹钟
        // 工作时间闹钟
        time_t temp_time;
        temp_time = time(NULL); // 获取当前时间戳
        struct tm p_tm;
        temp_time += work_time * 60;                   // 获取当前时间戳加上工作时间
        gmtime_r(&temp_time, &p_tm);                   // 获取当前时间
        work_time_alarm_setup.flag = RT_ALARM_ONESHOT; // 单次闹钟
        work_time_alarm_setup.wktime.tm_hour = p_tm.tm_hour;
        work_time_alarm_setup.wktime.tm_min = p_tm.tm_min;
        work_time_alarm_setup.wktime.tm_sec = p_tm.tm_sec;
        rt_alarm_control(work_time_alarm, RT_ALARM_CTRL_MODIFY, &work_time_alarm_setup); // 修改工作时间闹钟
        rt_alarm_start(work_time_alarm);                                                 // 启动工作时间闹钟
        work_count--;
    }
    // 这里可以添加休息时间到达后的处理逻辑
}

/*喝水提醒闹钟回调*/
static void water_reminder_alarm_callback(rt_alarm_t alarm, time_t timestamp)
{
    // 监测模式喝水提醒间隔时间到达回调函数
    rt_kprintf("=================[lvgl]Water reminder alarm triggered.\n");
    // 这里可以添加喝水提醒逻辑
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
            uart_send_protocol("c", 02); // 发送关闭追踪模式指令
            break;
        case 1:
            // 书本追踪模式
            uart_send_protocol("c", 03); // 发送书本追踪模式指令
            break;
        case 2:
            // 手追踪模式
            uart_send_protocol("c", 04); // 发送手追踪模式指令
            break;
        }
        break;
    }
    }
}

void events_init_settings(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->settings_btn_1, settings_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->settings_ddlist_switch, settings_ddlist_switch_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->settings_ddlist_1, settings_ddlist_1_event_handler, LV_EVENT_ALL, ui);
}

static void guader_btn_1_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.home, guider_ui.home_del, &guider_ui.guader_del, setup_scr_home, LV_SCR_LOAD_ANIM_NONE, 20, 20, true, false);
        uart_send_protocol("c", 11); // 发送守护模式关闭指令
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
