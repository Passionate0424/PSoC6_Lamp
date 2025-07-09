/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "widgets_init.h"
#include "events_init.h"

// lvgl互斥锁
rt_mutex_t lv_mutex = RT_NULL;
rt_int32_t drink_count = 0; // 喝水计数

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "gg_external_data.h"
#endif

void ui_init_style(lv_style_t *style)
{
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}

void ui_load_scr_animation(lv_ui *ui, lv_obj_t **new_scr, bool new_scr_del, bool *old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del)
{
    lv_obj_t *act_scr = lv_scr_act();

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
    if (auto_del)
    {
        gg_edata_task_clear(act_scr);
    }
#endif
    if (auto_del && is_clean)
    {
        lv_obj_clean(act_scr);
    }
    if (new_scr_del)
    {
        setup_scr(ui);
    }
    lv_scr_load_anim(*new_scr, anim_type, time, delay, auto_del);
    *old_scr_del = auto_del;
}

void ui_animation(void *var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb)
{
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, var);
    lv_anim_set_exec_cb(&anim, exec_cb);
    lv_anim_set_values(&anim, start_value, end_value);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_delay(&anim, delay);
    lv_anim_set_path_cb(&anim, path_cb);
    lv_anim_set_repeat_count(&anim, repeat_cnt);
    lv_anim_set_repeat_delay(&anim, repeat_delay);
    lv_anim_set_playback_time(&anim, playback_time);
    lv_anim_set_playback_delay(&anim, playback_delay);
    if (start_cb)
    {
        lv_anim_set_start_cb(&anim, start_cb);
    }
    if (ready_cb)
    {
        lv_anim_set_ready_cb(&anim, ready_cb);
    }
    if (deleted_cb)
    {
        lv_anim_set_deleted_cb(&anim, deleted_cb);
    }
    lv_anim_start(&anim);
}

void init_scr_del_flag(lv_ui *ui)
{

    ui->home_del = true;
    ui->monitor_del = true;
    ui->settings_del = true;
    ui->guader_del = true;
    ui->font_del = true;
}

void lv_rest_end_entry(void *parameter)
{
    while (1)
    {
        rt_sem_take(lv_rest_end_sem, RT_WAITING_FOREVER);
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保在修改UI前获取互斥锁
        lv_obj_set_style_bg_color(guider_ui.monitor_work_btn, lv_color_hex(0x8c8d8e), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(guider_ui.monitor_work_btn_label, "START");
        lv_obj_add_flag(guider_ui.monitor_spinner_1, LV_OBJ_FLAG_HIDDEN); // 隐藏工作状态指示器
        rt_mutex_release(lv_mutex);
        // 分数计算线程停止
        monitor_calc_stop();
        // 喝水监测停止
        rt_timer_delete(water_timer);
        water_timer = RT_NULL; // 重置喝水定时器指针
        rt_kprintf("[lvgl]工作和休息循环结束。\n");
    }
}

rt_sem_t lv_rest_end_sem = RT_NULL;
void setup_ui(lv_ui *ui)
{
    lv_mutex = rt_mutex_create("lv_mutex", RT_IPC_FLAG_FIFO);
    if (lv_mutex == RT_NULL)
    {
        rt_kprintf("[gui_guider]Failed to create lv_mutex\n");
        return;
    }
    lv_rest_end_sem = rt_sem_create("lv_rest_end_sem", 0, RT_IPC_FLAG_FIFO);
    if (lv_rest_end_sem == RT_NULL)
    {
        rt_kprintf("[gui_guider]Failed to create lv_rest_end_sem\n");
        rt_mutex_delete(lv_mutex);
        return;
    }
    rt_thread_t lv_rest_end_thread = rt_thread_create("lv_rest_end_thread", lv_rest_end_entry, NULL, 1024, 10, 20);
    if (lv_rest_end_thread == RT_NULL)
    {
        rt_kprintf("[gui_guider]Failed to create lv_rest_end_thread\n");
        rt_sem_delete(lv_rest_end_sem);
        return;
    }
    rt_thread_startup(lv_rest_end_thread);
    init_scr_del_flag(ui);
    init_keyboard(ui);
    setup_scr_home(ui);
    lv_scr_load(ui->home);
}

void init_keyboard(lv_ui *ui)
{
}
