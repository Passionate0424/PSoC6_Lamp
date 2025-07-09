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
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"

int home_digital_clock_1_min_value = 25;
int home_digital_clock_1_hour_value = 11;
int home_digital_clock_1_sec_value = 50;
//char home_digital_clock_1_meridiem[] = "AM";
void setup_scr_home(lv_ui *ui)
{
    // Write codes home
    ui->home = lv_obj_create(NULL);
    lv_obj_set_size(ui->home, 320, 240);
    lv_obj_set_scrollbar_mode(ui->home, LV_SCROLLBAR_MODE_OFF);

    // Write style for home, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_cont_1
    ui->home_cont_1 = lv_obj_create(ui->home);
    lv_obj_set_pos(ui->home_cont_1, 0, 0);
    lv_obj_set_size(ui->home_cont_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->home_cont_1, LV_SCROLLBAR_MODE_OFF);

    // Write style for home_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_cont_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->home_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->home_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->home_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_cont_1, lv_color_hex(0xe7fafd), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_dither_mode(ui->home_cont_1, LV_DITHER_ORDERED, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_cont_1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->home_cont_1, lv_color_hex(0x4dffdf), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->home_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->home_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_digital_clock_1
    static bool home_digital_clock_1_timer_enabled = false;
    ui->home_digital_clock_1 = lv_dclock_create(ui->home, "11:25:50");
    if (!home_digital_clock_1_timer_enabled)
    {
        lv_timer_create(home_digital_clock_1_timer, 1000, NULL);
        home_digital_clock_1_timer_enabled = true;
    }
    lv_obj_set_pos(ui->home_digital_clock_1, 70, 41);
    lv_obj_set_size(ui->home_digital_clock_1, 180, 50);

    // Write style for home_digital_clock_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->home_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_digital_clock_1, lv_color_hex(0x2F35DA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_digital_clock_1, &lv_font_Alatsi_Regular_35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_digital_clock_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->home_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_digital_clock_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_digital_clock_1, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_datetext_1
    ui->home_datetext_1 = lv_label_create(ui->home);
    lv_label_set_text(ui->home_datetext_1, "2025/06/30");
    lv_obj_set_style_text_align(ui->home_datetext_1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_add_flag(ui->home_datetext_1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui->home_datetext_1, home_datetext_1_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->home_datetext_1, 95, 15);
    lv_obj_set_size(ui->home_datetext_1, 130, 36);

    // Write style for home_datetext_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->home_datetext_1, lv_color_hex(0x2F35DA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_datetext_1, &lv_font_Alatsi_Regular_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_datetext_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->home_datetext_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_datetext_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->home_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_datetext_1, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_btn_led
    ui->home_btn_led = lv_btn_create(ui->home);
    ui->home_btn_led_label = lv_label_create(ui->home_btn_led);
    lv_label_set_text(ui->home_btn_led_label, "Off");
    lv_label_set_long_mode(ui->home_btn_led_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->home_btn_led_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->home_btn_led, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->home_btn_led_label, LV_PCT(100));
    lv_obj_set_pos(ui->home_btn_led, 30, 127);
    lv_obj_set_size(ui->home_btn_led, 65, 40);
    lv_obj_add_flag(ui->home_btn_led, LV_OBJ_FLAG_HIDDEN);

    // Write style for home_btn_led, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_btn_led, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_btn_led, lv_color_hex(0x8c8d8e), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_btn_led, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->home_btn_led, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_btn_led, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_btn_led, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->home_btn_led, lv_color_hex(0x4f606b), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->home_btn_led, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->home_btn_led, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->home_btn_led, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->home_btn_led, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_btn_led, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_btn_led, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_btn_led, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_btn_led, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_bright_slider
    ui->home_bright_slider = lv_slider_create(ui->home);
    lv_slider_set_range(ui->home_bright_slider, 0, 100);
    lv_slider_set_mode(ui->home_bright_slider, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->home_bright_slider, 50, LV_ANIM_OFF);
    lv_obj_set_pos(ui->home_bright_slider, 137, 98);
    lv_obj_set_size(ui->home_bright_slider, 160, 8);

    // Write style for home_bright_slider, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_bright_slider, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_bright_slider, lv_color_hex(0x0c6aff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_bright_slider, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->home_bright_slider, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->home_bright_slider, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->home_bright_slider, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_bright_slider, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->home_bright_slider, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_bright_slider, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for home_bright_slider, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
    lv_obj_set_style_shadow_width(ui->home_bright_slider, 0, LV_PART_MAIN | LV_STATE_FOCUSED);

    // Write style for home_bright_slider, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_bright_slider, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_bright_slider, lv_color_hex(0x2195f6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_bright_slider, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_bright_slider, 50, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write style for home_bright_slider, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_bright_slider, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_bright_slider, lv_color_hex(0x2195f6), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_bright_slider, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_bright_slider, 50, LV_PART_KNOB | LV_STATE_DEFAULT);

    // 修改初始未禁用，对应自动亮度
    lv_obj_clear_flag(guider_ui.home_bright_slider, LV_OBJ_FLAG_CLICKABLE);                                                // 禁止点击
    lv_obj_clear_flag(guider_ui.home_bright_slider, LV_OBJ_FLAG_PRESS_LOCK);                                               // 禁止按压/拖动
    lv_obj_set_style_bg_color(guider_ui.home_bright_slider, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);      // 轨道
    lv_obj_set_style_bg_color(guider_ui.home_bright_slider, lv_color_hex(0x808080), LV_PART_INDICATOR | LV_STATE_DEFAULT); // 指示器
    lv_obj_set_style_bg_color(guider_ui.home_bright_slider, lv_color_hex(0x808080), LV_PART_KNOB | LV_STATE_DEFAULT);      // 旋钮

    // Write codes home_sw_2
    ui->home_sw_2 = lv_switch_create(ui->home);
    lv_obj_set_pos(ui->home_sw_2, 78, 91);
    lv_obj_set_size(ui->home_sw_2, 40, 20);

    // Write style for home_sw_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_sw_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_sw_2, lv_color_hex(0xe6e2e6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_sw_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->home_sw_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_sw_2, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_sw_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_state(ui->home_sw_2, LV_STATE_CHECKED);

    // Write style for home_sw_2, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
    lv_obj_set_style_bg_opa(ui->home_sw_2, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(ui->home_sw_2, lv_color_hex(0xe6e2e6), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_grad_dir(ui->home_sw_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(ui->home_sw_2, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_radius(ui->home_sw_2, 10, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui->home_sw_2, 0, LV_PART_MAIN | LV_STATE_FOCUSED);

    // Write style for home_sw_2, Part: LV_PART_MAIN, State: LV_STATE_DISABLED.
    lv_obj_set_style_bg_opa(ui->home_sw_2, 255, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(ui->home_sw_2, lv_color_hex(0xe6e2e6), LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_bg_grad_dir(ui->home_sw_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_border_width(ui->home_sw_2, 0, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_radius(ui->home_sw_2, 10, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_shadow_width(ui->home_sw_2, 0, LV_PART_MAIN | LV_STATE_DISABLED);

    // Write style for home_sw_2, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->home_sw_2, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->home_sw_2, lv_color_hex(0x2195f6), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->home_sw_2, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->home_sw_2, 0, LV_PART_INDICATOR | LV_STATE_CHECKED);

    // Write style for home_sw_2, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_sw_2, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_sw_2, lv_color_hex(0xffffff), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_sw_2, LV_GRAD_DIR_NONE, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->home_sw_2, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_sw_2, 10, LV_PART_KNOB | LV_STATE_DEFAULT);

    // Write codes home_cont_3
    ui->home_cont_3 = lv_obj_create(ui->home);
    lv_obj_set_pos(ui->home_cont_3, 144, 127);
    lv_obj_set_size(ui->home_cont_3, 150, 100);
    lv_obj_set_scrollbar_mode(ui->home_cont_3, LV_SCROLLBAR_MODE_OFF);

    // Write style for home_cont_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_cont_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->home_cont_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->home_cont_3, lv_color_hex(0xbdfffc), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->home_cont_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_cont_3, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_cont_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_cont_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_cont_3, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_cont_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_cont_3, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->home_cont_3, lv_color_hex(0x6e8385), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->home_cont_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->home_cont_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->home_cont_3, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->home_cont_3, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_btn_monitor
    ui->home_btn_monitor = lv_btn_create(ui->home_cont_3);
    ui->home_btn_monitor_label = lv_label_create(ui->home_btn_monitor);
    lv_label_set_text(ui->home_btn_monitor_label, "监测模式");
    lv_label_set_long_mode(ui->home_btn_monitor_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->home_btn_monitor_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->home_btn_monitor, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->home_btn_monitor_label, LV_PCT(100));
    lv_obj_set_pos(ui->home_btn_monitor, 48, 5);
    lv_obj_set_size(ui->home_btn_monitor, 92, 26);

    // Write style for home_btn_monitor, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_btn_monitor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_btn_monitor, lv_color_hex(0x46e6bc), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_btn_monitor, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->home_btn_monitor, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_btn_monitor, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_btn_monitor, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->home_btn_monitor, lv_color_hex(0xc4c4c4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->home_btn_monitor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->home_btn_monitor, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->home_btn_monitor, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->home_btn_monitor, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_btn_monitor, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_btn_monitor, &lv_font_SourceHanSerifSC_Regular_13, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_btn_monitor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_btn_monitor, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_btn_guard
    ui->home_btn_guard = lv_btn_create(ui->home_cont_3);
    ui->home_btn_guard_label = lv_label_create(ui->home_btn_guard);
    lv_label_set_text(ui->home_btn_guard_label, "警戒模式");
    lv_label_set_long_mode(ui->home_btn_guard_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->home_btn_guard_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->home_btn_guard, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->home_btn_guard_label, LV_PCT(100));
    lv_obj_set_pos(ui->home_btn_guard, 48, 67);
    lv_obj_set_size(ui->home_btn_guard, 92, 26);

    // Write style for home_btn_guard, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_btn_guard, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_btn_guard, lv_color_hex(0xbbff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_btn_guard, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->home_btn_guard, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_btn_guard, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_btn_guard, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->home_btn_guard, lv_color_hex(0xc4c4c4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->home_btn_guard, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->home_btn_guard, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->home_btn_guard, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->home_btn_guard, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_btn_guard, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_btn_guard, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_btn_guard, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_btn_guard, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_btn_setting
    ui->home_btn_setting = lv_btn_create(ui->home_cont_3);
    ui->home_btn_setting_label = lv_label_create(ui->home_btn_setting);
    lv_label_set_text(ui->home_btn_setting_label, "设置");
    lv_label_set_long_mode(ui->home_btn_setting_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->home_btn_setting_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->home_btn_setting, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->home_btn_setting_label, LV_PCT(100));
    lv_obj_set_pos(ui->home_btn_setting, 48, 36);
    lv_obj_set_size(ui->home_btn_setting, 92, 26);

    // Write style for home_btn_setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_btn_setting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_btn_setting, lv_color_hex(0x82e249), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_btn_setting, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->home_btn_setting, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_btn_setting, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_btn_setting, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->home_btn_setting, lv_color_hex(0xc4c4c4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->home_btn_setting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->home_btn_setting, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->home_btn_setting, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->home_btn_setting, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_btn_setting, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_btn_setting, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_btn_setting, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_btn_setting, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_label_2
    ui->home_label_2 = lv_label_create(ui->home);
    lv_label_set_text(ui->home_label_2, "自动亮度");
    lv_label_set_long_mode(ui->home_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->home_label_2, 8, 91);
    lv_obj_set_size(ui->home_label_2, 70, 18);

    // Write style for home_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_label_2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_label_2, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_label_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_label_3
    ui->home_label_3 = lv_label_create(ui->home);
    lv_label_set_text(ui->home_label_3, "");
    lv_label_set_long_mode(ui->home_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->home_label_3, 152, 140);
    lv_obj_set_size(ui->home_label_3, 36, 15);

    // Write style for home_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_label_3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_label_3, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_label_3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_label_4
    ui->home_label_4 = lv_label_create(ui->home);
    lv_label_set_text(ui->home_label_4, "");
    lv_label_set_long_mode(ui->home_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->home_label_4, 152, 171);
    lv_obj_set_size(ui->home_label_4, 36, 15);

    // Write style for home_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_label_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_label_4, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_label_5
    ui->home_label_5 = lv_label_create(ui->home);
    lv_label_set_text(ui->home_label_5, "");
    lv_label_set_long_mode(ui->home_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->home_label_5, 152, 201);
    lv_obj_set_size(ui->home_label_5, 36, 15);

    // Write style for home_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_label_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_label_5, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes home_btn_ai
    ui->home_btn_ai = lv_btn_create(ui->home);
    ui->home_btn_ai_label = lv_label_create(ui->home_btn_ai);
    lv_label_set_text(ui->home_btn_ai_label, "按住说话");
    lv_label_set_long_mode(ui->home_btn_ai_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->home_btn_ai_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->home_btn_ai, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->home_btn_ai_label, LV_PCT(100));
    lv_obj_set_pos(ui->home_btn_ai, 30, 185);
    lv_obj_set_size(ui->home_btn_ai, 65, 40);

    // Write style for home_btn_ai, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->home_btn_ai, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->home_btn_ai, lv_color_hex(0x00e0ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->home_btn_ai, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->home_btn_ai, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->home_btn_ai, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->home_btn_ai, lv_color_hex(0x0093ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->home_btn_ai, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->home_btn_ai, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->home_btn_ai, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->home_btn_ai, lv_color_hex(0x3a7ba1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->home_btn_ai, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->home_btn_ai, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->home_btn_ai, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->home_btn_ai, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->home_btn_ai, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->home_btn_ai, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->home_btn_ai, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->home_btn_ai, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // The custom code of home.

    // Update current screen layout.
    lv_obj_update_layout(ui->home);

    // Init events for screen.
    events_init_home(ui);
}
