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
#include "gimbal_pid.h"

int settings_digital_clock_1_min_value = 25;
int settings_digital_clock_1_hour_value = 11;
int settings_digital_clock_1_sec_value = 50;
// char settings_digital_clock_1_meridiem[] = "AM";
void setup_scr_settings(lv_ui *ui)
{
    // Write codes settings
    ui->settings = lv_obj_create(NULL);
    lv_obj_set_size(ui->settings, 320, 240);
    lv_obj_set_scrollbar_mode(ui->settings, LV_SCROLLBAR_MODE_OFF);

    // Write style for settings, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->settings, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes settings_cont_1
    ui->settings_cont_1 = lv_obj_create(ui->settings);
    lv_obj_set_pos(ui->settings_cont_1, 0, 0);
    lv_obj_set_size(ui->settings_cont_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->settings_cont_1, LV_SCROLLBAR_MODE_OFF);

    // Write style for settings_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->settings_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_cont_1, lv_color_hex(0xccffe0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_dither_mode(ui->settings_cont_1, LV_DITHER_ORDERED, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_cont_1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->settings_cont_1, lv_color_hex(0x4dffb8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->settings_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->settings_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->settings_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes settings_cont_2
    ui->settings_cont_2 = lv_obj_create(ui->settings);
    lv_obj_set_pos(ui->settings_cont_2, 0, 0);
    lv_obj_set_size(ui->settings_cont_2, 320, 15);
    lv_obj_set_scrollbar_mode(ui->settings_cont_2, LV_SCROLLBAR_MODE_OFF);

    // Write style for settings_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->settings_cont_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->settings_cont_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->settings_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->settings_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_cont_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->settings_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes settings_datetext_1
    ui->settings_datetext_1 = lv_label_create(ui->settings);
    lv_label_set_text(ui->settings_datetext_1, "2025/06/30");
    lv_obj_set_style_text_align(ui->settings_datetext_1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_add_flag(ui->settings_datetext_1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui->settings_datetext_1, settings_datetext_1_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->settings_datetext_1, 0, 0);
    lv_obj_set_size(ui->settings_datetext_1, 100, 15);

    // Write style for settings_datetext_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->settings_datetext_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->settings_datetext_1, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->settings_datetext_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->settings_datetext_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->settings_datetext_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_datetext_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_datetext_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_datetext_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->settings_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_datetext_1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes settings_digital_clock_1
    static bool settings_digital_clock_1_timer_enabled = false;
    ui->settings_digital_clock_1 = lv_dclock_create(ui->settings, "11:25:50");
    if (!settings_digital_clock_1_timer_enabled)
    {
        lv_timer_create(settings_digital_clock_1_timer, 1000, NULL);
        settings_digital_clock_1_timer_enabled = true;
    }
    lv_obj_set_pos(ui->settings_digital_clock_1, 220, 0);
    lv_obj_set_size(ui->settings_digital_clock_1, 100, 15);

    // Write style for settings_digital_clock_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->settings_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->settings_digital_clock_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->settings_digital_clock_1, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->settings_digital_clock_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->settings_digital_clock_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->settings_digital_clock_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_digital_clock_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_digital_clock_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_digital_clock_1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->settings_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes settings_mode_l
    ui->settings_mode_l = lv_label_create(ui->settings);
    lv_label_set_text(ui->settings_mode_l, "台灯开关模式");
    lv_label_set_long_mode(ui->settings_mode_l, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->settings_mode_l, 5, 30);
    lv_obj_set_size(ui->settings_mode_l, 86, 18);

    // Write style for settings_mode_l, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->settings_mode_l, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->settings_mode_l, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->settings_mode_l, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->settings_mode_l, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_mode_l, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes settings_label_wifi
    ui->settings_label_wifi = lv_label_create(ui->settings);
    lv_label_set_text(ui->settings_label_wifi, "");
    lv_label_set_long_mode(ui->settings_label_wifi, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->settings_label_wifi, 95, 0);
    lv_obj_set_size(ui->settings_label_wifi, 20, 15);
    lv_obj_add_flag(ui->settings_label_wifi, LV_OBJ_FLAG_HIDDEN);

    // Write style for settings_label_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->settings_label_wifi, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->settings_label_wifi, &lv_font_iconfont_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->settings_label_wifi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->settings_label_wifi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes settings_btn_1
    ui->settings_btn_1 = lv_btn_create(ui->settings);
    ui->settings_btn_1_label = lv_label_create(ui->settings_btn_1);
    lv_label_set_text(ui->settings_btn_1_label, "");
    lv_label_set_long_mode(ui->settings_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->settings_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->settings_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->settings_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->settings_btn_1, 291, 107);
    lv_obj_set_size(ui->settings_btn_1, 23, 23);

    // Write style for settings_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->settings_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_btn_1, lv_color_hex(0xf9dfff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->settings_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_btn_1, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_btn_1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->settings_btn_1, lv_color_hex(0xb8a0bd), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->settings_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->settings_btn_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->settings_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->settings_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->settings_btn_1, lv_color_hex(0x575757), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->settings_btn_1, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->settings_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->settings_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes settings_ddlist_switch
    ui->settings_ddlist_switch = lv_dropdown_create(ui->settings);
    lv_dropdown_set_options(ui->settings_ddlist_switch, "自动开关模式\n手动模式");
    lv_obj_set_pos(ui->settings_ddlist_switch, 100, 24);
    lv_obj_set_size(ui->settings_ddlist_switch, 130, 30);

    // Write style for settings_ddlist_switch, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->settings_ddlist_switch, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->settings_ddlist_switch, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->settings_ddlist_switch, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->settings_ddlist_switch, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->settings_ddlist_switch, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->settings_ddlist_switch, lv_color_hex(0xe1e6ee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->settings_ddlist_switch, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_ddlist_switch, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_ddlist_switch, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_ddlist_switch, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_ddlist_switch, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_ddlist_switch, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_ddlist_switch, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_ddlist_switch, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_ddlist_switch, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_settings_ddlist_switch_extra_list_selected_checked
    static lv_style_t style_settings_ddlist_switch_extra_list_selected_checked;
    ui_init_style(&style_settings_ddlist_switch_extra_list_selected_checked);

    lv_style_set_border_width(&style_settings_ddlist_switch_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_settings_ddlist_switch_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_settings_ddlist_switch_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_settings_ddlist_switch_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_settings_ddlist_switch_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_settings_ddlist_switch_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_settings_ddlist_switch_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_settings_ddlist_switch_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->settings_ddlist_switch), &style_settings_ddlist_switch_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_settings_ddlist_switch_extra_list_main_default
    static lv_style_t style_settings_ddlist_switch_extra_list_main_default;
    ui_init_style(&style_settings_ddlist_switch_extra_list_main_default);

    lv_style_set_max_height(&style_settings_ddlist_switch_extra_list_main_default, 90);
    lv_style_set_text_color(&style_settings_ddlist_switch_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_settings_ddlist_switch_extra_list_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_settings_ddlist_switch_extra_list_main_default, 255);
    lv_style_set_border_width(&style_settings_ddlist_switch_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_settings_ddlist_switch_extra_list_main_default, 255);
    lv_style_set_border_color(&style_settings_ddlist_switch_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_settings_ddlist_switch_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_settings_ddlist_switch_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_settings_ddlist_switch_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_settings_ddlist_switch_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_settings_ddlist_switch_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->settings_ddlist_switch), &style_settings_ddlist_switch_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_settings_ddlist_switch_extra_list_scrollbar_default
    static lv_style_t style_settings_ddlist_switch_extra_list_scrollbar_default;
    ui_init_style(&style_settings_ddlist_switch_extra_list_scrollbar_default);

    lv_style_set_radius(&style_settings_ddlist_switch_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_settings_ddlist_switch_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_settings_ddlist_switch_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir(&style_settings_ddlist_switch_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->settings_ddlist_switch), &style_settings_ddlist_switch_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes settings_label_4
    ui->settings_label_4 = lv_label_create(ui->settings);
    lv_label_set_text(ui->settings_label_4, "追踪模式");
    lv_label_set_long_mode(ui->settings_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->settings_label_4, 5, 72);
    lv_obj_set_size(ui->settings_label_4, 69, 18);

    // Write style for settings_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->settings_label_4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->settings_label_4, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->settings_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->settings_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes settings_ddlist_1
    ui->settings_ddlist_1 = lv_dropdown_create(ui->settings);
    lv_dropdown_set_options(ui->settings_ddlist_1, "关\n阅读模式(书本)\n检测模式(手)");
    lv_obj_set_pos(ui->settings_ddlist_1, 100, 64);
    lv_obj_set_size(ui->settings_ddlist_1, 130, 30);

    // Write style for settings_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->settings_ddlist_1, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->settings_ddlist_1, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->settings_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->settings_ddlist_1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->settings_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->settings_ddlist_1, lv_color_hex(0xe1e6ee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->settings_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->settings_ddlist_1, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->settings_ddlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->settings_ddlist_1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_ddlist_1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_ddlist_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_ddlist_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_settings_ddlist_1_extra_list_selected_checked
    static lv_style_t style_settings_ddlist_1_extra_list_selected_checked;
    ui_init_style(&style_settings_ddlist_1_extra_list_selected_checked);

    lv_style_set_border_width(&style_settings_ddlist_1_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_settings_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_settings_ddlist_1_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_settings_ddlist_1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_settings_ddlist_1_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_settings_ddlist_1_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_settings_ddlist_1_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_settings_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->settings_ddlist_1), &style_settings_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_settings_ddlist_1_extra_list_main_default
    static lv_style_t style_settings_ddlist_1_extra_list_main_default;
    ui_init_style(&style_settings_ddlist_1_extra_list_main_default);

    lv_style_set_max_height(&style_settings_ddlist_1_extra_list_main_default, 90);
    lv_style_set_text_color(&style_settings_ddlist_1_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_settings_ddlist_1_extra_list_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_settings_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_width(&style_settings_ddlist_1_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_settings_ddlist_1_extra_list_main_default, 255);
    lv_style_set_border_color(&style_settings_ddlist_1_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_settings_ddlist_1_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_settings_ddlist_1_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_settings_ddlist_1_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_settings_ddlist_1_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_settings_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->settings_ddlist_1), &style_settings_ddlist_1_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_settings_ddlist_1_extra_list_scrollbar_default
    static lv_style_t style_settings_ddlist_1_extra_list_scrollbar_default;
    ui_init_style(&style_settings_ddlist_1_extra_list_scrollbar_default);

    lv_style_set_radius(&style_settings_ddlist_1_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_settings_ddlist_1_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_settings_ddlist_1_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir(&style_settings_ddlist_1_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->settings_ddlist_1), &style_settings_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    // Write codes settings_btn_2
    ui->settings_btn_2 = lv_btn_create(ui->settings);
    ui->settings_btn_2_label = lv_label_create(ui->settings_btn_2);
    lv_label_set_text(ui->settings_btn_2_label, "WIFI");
    lv_label_set_long_mode(ui->settings_btn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->settings_btn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->settings_btn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->settings_btn_2_label, LV_PCT(100));
    lv_obj_set_pos(ui->settings_btn_2, 236, 24);
    lv_obj_set_size(ui->settings_btn_2, 80, 30);

    // Write style for settings_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->settings_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_btn_2, lv_color_hex(0x00a8ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->settings_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->settings_btn_2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->settings_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->settings_btn_2, lv_color_hex(0x2999f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->settings_btn_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->settings_btn_2, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_btn_2, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->settings_btn_2, lv_color_hex(0x606162), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->settings_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->settings_btn_2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->settings_btn_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->settings_btn_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->settings_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->settings_btn_2, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->settings_btn_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->settings_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);


    // The custom code of settings.
    //  The custom code of settings.

    // 创建方向控制按钮（用canvas绘制三角形指示）
    // 公共三角形参数
    lv_draw_rect_dsc_t tri_dsc;
    lv_draw_rect_dsc_init(&tri_dsc);
    tri_dsc.bg_color = lv_color_hex(0x4d5cff); // 蓝色
    tri_dsc.bg_opa = LV_OPA_COVER;
    tri_dsc.radius = 0;

    // 按下状态样式
    static lv_style_t style_btn_pressed;
    lv_style_init(&style_btn_pressed);
    lv_style_set_bg_color(&style_btn_pressed, lv_color_hex(0x2d3c99)); // 深蓝
    lv_style_set_bg_opa(&style_btn_pressed, 255);
    lv_style_set_shadow_width(&style_btn_pressed, 8);
    lv_style_set_shadow_color(&style_btn_pressed, lv_color_hex(0x4d5cff));
    lv_style_set_shadow_opa(&style_btn_pressed, 180);

    // 上按钮
    ui->settings_btn_up = lv_btn_create(ui->settings);
    lv_obj_set_size(ui->settings_btn_up, 40, 40);
    lv_obj_set_pos(ui->settings_btn_up, 142, 120);
    lv_obj_set_style_radius(ui->settings_btn_up, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_btn_up, lv_color_hex(0xe1e6ee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_btn_up, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_btn_up, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->settings_btn_up, lv_color_hex(0xb8a0bd), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->settings_btn_up, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->settings_btn_up, &style_btn_pressed, LV_STATE_PRESSED);
    static lv_color_t buf_up[LV_CANVAS_BUF_SIZE_TRUE_COLOR(40, 40)];
    lv_obj_t *canvas_up = lv_canvas_create(ui->settings_btn_up);
    lv_canvas_set_buffer(canvas_up, buf_up, 40, 40, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(canvas_up, lv_color_hex(0xe1e6ee), LV_OPA_COVER);
    lv_point_t points_up[3] = {{20, 6}, {6, 34}, {34, 34}};
    lv_canvas_draw_polygon(canvas_up, points_up, 3, &tri_dsc);
    lv_obj_center(canvas_up);
    lv_obj_add_event_cb(ui->settings_btn_up, direction_btn_canvas_event_cb, LV_EVENT_PRESSED, canvas_up);
    lv_obj_add_event_cb(ui->settings_btn_up, direction_btn_canvas_event_cb, LV_EVENT_RELEASED, canvas_up);
    lv_obj_add_event_cb(ui->settings_btn_up, direction_btn_canvas_event_cb, LV_EVENT_PRESS_LOST, canvas_up);

    // 下按钮
    ui->settings_btn_down = lv_btn_create(ui->settings);
    lv_obj_set_size(ui->settings_btn_down, 40, 40);
    lv_obj_set_pos(ui->settings_btn_down, 140, 195);
    lv_obj_set_style_radius(ui->settings_btn_down, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_btn_down, lv_color_hex(0xe1e6ee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_btn_down, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_btn_down, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->settings_btn_down, lv_color_hex(0xb8a0bd), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->settings_btn_down, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->settings_btn_down, &style_btn_pressed, LV_STATE_PRESSED);
    static lv_color_t buf_down[LV_CANVAS_BUF_SIZE_TRUE_COLOR(40, 40)];
    lv_obj_t *canvas_down = lv_canvas_create(ui->settings_btn_down);
    lv_canvas_set_buffer(canvas_down, buf_down, 40, 40, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(canvas_down, lv_color_hex(0xe1e6ee), LV_OPA_COVER);
    lv_point_t points_down[3] = {{6, 6}, {34, 6}, {20, 34}};
    lv_canvas_draw_polygon(canvas_down, points_down, 3, &tri_dsc);
    lv_obj_center(canvas_down);
    lv_obj_add_event_cb(ui->settings_btn_down, direction_btn_canvas_event_cb, LV_EVENT_PRESSED, canvas_down);
    lv_obj_add_event_cb(ui->settings_btn_down, direction_btn_canvas_event_cb, LV_EVENT_RELEASED, canvas_down);
    lv_obj_add_event_cb(ui->settings_btn_down, direction_btn_canvas_event_cb, LV_EVENT_PRESS_LOST, canvas_down);

    // 左按钮
    ui->settings_btn_left = lv_btn_create(ui->settings);
    lv_obj_set_size(ui->settings_btn_left, 40, 40);
    lv_obj_set_pos(ui->settings_btn_left, 75, 157);
    lv_obj_set_style_radius(ui->settings_btn_left, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_btn_left, lv_color_hex(0xe1e6ee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_btn_left, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_btn_left, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->settings_btn_left, lv_color_hex(0xb8a0bd), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->settings_btn_left, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->settings_btn_left, &style_btn_pressed, LV_STATE_PRESSED);
    static lv_color_t buf_left[LV_CANVAS_BUF_SIZE_TRUE_COLOR(40, 40)];
    lv_obj_t *canvas_left = lv_canvas_create(ui->settings_btn_left);
    lv_canvas_set_buffer(canvas_left, buf_left, 40, 40, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(canvas_left, lv_color_hex(0xe1e6ee), LV_OPA_COVER);
    lv_point_t points_left[3] = {{6, 20}, {34, 6}, {34, 34}};
    lv_canvas_draw_polygon(canvas_left, points_left, 3, &tri_dsc);
    lv_obj_center(canvas_left);
    lv_obj_add_event_cb(ui->settings_btn_left, direction_btn_canvas_event_cb, LV_EVENT_PRESSED, canvas_left);
    lv_obj_add_event_cb(ui->settings_btn_left, direction_btn_canvas_event_cb, LV_EVENT_RELEASED, canvas_left);
    lv_obj_add_event_cb(ui->settings_btn_left, direction_btn_canvas_event_cb, LV_EVENT_PRESS_LOST, canvas_left);

    // 右按钮
    ui->settings_btn_right = lv_btn_create(ui->settings);
    lv_obj_set_size(ui->settings_btn_right, 40, 40);
    lv_obj_set_pos(ui->settings_btn_right, 205, 157);
    lv_obj_set_style_radius(ui->settings_btn_right, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->settings_btn_right, lv_color_hex(0xe1e6ee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->settings_btn_right, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->settings_btn_right, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->settings_btn_right, lv_color_hex(0xb8a0bd), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->settings_btn_right, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->settings_btn_right, &style_btn_pressed, LV_STATE_PRESSED);
    static lv_color_t buf_right[LV_CANVAS_BUF_SIZE_TRUE_COLOR(40, 40)];
    lv_obj_t *canvas_right = lv_canvas_create(ui->settings_btn_right);
    lv_canvas_set_buffer(canvas_right, buf_right, 40, 40, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(canvas_right, lv_color_hex(0xe1e6ee), LV_OPA_COVER);
    lv_point_t points_right[3] = {{34, 20}, {6, 6}, {6, 34}};
    lv_canvas_draw_polygon(canvas_right, points_right, 3, &tri_dsc);
    lv_obj_center(canvas_right);
    lv_obj_add_event_cb(ui->settings_btn_right, direction_btn_canvas_event_cb, LV_EVENT_PRESSED, canvas_right);
    lv_obj_add_event_cb(ui->settings_btn_right, direction_btn_canvas_event_cb, LV_EVENT_RELEASED, canvas_right);
    lv_obj_add_event_cb(ui->settings_btn_right, direction_btn_canvas_event_cb, LV_EVENT_PRESS_LOST, canvas_right);

    // 在四个方向按钮中间添加“云台控制”文字说明
    lv_obj_t *label_ptz = lv_label_create(ui->settings);
    lv_label_set_text(label_ptz, "云台控制");
    lv_obj_set_style_text_color(label_ptz, lv_color_hex(0x2d3c99), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label_ptz, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(label_ptz, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(label_ptz, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(label_ptz, 80, 20);
    lv_obj_set_pos(label_ptz, 130, 170); // 设置坐标，居中于四个按钮之间

    // Update current screen layout.
    lv_obj_update_layout(ui->settings);

    // Init events for screen.
    events_init_settings(ui);
}

// 方向按钮canvas三角形颜色切换事件
static void direction_btn_canvas_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *canvas = lv_event_get_user_data(e);
    lv_draw_rect_dsc_t tri_dsc;
    lv_draw_rect_dsc_init(&tri_dsc);
    // 判断方向
    lv_point_t points[3];
    if (canvas == NULL)
        return;
    // 通过canvas的父对象判断方向
    lv_obj_t *btn = lv_obj_get_parent(canvas);
    if (btn == NULL)
        return;
    // 上
    if (btn == guider_ui.settings_btn_up)
    {
        points[0].x = 20;
        points[0].y = 6;
        points[1].x = 6;
        points[1].y = 34;
        points[2].x = 34;
        points[2].y = 34;
    }
    else if (btn == guider_ui.settings_btn_down)
    {
        points[0].x = 6;
        points[0].y = 6;
        points[1].x = 34;
        points[1].y = 6;
        points[2].x = 20;
        points[2].y = 34;
    }
    else if (btn == guider_ui.settings_btn_left)
    {
        points[0].x = 6;
        points[0].y = 20;
        points[1].x = 34;
        points[1].y = 6;
        points[2].x = 34;
        points[2].y = 34;
    }
    else if (btn == guider_ui.settings_btn_right)
    {
        points[0].x = 34;
        points[0].y = 20;
        points[1].x = 6;
        points[1].y = 6;
        points[2].x = 6;
        points[2].y = 34;
    }
    tri_dsc.bg_opa = LV_OPA_COVER;
    tri_dsc.radius = 0;
    if (code == LV_EVENT_PRESSED)
    {
        tri_dsc.bg_color = lv_color_hex(0x00cfff); // 高亮蓝
        if (btn == guider_ui.settings_btn_up)
        {
            if (sg90_get_angle(gimbal.servo_y) < 95)
                sg90_set_angle(gimbal.servo_y, sg90_get_angle(gimbal.servo_y) + 10);
        }
        else if (btn == guider_ui.settings_btn_down)
        {
            if (sg90_get_angle(gimbal.servo_y) > 50)
                sg90_set_angle(gimbal.servo_y, sg90_get_angle(gimbal.servo_y) - 10);
        }
        else if (btn == guider_ui.settings_btn_right)
        {
            if (sg90_get_angle(gimbal.servo_x) > 30)
                sg90_set_angle(gimbal.servo_x, sg90_get_angle(gimbal.servo_x) - 10);
        }
        else if (btn == guider_ui.settings_btn_left)
        {
            if (sg90_get_angle(gimbal.servo_x) < 150)
                sg90_set_angle(gimbal.servo_x, sg90_get_angle(gimbal.servo_x) + 10);
        }
    }

    else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST)
    {
        tri_dsc.bg_color = lv_color_hex(0x4d5cff); // 默认蓝
    }
    else
    {
        return;
    }
    lv_canvas_fill_bg(canvas, lv_color_hex(0xe1e6ee), LV_OPA_COVER);
    lv_canvas_draw_polygon(canvas, points, 3, &tri_dsc);
}
