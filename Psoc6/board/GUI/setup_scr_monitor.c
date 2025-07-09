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

int monitor_digital_clock_1_min_value = 25;
int monitor_digital_clock_1_hour_value = 11;
int monitor_digital_clock_1_sec_value = 50;
//char monitor_digital_clock_1_meridiem[] = "AM";
void setup_scr_monitor(lv_ui *ui)
{
    // Write codes monitor
    ui->monitor = lv_obj_create(NULL);
    lv_obj_set_size(ui->monitor, 320, 240);
    lv_obj_set_scrollbar_mode(ui->monitor, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_cont_1
    ui->monitor_cont_1 = lv_obj_create(ui->monitor);
    lv_obj_set_pos(ui->monitor_cont_1, 0, 0);
    lv_obj_set_size(ui->monitor_cont_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_1, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_1, lv_color_hex(0xccffe0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_tabview_1
    ui->monitor_tabview_1 = lv_tabview_create(ui->monitor, LV_DIR_TOP, 25);
    lv_obj_set_pos(ui->monitor_tabview_1, 0, 15);
    lv_obj_set_size(ui->monitor_tabview_1, 320, 225);
    lv_obj_set_scrollbar_mode(ui->monitor_tabview_1, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_tabview_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_tabview_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_tabview_1, lv_color_hex(0x4dfff3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_dither_mode(ui->monitor_tabview_1, LV_DITHER_ORDERED, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_tabview_1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->monitor_tabview_1, lv_color_hex(0x4d9cff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->monitor_tabview_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->monitor_tabview_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_tabview_1, lv_color_hex(0x4d4d4d), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_tabview_1, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_tabview_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_tabview_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_tabview_1, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_tabview_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_tabview_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_tabview_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_tabview_1_extra_btnm_main_default
    static lv_style_t style_monitor_tabview_1_extra_btnm_main_default;
    ui_init_style(&style_monitor_tabview_1_extra_btnm_main_default);

    lv_style_set_bg_opa(&style_monitor_tabview_1_extra_btnm_main_default, 255);
    lv_style_set_bg_color(&style_monitor_tabview_1_extra_btnm_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_monitor_tabview_1_extra_btnm_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_monitor_tabview_1_extra_btnm_main_default, 0);
    lv_style_set_radius(&style_monitor_tabview_1_extra_btnm_main_default, 0);
    lv_obj_add_style(lv_tabview_get_tab_btns(ui->monitor_tabview_1), &style_monitor_tabview_1_extra_btnm_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_tabview_1_extra_btnm_items_default
    static lv_style_t style_monitor_tabview_1_extra_btnm_items_default;
    ui_init_style(&style_monitor_tabview_1_extra_btnm_items_default);

    lv_style_set_text_color(&style_monitor_tabview_1_extra_btnm_items_default, lv_color_hex(0x4d4d4d));
    lv_style_set_text_font(&style_monitor_tabview_1_extra_btnm_items_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_tabview_1_extra_btnm_items_default, 255);
    lv_obj_add_style(lv_tabview_get_tab_btns(ui->monitor_tabview_1), &style_monitor_tabview_1_extra_btnm_items_default, LV_PART_ITEMS | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_monitor_tabview_1_extra_btnm_items_checked
    static lv_style_t style_monitor_tabview_1_extra_btnm_items_checked;
    ui_init_style(&style_monitor_tabview_1_extra_btnm_items_checked);

    lv_style_set_text_color(&style_monitor_tabview_1_extra_btnm_items_checked, lv_color_hex(0x2195f6));
    lv_style_set_text_font(&style_monitor_tabview_1_extra_btnm_items_checked, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_tabview_1_extra_btnm_items_checked, 255);
    lv_style_set_border_width(&style_monitor_tabview_1_extra_btnm_items_checked, 4);
    lv_style_set_border_opa(&style_monitor_tabview_1_extra_btnm_items_checked, 255);
    lv_style_set_border_color(&style_monitor_tabview_1_extra_btnm_items_checked, lv_color_hex(0x2195f6));
    lv_style_set_border_side(&style_monitor_tabview_1_extra_btnm_items_checked, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_radius(&style_monitor_tabview_1_extra_btnm_items_checked, 0);
    lv_style_set_bg_opa(&style_monitor_tabview_1_extra_btnm_items_checked, 60);
    lv_style_set_bg_color(&style_monitor_tabview_1_extra_btnm_items_checked, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_monitor_tabview_1_extra_btnm_items_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_tabview_get_tab_btns(ui->monitor_tabview_1), &style_monitor_tabview_1_extra_btnm_items_checked, LV_PART_ITEMS | LV_STATE_CHECKED);

    // Write codes 传感器数据
    ui->monitor_tabview_1_tab_1 = lv_tabview_add_tab(ui->monitor_tabview_1, "传感器数据");
    lv_obj_t *monitor_tabview_1_tab_1_label = lv_label_create(ui->monitor_tabview_1_tab_1);
    lv_label_set_text(monitor_tabview_1_tab_1_label, "");

    // Write codes monitor_cont_data_now
    ui->monitor_cont_data_now = lv_obj_create(ui->monitor_tabview_1_tab_1);
    lv_obj_set_pos(ui->monitor_cont_data_now, 24, -5);
    lv_obj_set_size(ui->monitor_cont_data_now, 98, 175);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_data_now, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_data_now, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_data_now, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_data_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_data_now, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_data_now, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_data_now, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_data_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_data_now, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_data_now, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_data_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_data_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_data_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_data_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_data_now, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_cont_data_now, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_cont_data_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_cont_data_now, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_cont_data_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_cont_data_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_breath_now
    ui->monitor_label_breath_now = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_label_breath_now, "---");
    lv_label_set_long_mode(ui->monitor_label_breath_now, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_breath_now, 28, 124);
    lv_obj_set_size(ui->monitor_label_breath_now, 68, 16);

    // Write style for monitor_label_breath_now, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_breath_now, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_breath_now, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_breath_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_breath_now, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_breath_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_temperature
    ui->monitor_temperature = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_temperature, "");
    lv_label_set_long_mode(ui->monitor_temperature, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_temperature, 3, 26);
    lv_obj_set_size(ui->monitor_temperature, 26, 15);
    lv_obj_add_flag(ui->monitor_temperature, LV_OBJ_FLAG_CLICKABLE);

    // Write style for monitor_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_temperature, lv_color_hex(0xff6500), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_temperature, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_temperature, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_temperature, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_co2
    ui->monitor_co2 = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_co2, "");
    lv_label_set_long_mode(ui->monitor_co2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_co2, 3, 84);
    lv_obj_set_size(ui->monitor_co2, 26, 15);

    // Write style for monitor_co2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_co2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_co2, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_co2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_co2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_co2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_4
    ui->monitor_label_4 = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_label_4, "当前");
    lv_label_set_long_mode(ui->monitor_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_4, 6, 5);
    lv_obj_set_size(ui->monitor_label_4, 80, 16);

    // Write style for monitor_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_4, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_4, lv_color_hex(0x2900ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_4, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_4, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_label_4, lv_color_hex(0xb400ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_label_4, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_humidity
    ui->monitor_humidity = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_humidity, "");
    lv_label_set_long_mode(ui->monitor_humidity, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_humidity, 3, 44);
    lv_obj_set_size(ui->monitor_humidity, 26, 15);
    lv_obj_add_flag(ui->monitor_humidity, LV_OBJ_FLAG_CLICKABLE);

    // Write style for monitor_humidity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_humidity, lv_color_hex(0x0093ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_humidity, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_humidity, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_humidity, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_pressure
    ui->monitor_pressure = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_pressure, "");
    lv_label_set_long_mode(ui->monitor_pressure, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_pressure, 3, 63);
    lv_obj_set_size(ui->monitor_pressure, 26, 15);

    // Write style for monitor_pressure, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_pressure, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_pressure, &lv_font_iconfont_15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_pressure, 203, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_pressure, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_heart
    ui->monitor_heart = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_heart, "");
    lv_label_set_long_mode(ui->monitor_heart, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_heart, 3, 145);
    lv_obj_set_size(ui->monitor_heart, 26, 15);
    lv_obj_add_flag(ui->monitor_heart, LV_OBJ_FLAG_CLICKABLE);

    // Write style for monitor_heart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_heart, lv_color_hex(0xf00000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_heart, &lv_font_iconfont_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_heart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_heart, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_tvoc
    ui->monitor_tvoc = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_tvoc, "");
    lv_label_set_long_mode(ui->monitor_tvoc, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_tvoc, 3, 105);
    lv_obj_set_size(ui->monitor_tvoc, 26, 15);

    // Write style for monitor_tvoc, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_tvoc, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_tvoc, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_tvoc, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_tvoc, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_tvoc, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_breath
    ui->monitor_breath = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_breath, "");
    lv_label_set_long_mode(ui->monitor_breath, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_breath, 3, 125);
    lv_obj_set_size(ui->monitor_breath, 26, 16);
    lv_obj_add_flag(ui->monitor_breath, LV_OBJ_FLAG_CLICKABLE);

    // Write style for monitor_breath, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_breath, lv_color_hex(0xff3c69), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_breath, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_breath, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_breath, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_humidity_now
    ui->monitor_label_humidity_now = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_label_humidity_now, "---");
    lv_label_set_long_mode(ui->monitor_label_humidity_now, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_humidity_now, 28, 45);
    lv_obj_set_size(ui->monitor_label_humidity_now, 68, 16);

    // Write style for monitor_label_humidity_now, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_humidity_now, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_humidity_now, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_humidity_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_humidity_now, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_humidity_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_temperature_now
    ui->monitor_label_temperature_now = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_label_temperature_now, "---");
    lv_label_set_long_mode(ui->monitor_label_temperature_now, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_temperature_now, 28, 27);
    lv_obj_set_size(ui->monitor_label_temperature_now, 68, 16);

    // Write style for monitor_label_temperature_now, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_temperature_now, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_temperature_now, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_temperature_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_temperature_now, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_temperature_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_pressure_now
    ui->monitor_label_pressure_now = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_label_pressure_now, "---");
    lv_label_set_long_mode(ui->monitor_label_pressure_now, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_pressure_now, 28, 64);
    lv_obj_set_size(ui->monitor_label_pressure_now, 68, 13);

    // Write style for monitor_label_pressure_now, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_pressure_now, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_pressure_now, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_pressure_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_pressure_now, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_pressure_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_co2_now
    ui->monitor_label_co2_now = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_label_co2_now, "---");
    lv_label_set_long_mode(ui->monitor_label_co2_now, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_co2_now, 28, 84);
    lv_obj_set_size(ui->monitor_label_co2_now, 68, 16);

    // Write style for monitor_label_co2_now, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_co2_now, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_co2_now, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_co2_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_co2_now, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_co2_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_tvoc_now
    ui->monitor_label_tvoc_now = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_label_tvoc_now, "---");
    lv_label_set_long_mode(ui->monitor_label_tvoc_now, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_tvoc_now, 28, 104);
    lv_obj_set_size(ui->monitor_label_tvoc_now, 68, 16);

    // Write style for monitor_label_tvoc_now, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_tvoc_now, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_tvoc_now, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_tvoc_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_tvoc_now, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_tvoc_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_heart_now
    ui->monitor_label_heart_now = lv_label_create(ui->monitor_cont_data_now);
    lv_label_set_text(ui->monitor_label_heart_now, "---");
    lv_label_set_long_mode(ui->monitor_label_heart_now, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_heart_now, 28, 145);
    lv_obj_set_size(ui->monitor_label_heart_now, 68, 16);

    // Write style for monitor_label_heart_now, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_heart_now, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_heart_now, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_heart_now, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_heart_now, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_heart_now, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_cont_score
    ui->monitor_cont_score = lv_obj_create(ui->monitor_tabview_1_tab_1);
    lv_obj_set_pos(ui->monitor_cont_score, 130, 77);
    lv_obj_set_size(ui->monitor_cont_score, 92, 90);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_score, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_score, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_score, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_score, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_score, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_score, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_score, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_score, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_score, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_cont_score, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_cont_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_cont_score, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_cont_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_cont_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_28
    ui->monitor_label_28 = lv_label_create(ui->monitor_cont_score);
    lv_label_set_text(ui->monitor_label_28, "环境评分");
    lv_label_set_long_mode(ui->monitor_label_28, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_28, 6, 26);
    lv_obj_set_size(ui->monitor_label_28, 26, 26);

    // Write style for monitor_label_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_28, lv_color_hex(0xff6500), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_28, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_28, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_28, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_27
    ui->monitor_label_27 = lv_label_create(ui->monitor_cont_score);
    lv_label_set_text(ui->monitor_label_27, "评分");
    lv_label_set_long_mode(ui->monitor_label_27, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_27, 5, 5);
    lv_obj_set_size(ui->monitor_label_27, 80, 16);

    // Write style for monitor_label_27, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_27, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_27, lv_color_hex(0x2900ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_27, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_27, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_27, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_27, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_label_27, lv_color_hex(0xb400ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_label_27, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_27, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_26
    ui->monitor_label_26 = lv_label_create(ui->monitor_cont_score);
    lv_label_set_text(ui->monitor_label_26, "压力状态");
    lv_label_set_long_mode(ui->monitor_label_26, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_26, 6, 55);
    lv_obj_set_size(ui->monitor_label_26, 26, 24);

    // Write style for monitor_label_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_26, lv_color_hex(0x0093ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_26, &lv_font_SourceHanSerifSC_Regular_11, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_26, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_26, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_stress_score
    ui->monitor_label_stress_score = lv_label_create(ui->monitor_cont_score);
    lv_label_set_text(ui->monitor_label_stress_score, "---");
    lv_label_set_long_mode(ui->monitor_label_stress_score, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_stress_score, 35, 58);
    lv_obj_set_size(ui->monitor_label_stress_score, 52, 16);

    // Write style for monitor_label_stress_score, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_stress_score, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_stress_score, &lv_font_montserratMedium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_stress_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_stress_score, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_stress_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_env_score
    ui->monitor_label_env_score = lv_label_create(ui->monitor_cont_score);
    lv_label_set_text(ui->monitor_label_env_score, "---");
    lv_label_set_long_mode(ui->monitor_label_env_score, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_env_score, 35, 31);
    lv_obj_set_size(ui->monitor_label_env_score, 52, 16);

    // Write style for monitor_label_env_score, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_env_score, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_env_score, &lv_font_montserratMedium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_env_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_env_score, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_env_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_cont_pre
    ui->monitor_cont_pre = lv_obj_create(ui->monitor_tabview_1_tab_1);
    lv_obj_set_pos(ui->monitor_cont_pre, 130, -3);
    lv_obj_set_size(ui->monitor_cont_pre, 92, 71);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_pre, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_pre, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_pre, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_pre, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_pre, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_pre, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_pre, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_pre, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_pre, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_pre, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_pre, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_cont_pre, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_cont_pre, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_cont_pre, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_cont_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_cont_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_23
    ui->monitor_label_23 = lv_label_create(ui->monitor_cont_pre);
    lv_label_set_text(ui->monitor_label_23, "");
    lv_label_set_long_mode(ui->monitor_label_23, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_23, 3, 26);
    lv_obj_set_size(ui->monitor_label_23, 26, 15);

    // Write style for monitor_label_23, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_23, lv_color_hex(0xff6500), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_23, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_23, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_21
    ui->monitor_label_21 = lv_label_create(ui->monitor_cont_pre);
    lv_label_set_text(ui->monitor_label_21, "预测");
    lv_label_set_long_mode(ui->monitor_label_21, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_21, 5, 5);
    lv_obj_set_size(ui->monitor_label_21, 80, 16);

    // Write style for monitor_label_21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_21, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_21, lv_color_hex(0x2900ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_21, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_21, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_21, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_21, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_label_21, lv_color_hex(0xb400ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_label_21, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_21, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_20
    ui->monitor_label_20 = lv_label_create(ui->monitor_cont_pre);
    lv_label_set_text(ui->monitor_label_20, "");
    lv_label_set_long_mode(ui->monitor_label_20, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_20, 3, 44);
    lv_obj_set_size(ui->monitor_label_20, 26, 15);

    // Write style for monitor_label_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_20, lv_color_hex(0x0093ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_20, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_20, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_20, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_humidity_pre
    ui->monitor_label_humidity_pre = lv_label_create(ui->monitor_cont_pre);
    lv_label_set_text(ui->monitor_label_humidity_pre, "---");
    lv_label_set_long_mode(ui->monitor_label_humidity_pre, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_humidity_pre, 25, 46);
    lv_obj_set_size(ui->monitor_label_humidity_pre, 60, 16);

    // Write style for monitor_label_humidity_pre, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_humidity_pre, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_humidity_pre, &lv_font_montserratMedium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_humidity_pre, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_humidity_pre, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_humidity_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_temperature_pre
    ui->monitor_label_temperature_pre = lv_label_create(ui->monitor_cont_pre);
    lv_label_set_text(ui->monitor_label_temperature_pre, "---");
    lv_label_set_long_mode(ui->monitor_label_temperature_pre, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_temperature_pre, 25, 26);
    lv_obj_set_size(ui->monitor_label_temperature_pre, 60, 16);

    // Write style for monitor_label_temperature_pre, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_temperature_pre, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_temperature_pre, &lv_font_montserratMedium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_temperature_pre, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_temperature_pre, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_temperature_pre, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_btn_env_advice
    ui->monitor_btn_env_advice = lv_btn_create(ui->monitor_tabview_1_tab_1);
    ui->monitor_btn_env_advice_label = lv_label_create(ui->monitor_btn_env_advice);
    lv_label_set_text(ui->monitor_btn_env_advice_label, "环境\n建议");
    lv_label_set_long_mode(ui->monitor_btn_env_advice_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->monitor_btn_env_advice_label, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(ui->monitor_btn_env_advice, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->monitor_btn_env_advice_label, LV_PCT(100));
    lv_obj_set_pos(ui->monitor_btn_env_advice, 243, 33);
    lv_obj_set_size(ui->monitor_btn_env_advice, 40, 80);
    lv_obj_add_flag(ui->monitor_btn_env_advice, LV_OBJ_FLAG_CLICKABLE);

    // Write style for monitor_btn_env_advice, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_btn_env_advice, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_btn_env_advice, lv_color_hex(0x9800ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_btn_env_advice, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_btn_env_advice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_btn_env_advice, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_btn_env_advice, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_btn_env_advice, lv_color_hex(0x4f606b), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_btn_env_advice, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_btn_env_advice, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_btn_env_advice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_btn_env_advice, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_btn_env_advice, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_btn_env_advice, &lv_font_SourceHanSerifSC_Regular_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_btn_env_advice, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_btn_env_advice, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for monitor_btn_env_advice, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->monitor_btn_env_advice, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->monitor_btn_env_advice, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->monitor_btn_env_advice, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->monitor_btn_env_advice, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_radius(ui->monitor_btn_env_advice, 5, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->monitor_btn_env_advice, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->monitor_btn_env_advice, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->monitor_btn_env_advice, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->monitor_btn_env_advice, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    // Write codes 工作状态
    ui->monitor_tabview_1_tab_2 = lv_tabview_add_tab(ui->monitor_tabview_1, "工作状态");
    lv_obj_t *monitor_tabview_1_tab_2_label = lv_label_create(ui->monitor_tabview_1_tab_2);
    lv_label_set_text(monitor_tabview_1_tab_2_label, "");

    // Write codes monitor_bar_energy
    ui->monitor_bar_energy = lv_bar_create(ui->monitor_tabview_1_tab_2);
    lv_obj_set_style_anim_time(ui->monitor_bar_energy, 1000, 0);
    lv_bar_set_mode(ui->monitor_bar_energy, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->monitor_bar_energy, 0, 100);
    lv_bar_set_value(ui->monitor_bar_energy, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->monitor_bar_energy, 66, 5);
    lv_obj_set_size(ui->monitor_bar_energy, 181, 15);

    // Write style for monitor_bar_energy, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_bar_energy, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_bar_energy, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_bar_energy, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->monitor_bar_energy, lv_color_hex(0x920ee3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->monitor_bar_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->monitor_bar_energy, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_bar_energy, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_bar_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for monitor_bar_energy, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_bar_energy, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_bar_energy, lv_color_hex(0x2195f6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_bar_energy, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_bar_energy, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write codes monitor_label_energy
    ui->monitor_label_energy = lv_label_create(ui->monitor_tabview_1_tab_2);
    lv_label_set_text(ui->monitor_label_energy, "100%");
    lv_label_set_long_mode(ui->monitor_label_energy, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_energy, 254, 5);
    lv_obj_set_size(ui->monitor_label_energy, 38, 18);

    // Write style for monitor_label_energy, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_energy, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_energy, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_energy, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_energy, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_energy, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_carefulness
    ui->monitor_label_carefulness = lv_label_create(ui->monitor_tabview_1_tab_2);
    lv_label_set_text(ui->monitor_label_carefulness, "100%");
    lv_label_set_long_mode(ui->monitor_label_carefulness, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_carefulness, 254, 46);
    lv_obj_set_size(ui->monitor_label_carefulness, 37, 18);

    // Write style for monitor_label_carefulness, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_carefulness, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_carefulness, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_carefulness, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_carefulness, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_bar_carefulness
    ui->monitor_bar_carefulness = lv_bar_create(ui->monitor_tabview_1_tab_2);
    lv_obj_set_style_anim_time(ui->monitor_bar_carefulness, 1000, 0);
    lv_bar_set_mode(ui->monitor_bar_carefulness, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->monitor_bar_carefulness, 0, 100);
    lv_bar_set_value(ui->monitor_bar_carefulness, 100, LV_ANIM_OFF);
    lv_obj_set_pos(ui->monitor_bar_carefulness, 66, 46);
    lv_obj_set_size(ui->monitor_bar_carefulness, 181, 15);

    // Write style for monitor_bar_carefulness, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_bar_carefulness, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_bar_carefulness, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_bar_carefulness, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->monitor_bar_carefulness, lv_color_hex(0x920ee3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->monitor_bar_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->monitor_bar_carefulness, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_bar_carefulness, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_bar_carefulness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for monitor_bar_carefulness, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_bar_carefulness, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_bar_carefulness, lv_color_hex(0x2195f6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_bar_carefulness, LV_GRAD_DIR_NONE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_bar_carefulness, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write codes monitor_cont_5
    ui->monitor_cont_5 = lv_obj_create(ui->monitor_tabview_1_tab_2);
    lv_obj_set_pos(ui->monitor_cont_5, -11, 36);
    lv_obj_set_size(ui->monitor_cont_5, 65, 25);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_5, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_5, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_5, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_5, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_5, lv_color_hex(0x00ffcb), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_5, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_5, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_cont_5, lv_color_hex(0x4e837b), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_cont_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_cont_5, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_cont_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_7
    ui->monitor_label_7 = lv_label_create(ui->monitor_cont_5);
    lv_label_set_text(ui->monitor_label_7, "专注度");
    lv_label_set_long_mode(ui->monitor_label_7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_7, 5, 3);
    lv_obj_set_size(ui->monitor_label_7, 50, 16);

    // Write style for monitor_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_7, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_cont_6
    ui->monitor_cont_6 = lv_obj_create(ui->monitor_tabview_1_tab_2);
    lv_obj_set_pos(ui->monitor_cont_6, -11, -2);
    lv_obj_set_size(ui->monitor_cont_6, 65, 25);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_6, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_6, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_6, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_6, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_6, lv_color_hex(0x00ffcb), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_6, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_cont_6, lv_color_hex(0x4e837b), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_cont_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_cont_6, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_cont_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_29
    ui->monitor_label_29 = lv_label_create(ui->monitor_cont_6);
    lv_label_set_text(ui->monitor_label_29, "能量值");
    lv_label_set_long_mode(ui->monitor_label_29, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_29, 7, 3);
    lv_obj_set_size(ui->monitor_label_29, 48, 16);

    // Write style for monitor_label_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_29, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_29, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_29, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_cont_work_score
    ui->monitor_cont_work_score = lv_obj_create(ui->monitor_tabview_1_tab_2);
    lv_obj_set_pos(ui->monitor_cont_work_score, 28, 95);
    lv_obj_set_size(ui->monitor_cont_work_score, 119, 70);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_work_score, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_work_score, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_work_score, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_work_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_work_score, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_work_score, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_work_score, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_work_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_work_score, lv_color_hex(0x00d2ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_work_score, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_work_score, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_cont_work_score, lv_color_hex(0x587483), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_cont_work_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_cont_work_score, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_cont_work_score, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_cont_work_score, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_cont_8
    ui->monitor_cont_8 = lv_obj_create(ui->monitor_cont_work_score);
    lv_obj_set_pos(ui->monitor_cont_8, 9, 0);
    lv_obj_set_size(ui->monitor_cont_8, 100, 24);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_8, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_8, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_8, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_8, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_8, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_8, lv_color_hex(0xadff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_8, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_8, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_30
    ui->monitor_label_30 = lv_label_create(ui->monitor_cont_8);
    lv_label_set_text(ui->monitor_label_30, "工作状态评分");
    lv_label_set_long_mode(ui->monitor_label_30, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_30, 2, 1);
    lv_obj_set_size(ui->monitor_label_30, 91, 19);

    // Write style for monitor_label_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_30, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_30, &lv_font_SourceHanSerifSC_Regular_15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_30, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_30, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_work_score
    ui->monitor_label_work_score = lv_label_create(ui->monitor_cont_work_score);
    lv_label_set_text(ui->monitor_label_work_score, "---");
    lv_label_set_long_mode(ui->monitor_label_work_score, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_work_score, 34, 36);
    lv_obj_set_size(ui->monitor_label_work_score, 56, 20);

    // Write style for monitor_label_work_score, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_work_score, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_work_score, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_work_score, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_work_score, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_work_score, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_cont_drink
    ui->monitor_cont_drink = lv_obj_create(ui->monitor_tabview_1_tab_2);
    lv_obj_set_pos(ui->monitor_cont_drink, 162, 95);
    lv_obj_set_size(ui->monitor_cont_drink, 127, 70);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_drink, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_drink, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_drink, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_drink, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_drink, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_drink, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_drink, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_drink, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_drink, lv_color_hex(0x00afff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_drink, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_drink, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_cont_drink, lv_color_hex(0x587483), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_cont_drink, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_cont_drink, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_cont_drink, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_cont_drink, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_cont_10
    ui->monitor_cont_10 = lv_obj_create(ui->monitor_cont_drink);
    lv_obj_set_pos(ui->monitor_cont_10, 12, 0);
    lv_obj_set_size(ui->monitor_cont_10, 100, 24);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_10, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_10, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_10, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_10, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_10, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_10, lv_color_hex(0xadff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_10, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_32
    ui->monitor_label_32 = lv_label_create(ui->monitor_cont_10);
    lv_label_set_text(ui->monitor_label_32, "喝水次数");
    lv_label_set_long_mode(ui->monitor_label_32, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_32, 2, 1);
    lv_obj_set_size(ui->monitor_label_32, 91, 19);

    // Write style for monitor_label_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_32, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_32, &lv_font_SourceHanSerifSC_Regular_15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_32, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_32, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_drink
    ui->monitor_label_drink = lv_label_create(ui->monitor_cont_drink);
    lv_label_set_text(ui->monitor_label_drink, "---");
    lv_label_set_long_mode(ui->monitor_label_drink, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_drink, 34, 36);
    lv_obj_set_size(ui->monitor_label_drink, 56, 20);

    // Write style for monitor_label_drink, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_drink, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_drink, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_drink, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_drink, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_drink, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_33
    ui->monitor_label_33 = lv_label_create(ui->monitor_tabview_1_tab_2);
    lv_label_set_text(ui->monitor_label_33, "已工作：");
    lv_label_set_long_mode(ui->monitor_label_33, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_33, -2, 69);
    lv_obj_set_size(ui->monitor_label_33, 64, 16);

    // Write style for monitor_label_33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_33, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_33, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_33, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_33, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_33, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_36
    ui->monitor_label_36 = lv_label_create(ui->monitor_tabview_1_tab_2);
    lv_label_set_text(ui->monitor_label_36, "使用手机：");
    lv_label_set_long_mode(ui->monitor_label_36, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_36, 136, 69);
    lv_obj_set_size(ui->monitor_label_36, 73, 16);

    // Write style for monitor_label_36, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_36, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_36, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_36, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_36, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_36, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_phone
    ui->monitor_label_phone = lv_label_create(ui->monitor_tabview_1_tab_2);
    lv_label_set_text(ui->monitor_label_phone, "---");
    lv_label_set_long_mode(ui->monitor_label_phone, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_phone, 202, 69);
    lv_obj_set_size(ui->monitor_label_phone, 43, 16);

    // Write style for monitor_label_phone, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_phone, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_label_phone, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_label_phone, lv_color_hex(0xcfff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_label_phone, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_phone, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_phone, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_phone, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_phone, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_phone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_phone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_phone, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_phone, 213, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_label_phone, lv_color_hex(0xf3ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_dither_mode(ui->monitor_label_phone, LV_DITHER_ORDERED, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_label_phone, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->monitor_label_phone, lv_color_hex(0x7cff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->monitor_label_phone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->monitor_label_phone, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_phone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_phone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_phone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_phone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_phone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_work_duration
    ui->monitor_label_work_duration = lv_label_create(ui->monitor_tabview_1_tab_2);
    lv_label_set_text(ui->monitor_label_work_duration, "---");
    lv_label_set_long_mode(ui->monitor_label_work_duration, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_work_duration, 53, 69);
    lv_obj_set_size(ui->monitor_label_work_duration, 43, 16);

    // Write style for monitor_label_work_duration, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_work_duration, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_label_work_duration, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_label_work_duration, lv_color_hex(0xcfff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_label_work_duration, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_work_duration, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_work_duration, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_work_duration, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_work_duration, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_work_duration, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_work_duration, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_work_duration, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_work_duration, 213, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_label_work_duration, lv_color_hex(0xf3ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_dither_mode(ui->monitor_label_work_duration, LV_DITHER_ORDERED, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_label_work_duration, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->monitor_label_work_duration, lv_color_hex(0x7cff00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->monitor_label_work_duration, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->monitor_label_work_duration, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_work_duration, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_work_duration, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_work_duration, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_work_duration, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_work_duration, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_35
    ui->monitor_label_35 = lv_label_create(ui->monitor_tabview_1_tab_2);
    lv_label_set_text(ui->monitor_label_35, "分钟");
    lv_label_set_long_mode(ui->monitor_label_35, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_35, 97, 69);
    lv_obj_set_size(ui->monitor_label_35, 33, 15);

    // Write style for monitor_label_35, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_35, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_35, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_35, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_35, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_38
    ui->monitor_label_38 = lv_label_create(ui->monitor_tabview_1_tab_2);
    lv_label_set_text(ui->monitor_label_38, "分钟");
    lv_label_set_long_mode(ui->monitor_label_38, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_38, 245, 69);
    lv_obj_set_size(ui->monitor_label_38, 33, 15);

    // Write style for monitor_label_38, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_38, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_38, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_38, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_38, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_38, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes 设置
    ui->monitor_tabview_1_tab_3 = lv_tabview_add_tab(ui->monitor_tabview_1, "设置");
    lv_obj_t *monitor_tabview_1_tab_3_label = lv_label_create(ui->monitor_tabview_1_tab_3);
    lv_label_set_text(monitor_tabview_1_tab_3_label, "");

    // Write codes monitor_work_btn
    ui->monitor_work_btn = lv_btn_create(ui->monitor_tabview_1_tab_3);
    ui->monitor_work_btn_label = lv_label_create(ui->monitor_work_btn);
    lv_label_set_text(ui->monitor_work_btn_label, "Start");
    lv_label_set_long_mode(ui->monitor_work_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->monitor_work_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->monitor_work_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->monitor_work_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->monitor_work_btn, 48, 108);
    lv_obj_set_size(ui->monitor_work_btn, 72, 50);

    // Write style for monitor_work_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_work_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_work_btn, lv_color_hex(0x8c8d8e), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_work_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_work_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_work_btn, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_work_btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_work_btn, lv_color_hex(0x546471), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_work_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_work_btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_work_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_work_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_work_btn, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_work_btn, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_work_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_work_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_ddlist_work_time
    ui->monitor_ddlist_work_time = lv_dropdown_create(ui->monitor_tabview_1_tab_3);
    lv_dropdown_set_options(ui->monitor_ddlist_work_time, "30min\n1h\n1.5h\n2h\n2min");
    lv_obj_set_pos(ui->monitor_ddlist_work_time, 65, -9);
    lv_obj_set_size(ui->monitor_ddlist_work_time, 82, 30);

    // Write style for monitor_ddlist_work_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->monitor_ddlist_work_time, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_ddlist_work_time, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_ddlist_work_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_ddlist_work_time, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_ddlist_work_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_ddlist_work_time, lv_color_hex(0x4da0ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_ddlist_work_time, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_ddlist_work_time, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_ddlist_work_time, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_ddlist_work_time, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_ddlist_work_time, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_ddlist_work_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_ddlist_work_time, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_ddlist_work_time, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_ddlist_work_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_monitor_ddlist_work_time_extra_list_selected_checked
    static lv_style_t style_monitor_ddlist_work_time_extra_list_selected_checked;
    ui_init_style(&style_monitor_ddlist_work_time_extra_list_selected_checked);

    lv_style_set_border_width(&style_monitor_ddlist_work_time_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_monitor_ddlist_work_time_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_monitor_ddlist_work_time_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_monitor_ddlist_work_time_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_monitor_ddlist_work_time_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_work_time_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_work_time_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_work_time_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_work_time), &style_monitor_ddlist_work_time_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_ddlist_work_time_extra_list_main_default
    static lv_style_t style_monitor_ddlist_work_time_extra_list_main_default;
    ui_init_style(&style_monitor_ddlist_work_time_extra_list_main_default);

    lv_style_set_max_height(&style_monitor_ddlist_work_time_extra_list_main_default, 90);
    lv_style_set_text_color(&style_monitor_ddlist_work_time_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_monitor_ddlist_work_time_extra_list_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_ddlist_work_time_extra_list_main_default, 255);
    lv_style_set_border_width(&style_monitor_ddlist_work_time_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_monitor_ddlist_work_time_extra_list_main_default, 255);
    lv_style_set_border_color(&style_monitor_ddlist_work_time_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_monitor_ddlist_work_time_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_monitor_ddlist_work_time_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_work_time_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_work_time_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_work_time_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_work_time), &style_monitor_ddlist_work_time_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_ddlist_work_time_extra_list_scrollbar_default
    static lv_style_t style_monitor_ddlist_work_time_extra_list_scrollbar_default;
    ui_init_style(&style_monitor_ddlist_work_time_extra_list_scrollbar_default);

    lv_style_set_radius(&style_monitor_ddlist_work_time_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_work_time_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_work_time_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_work_time_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_work_time), &style_monitor_ddlist_work_time_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes monitor_label_5
    ui->monitor_label_5 = lv_label_create(ui->monitor_tabview_1_tab_3);
    lv_label_set_text(ui->monitor_label_5, "工作时间：");
    lv_label_set_long_mode(ui->monitor_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_5, -14, -6);
    lv_obj_set_size(ui->monitor_label_5, 88, 21);

    // Write style for monitor_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_5, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_5, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_spinner_1
    ui->monitor_spinner_1 = lv_spinner_create(ui->monitor_tabview_1_tab_3, 3000, 60);
    lv_obj_set_pos(ui->monitor_spinner_1, 150, 96);
    lv_obj_set_size(ui->monitor_spinner_1, 78, 74);
    lv_obj_add_flag(ui->monitor_spinner_1, LV_OBJ_FLAG_HIDDEN);

    // Write style for monitor_spinner_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->monitor_spinner_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_spinner_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_spinner_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_spinner_1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_spinner_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->monitor_spinner_1, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->monitor_spinner_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->monitor_spinner_1, lv_color_hex(0xd5d6de), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_spinner_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style for monitor_spinner_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->monitor_spinner_1, 12, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->monitor_spinner_1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->monitor_spinner_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // Write codes monitor_label_6
    ui->monitor_label_6 = lv_label_create(ui->monitor_tabview_1_tab_3);
    lv_label_set_text(ui->monitor_label_6, "休息时间：");
    lv_label_set_long_mode(ui->monitor_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_6, -14, 29);
    lv_obj_set_size(ui->monitor_label_6, 88, 21);

    // Write style for monitor_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_6, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_ddlist_rest_time
    ui->monitor_ddlist_rest_time = lv_dropdown_create(ui->monitor_tabview_1_tab_3);
    lv_dropdown_set_options(ui->monitor_ddlist_rest_time, "5min\n10min\n15min\n30min\n1h\n1min");
    lv_obj_set_pos(ui->monitor_ddlist_rest_time, 65, 27);
    lv_obj_set_size(ui->monitor_ddlist_rest_time, 82, 30);

    // Write style for monitor_ddlist_rest_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->monitor_ddlist_rest_time, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_ddlist_rest_time, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_ddlist_rest_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_ddlist_rest_time, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_ddlist_rest_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_ddlist_rest_time, lv_color_hex(0x4da0ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_ddlist_rest_time, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_ddlist_rest_time, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_ddlist_rest_time, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_ddlist_rest_time, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_ddlist_rest_time, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_ddlist_rest_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_ddlist_rest_time, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_ddlist_rest_time, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_ddlist_rest_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_monitor_ddlist_rest_time_extra_list_selected_checked
    static lv_style_t style_monitor_ddlist_rest_time_extra_list_selected_checked;
    ui_init_style(&style_monitor_ddlist_rest_time_extra_list_selected_checked);

    lv_style_set_border_width(&style_monitor_ddlist_rest_time_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_monitor_ddlist_rest_time_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_monitor_ddlist_rest_time_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_monitor_ddlist_rest_time_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_monitor_ddlist_rest_time_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_rest_time_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_rest_time_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_rest_time_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_rest_time), &style_monitor_ddlist_rest_time_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_ddlist_rest_time_extra_list_main_default
    static lv_style_t style_monitor_ddlist_rest_time_extra_list_main_default;
    ui_init_style(&style_monitor_ddlist_rest_time_extra_list_main_default);

    lv_style_set_max_height(&style_monitor_ddlist_rest_time_extra_list_main_default, 90);
    lv_style_set_text_color(&style_monitor_ddlist_rest_time_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_monitor_ddlist_rest_time_extra_list_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_ddlist_rest_time_extra_list_main_default, 255);
    lv_style_set_border_width(&style_monitor_ddlist_rest_time_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_monitor_ddlist_rest_time_extra_list_main_default, 255);
    lv_style_set_border_color(&style_monitor_ddlist_rest_time_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_monitor_ddlist_rest_time_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_monitor_ddlist_rest_time_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_rest_time_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_rest_time_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_rest_time_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_rest_time), &style_monitor_ddlist_rest_time_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_ddlist_rest_time_extra_list_scrollbar_default
    static lv_style_t style_monitor_ddlist_rest_time_extra_list_scrollbar_default;
    ui_init_style(&style_monitor_ddlist_rest_time_extra_list_scrollbar_default);

    lv_style_set_radius(&style_monitor_ddlist_rest_time_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_rest_time_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_rest_time_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_rest_time_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_rest_time), &style_monitor_ddlist_rest_time_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes monitor_label_9
    ui->monitor_label_9 = lv_label_create(ui->monitor_tabview_1_tab_3);
    lv_label_set_text(ui->monitor_label_9, "循环次数：");
    lv_label_set_long_mode(ui->monitor_label_9, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_9, -13, 67);
    lv_obj_set_size(ui->monitor_label_9, 88, 21);

    // Write style for monitor_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_9, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_9, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_ddlist_loop
    ui->monitor_ddlist_loop = lv_dropdown_create(ui->monitor_tabview_1_tab_3);
    lv_dropdown_set_options(ui->monitor_ddlist_loop, "1\n2\n3\n4\n5");
    lv_obj_set_pos(ui->monitor_ddlist_loop, 65, 63);
    lv_obj_set_size(ui->monitor_ddlist_loop, 82, 30);

    // Write style for monitor_ddlist_loop, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->monitor_ddlist_loop, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_ddlist_loop, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_ddlist_loop, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_ddlist_loop, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_ddlist_loop, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_ddlist_loop, lv_color_hex(0x4da0ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_ddlist_loop, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_ddlist_loop, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_ddlist_loop, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_ddlist_loop, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_ddlist_loop, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_ddlist_loop, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_ddlist_loop, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_ddlist_loop, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_ddlist_loop, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_monitor_ddlist_loop_extra_list_selected_checked
    static lv_style_t style_monitor_ddlist_loop_extra_list_selected_checked;
    ui_init_style(&style_monitor_ddlist_loop_extra_list_selected_checked);

    lv_style_set_border_width(&style_monitor_ddlist_loop_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_monitor_ddlist_loop_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_monitor_ddlist_loop_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_monitor_ddlist_loop_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_monitor_ddlist_loop_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_loop_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_loop_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_loop_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_loop), &style_monitor_ddlist_loop_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_ddlist_loop_extra_list_main_default
    static lv_style_t style_monitor_ddlist_loop_extra_list_main_default;
    ui_init_style(&style_monitor_ddlist_loop_extra_list_main_default);

    lv_style_set_max_height(&style_monitor_ddlist_loop_extra_list_main_default, 90);
    lv_style_set_text_color(&style_monitor_ddlist_loop_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_monitor_ddlist_loop_extra_list_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_ddlist_loop_extra_list_main_default, 255);
    lv_style_set_border_width(&style_monitor_ddlist_loop_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_monitor_ddlist_loop_extra_list_main_default, 255);
    lv_style_set_border_color(&style_monitor_ddlist_loop_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_monitor_ddlist_loop_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_monitor_ddlist_loop_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_loop_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_loop_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_loop_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_loop), &style_monitor_ddlist_loop_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_ddlist_loop_extra_list_scrollbar_default
    static lv_style_t style_monitor_ddlist_loop_extra_list_scrollbar_default;
    ui_init_style(&style_monitor_ddlist_loop_extra_list_scrollbar_default);

    lv_style_set_radius(&style_monitor_ddlist_loop_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_loop_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_loop_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_loop_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_loop), &style_monitor_ddlist_loop_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes monitor_ddlist_water
    ui->monitor_ddlist_water = lv_dropdown_create(ui->monitor_tabview_1_tab_3);
    lv_dropdown_set_options(ui->monitor_ddlist_water, "30min\n1h\n1.5h\n2min\n30s");
    lv_obj_set_pos(ui->monitor_ddlist_water, 172, 28);
    lv_obj_set_size(ui->monitor_ddlist_water, 82, 30);

    // Write style for monitor_ddlist_water, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->monitor_ddlist_water, lv_color_hex(0x0D3055), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_ddlist_water, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_ddlist_water, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_ddlist_water, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_ddlist_water, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_ddlist_water, lv_color_hex(0xf2a5f1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_ddlist_water, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_ddlist_water, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_ddlist_water, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_ddlist_water, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_ddlist_water, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_ddlist_water, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_ddlist_water, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_ddlist_water, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_ddlist_water, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_CHECKED for &style_monitor_ddlist_water_extra_list_selected_checked
    static lv_style_t style_monitor_ddlist_water_extra_list_selected_checked;
    ui_init_style(&style_monitor_ddlist_water_extra_list_selected_checked);

    lv_style_set_border_width(&style_monitor_ddlist_water_extra_list_selected_checked, 1);
    lv_style_set_border_opa(&style_monitor_ddlist_water_extra_list_selected_checked, 255);
    lv_style_set_border_color(&style_monitor_ddlist_water_extra_list_selected_checked, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_monitor_ddlist_water_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_monitor_ddlist_water_extra_list_selected_checked, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_water_extra_list_selected_checked, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_water_extra_list_selected_checked, lv_color_hex(0x00a1b5));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_water_extra_list_selected_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_water), &style_monitor_ddlist_water_extra_list_selected_checked, LV_PART_SELECTED | LV_STATE_CHECKED);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_ddlist_water_extra_list_main_default
    static lv_style_t style_monitor_ddlist_water_extra_list_main_default;
    ui_init_style(&style_monitor_ddlist_water_extra_list_main_default);

    lv_style_set_max_height(&style_monitor_ddlist_water_extra_list_main_default, 90);
    lv_style_set_text_color(&style_monitor_ddlist_water_extra_list_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_monitor_ddlist_water_extra_list_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_ddlist_water_extra_list_main_default, 255);
    lv_style_set_border_width(&style_monitor_ddlist_water_extra_list_main_default, 1);
    lv_style_set_border_opa(&style_monitor_ddlist_water_extra_list_main_default, 255);
    lv_style_set_border_color(&style_monitor_ddlist_water_extra_list_main_default, lv_color_hex(0xe1e6ee));
    lv_style_set_border_side(&style_monitor_ddlist_water_extra_list_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_monitor_ddlist_water_extra_list_main_default, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_water_extra_list_main_default, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_water_extra_list_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_water_extra_list_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_water), &style_monitor_ddlist_water_extra_list_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_ddlist_water_extra_list_scrollbar_default
    static lv_style_t style_monitor_ddlist_water_extra_list_scrollbar_default;
    ui_init_style(&style_monitor_ddlist_water_extra_list_scrollbar_default);

    lv_style_set_radius(&style_monitor_ddlist_water_extra_list_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_monitor_ddlist_water_extra_list_scrollbar_default, 255);
    lv_style_set_bg_color(&style_monitor_ddlist_water_extra_list_scrollbar_default, lv_color_hex(0x00ff00));
    lv_style_set_bg_grad_dir(&style_monitor_ddlist_water_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(lv_dropdown_get_list(ui->monitor_ddlist_water), &style_monitor_ddlist_water_extra_list_scrollbar_default, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    // Write codes monitor_label_39
    ui->monitor_label_39 = lv_label_create(ui->monitor_tabview_1_tab_3);
    lv_label_set_text(ui->monitor_label_39, "喝水提醒间隔：");
    lv_label_set_long_mode(ui->monitor_label_39, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_39, 173, -5);
    lv_obj_set_size(ui->monitor_label_39, 112, 21);

    // Write style for monitor_label_39, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_39, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_39, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_39, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_39, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_39, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write codes monitor_label_40
    ui->monitor_label_40 = lv_label_create(ui->monitor_tabview_1_tab_3);
    lv_label_set_text(ui->monitor_label_40, "demo");
    lv_label_set_long_mode(ui->monitor_label_40, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_40, 155, 71);
    lv_obj_set_size(ui->monitor_label_40, 51, 18);

    //Write style for monitor_label_40, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_40, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_40, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_40, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_40, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes monitor_sw_demo
    ui->monitor_sw_demo = lv_switch_create(ui->monitor_tabview_1_tab_3);
    lv_obj_set_pos(ui->monitor_sw_demo, 212, 70);
    lv_obj_set_size(ui->monitor_sw_demo, 40, 20);

    //Write style for monitor_sw_demo, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_sw_demo, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_sw_demo, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_sw_demo, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_sw_demo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_sw_demo, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_sw_demo, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for monitor_sw_demo, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->monitor_sw_demo, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->monitor_sw_demo, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->monitor_sw_demo, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->monitor_sw_demo, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for monitor_sw_demo, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_sw_demo, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_sw_demo, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_sw_demo, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_sw_demo, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_sw_demo, 10, LV_PART_KNOB|LV_STATE_DEFAULT);


    // Write codes monitor_cont_2
    ui->monitor_cont_2 = lv_obj_create(ui->monitor);
    lv_obj_set_pos(ui->monitor_cont_2, 0, 0);
    lv_obj_set_size(ui->monitor_cont_2, 320, 15);
    lv_obj_set_scrollbar_mode(ui->monitor_cont_2, LV_SCROLLBAR_MODE_OFF);

    // Write style for monitor_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_cont_2, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->monitor_cont_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->monitor_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->monitor_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_cont_2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_cont_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_datetext_1
    ui->monitor_datetext_1 = lv_label_create(ui->monitor);
    lv_label_set_text(ui->monitor_datetext_1, "2025/06/30");
    lv_obj_set_style_text_align(ui->monitor_datetext_1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_add_flag(ui->monitor_datetext_1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui->monitor_datetext_1, monitor_datetext_1_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->monitor_datetext_1, 0, 0);
    lv_obj_set_size(ui->monitor_datetext_1, 100, 15);

    // Write style for monitor_datetext_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->monitor_datetext_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_datetext_1, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_datetext_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_datetext_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_datetext_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_datetext_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_datetext_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_datetext_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_datetext_1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_datetext_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_digital_clock_1
    static bool monitor_digital_clock_1_timer_enabled = false;
    ui->monitor_digital_clock_1 = lv_dclock_create(ui->monitor, "11:25:50");
    if (!monitor_digital_clock_1_timer_enabled)
    {
        lv_timer_create(monitor_digital_clock_1_timer, 1000, NULL);
        monitor_digital_clock_1_timer_enabled = true;
    }
    lv_obj_set_pos(ui->monitor_digital_clock_1, 220, 0);
    lv_obj_set_size(ui->monitor_digital_clock_1, 100, 15);

    // Write style for monitor_digital_clock_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->monitor_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_digital_clock_1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_digital_clock_1, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_digital_clock_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_digital_clock_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_digital_clock_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_digital_clock_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_digital_clock_1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_digital_clock_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_digital_clock_1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_digital_clock_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_btn_1
    ui->monitor_btn_1 = lv_btn_create(ui->monitor);
    ui->monitor_btn_1_label = lv_label_create(ui->monitor_btn_1);
    lv_label_set_text(ui->monitor_btn_1_label, "");
    lv_label_set_long_mode(ui->monitor_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->monitor_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->monitor_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->monitor_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->monitor_btn_1, 7, 213);
    lv_obj_set_size(ui->monitor_btn_1, 23, 23);

    // Write style for monitor_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_btn_1, lv_color_hex(0xf9dfff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->monitor_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_btn_1, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_btn_1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->monitor_btn_1, lv_color_hex(0xb8a0bd), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->monitor_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->monitor_btn_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->monitor_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->monitor_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_btn_1, lv_color_hex(0x575757), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_btn_1, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_label_wifi
    ui->monitor_label_wifi = lv_label_create(ui->monitor);
    lv_label_set_text(ui->monitor_label_wifi, "");
    lv_label_set_long_mode(ui->monitor_label_wifi, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->monitor_label_wifi, 94, 0);
    lv_obj_set_size(ui->monitor_label_wifi, 20, 15);
    lv_obj_add_flag(ui->monitor_label_wifi, LV_OBJ_FLAG_HIDDEN);

    // Write style for monitor_label_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->monitor_label_wifi, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->monitor_label_wifi, &lv_font_iconfont_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->monitor_label_wifi, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->monitor_label_wifi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_label_wifi, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_win_heart
    ui->monitor_win_heart = lv_win_create(ui->monitor, 20);
    lv_obj_t *monitor_win_heart_title = lv_win_add_title(ui->monitor_win_heart, "心率曲线");
    ui->monitor_win_heart_item0 = lv_win_add_btn(ui->monitor_win_heart, LV_SYMBOL_CLOSE, 40);
    lv_obj_t *monitor_win_heart_label = lv_label_create(lv_win_get_content(ui->monitor_win_heart));
    lv_label_set_text(monitor_win_heart_label, "");
    lv_obj_set_scrollbar_mode(lv_win_get_content(ui->monitor_win_heart), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_pos(ui->monitor_win_heart, 57, 57);
    lv_obj_set_size(ui->monitor_win_heart, 200, 150);
    lv_obj_set_scrollbar_mode(ui->monitor_win_heart, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->monitor_win_heart, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->monitor_win_heart, LV_OBJ_FLAG_HIDDEN);

    // 新增：心率曲线图表
    ui->monitor_heart_chart = lv_chart_create(lv_win_get_content(ui->monitor_win_heart));
    lv_obj_set_size(ui->monitor_heart_chart, 180, 110);
    lv_obj_set_pos(ui->monitor_heart_chart, 0, 0);
    // 设置为折线图
    lv_chart_set_type(ui->monitor_heart_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(ui->monitor_heart_chart, LV_CHART_AXIS_PRIMARY_Y, 40, 150); // 心率范围
    lv_chart_set_point_count(ui->monitor_heart_chart, 20);                         // 20个点
    lv_chart_set_update_mode(ui->monitor_heart_chart, LV_CHART_UPDATE_MODE_SHIFT); 
    // 设置主区域为淡红色半透明背景
    lv_obj_set_style_bg_opa(ui->monitor_heart_chart, 120, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui->monitor_heart_chart, lv_palette_lighten(LV_PALETTE_RED, 3), LV_PART_MAIN);
    ui->monitor_heart_series = lv_chart_add_series(ui->monitor_heart_chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    for (int i = 0; i < 20; i++)
    {
        lv_chart_set_value_by_id(ui->monitor_heart_chart, ui->monitor_heart_series, i, 0);
    }
    lv_chart_refresh(ui->monitor_heart_chart);

    // Write style for monitor_win_heart, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_win_heart, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_win_heart, lv_color_hex(0xeeeef6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_win_heart, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->monitor_win_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_win_heart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_heart_extra_content_main_default
    static lv_style_t style_monitor_win_heart_extra_content_main_default;
    ui_init_style(&style_monitor_win_heart_extra_content_main_default);

    lv_style_set_bg_opa(&style_monitor_win_heart_extra_content_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_heart_extra_content_main_default, lv_color_hex(0xeeeef6));
    lv_style_set_bg_grad_dir(&style_monitor_win_heart_extra_content_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_monitor_win_heart_extra_content_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_monitor_win_heart_extra_content_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_win_heart_extra_content_main_default, 255);
    lv_style_set_text_letter_space(&style_monitor_win_heart_extra_content_main_default, 0);
    lv_style_set_text_line_space(&style_monitor_win_heart_extra_content_main_default, 2);
    lv_obj_add_style(lv_win_get_content(ui->monitor_win_heart), &style_monitor_win_heart_extra_content_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_heart_extra_header_main_default
    static lv_style_t style_monitor_win_heart_extra_header_main_default;
    ui_init_style(&style_monitor_win_heart_extra_header_main_default);

    lv_style_set_bg_opa(&style_monitor_win_heart_extra_header_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_heart_extra_header_main_default, lv_color_hex(0xe6e6e6));
    lv_style_set_bg_grad_dir(&style_monitor_win_heart_extra_header_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_monitor_win_heart_extra_header_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_monitor_win_heart_extra_header_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_win_heart_extra_header_main_default, 255);
    lv_style_set_text_letter_space(&style_monitor_win_heart_extra_header_main_default, 0);
    lv_style_set_text_line_space(&style_monitor_win_heart_extra_header_main_default, 2);
    lv_style_set_pad_top(&style_monitor_win_heart_extra_header_main_default, 5);
    lv_style_set_pad_right(&style_monitor_win_heart_extra_header_main_default, 5);
    lv_style_set_pad_bottom(&style_monitor_win_heart_extra_header_main_default, 5);
    lv_style_set_pad_left(&style_monitor_win_heart_extra_header_main_default, 5);
    lv_style_set_pad_column(&style_monitor_win_heart_extra_header_main_default, 5);
    lv_obj_add_style(lv_win_get_header(ui->monitor_win_heart), &style_monitor_win_heart_extra_header_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_heart_extra_btns_main_default
    static lv_style_t style_monitor_win_heart_extra_btns_main_default;
    ui_init_style(&style_monitor_win_heart_extra_btns_main_default);

    lv_style_set_radius(&style_monitor_win_heart_extra_btns_main_default, 8);
    lv_style_set_border_width(&style_monitor_win_heart_extra_btns_main_default, 0);
    lv_style_set_bg_opa(&style_monitor_win_heart_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_heart_extra_btns_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_monitor_win_heart_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_shadow_width(&style_monitor_win_heart_extra_btns_main_default, 0);
    lv_obj_add_style(ui->monitor_win_heart_item0, &style_monitor_win_heart_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_win_breath
    ui->monitor_win_breath = lv_win_create(ui->monitor, 20);
    lv_obj_t *monitor_win_breath_title = lv_win_add_title(ui->monitor_win_breath, "呼吸曲线");
    ui->monitor_win_breath_item0 = lv_win_add_btn(ui->monitor_win_breath, LV_SYMBOL_CLOSE, 40);
    lv_obj_t *monitor_win_breath_label = lv_label_create(lv_win_get_content(ui->monitor_win_breath));
    lv_label_set_text(monitor_win_breath_label, "");
    lv_obj_set_scrollbar_mode(lv_win_get_content(ui->monitor_win_breath), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_pos(ui->monitor_win_breath, 57, 57);
    lv_obj_set_size(ui->monitor_win_breath, 200, 150);
    lv_obj_set_scrollbar_mode(ui->monitor_win_breath, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->monitor_win_breath, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->monitor_win_breath, LV_OBJ_FLAG_HIDDEN);

    // 新增：呼吸曲线图表
    ui->monitor_breath_chart = lv_chart_create(lv_win_get_content(ui->monitor_win_breath));
    lv_obj_set_size(ui->monitor_breath_chart, 180, 110);
    lv_obj_set_pos(ui->monitor_breath_chart, 0, 0);
    // 设置为折线图
    lv_chart_set_type(ui->monitor_breath_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(ui->monitor_breath_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 35); // 呼吸频率范围
    lv_chart_set_point_count(ui->monitor_breath_chart, 20);                       // 20个点
    lv_chart_set_update_mode(ui->monitor_breath_chart, LV_CHART_UPDATE_MODE_SHIFT);
    // 设置主区域为淡蓝色半透明背景
    lv_obj_set_style_bg_opa(ui->monitor_breath_chart, 120, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui->monitor_breath_chart, lv_palette_lighten(LV_PALETTE_BLUE, 3), LV_PART_MAIN);
    ui->monitor_breath_series = lv_chart_add_series(ui->monitor_breath_chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);
    for (int i = 0; i < 20; i++)
    {
        lv_chart_set_value_by_id(ui->monitor_breath_chart, ui->monitor_breath_series, i, 0);
    }
    lv_chart_refresh(ui->monitor_breath_chart);

    // Write style for monitor_win_breath, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_win_breath, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_win_breath, lv_color_hex(0xeeeef6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_win_breath, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->monitor_win_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_win_breath, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_breath_extra_content_main_default
    static lv_style_t style_monitor_win_breath_extra_content_main_default;
    ui_init_style(&style_monitor_win_breath_extra_content_main_default);

    lv_style_set_bg_opa(&style_monitor_win_breath_extra_content_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_breath_extra_content_main_default, lv_color_hex(0xeeeef6));
    lv_style_set_bg_grad_dir(&style_monitor_win_breath_extra_content_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_monitor_win_breath_extra_content_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_monitor_win_breath_extra_content_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_win_breath_extra_content_main_default, 255);
    lv_style_set_text_letter_space(&style_monitor_win_breath_extra_content_main_default, 0);
    lv_style_set_text_line_space(&style_monitor_win_breath_extra_content_main_default, 2);
    lv_obj_add_style(lv_win_get_content(ui->monitor_win_breath), &style_monitor_win_breath_extra_content_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_breath_extra_header_main_default
    static lv_style_t style_monitor_win_breath_extra_header_main_default;
    ui_init_style(&style_monitor_win_breath_extra_header_main_default);

    lv_style_set_bg_opa(&style_monitor_win_breath_extra_header_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_breath_extra_header_main_default, lv_color_hex(0xe6e6e6));
    lv_style_set_bg_grad_dir(&style_monitor_win_breath_extra_header_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_monitor_win_breath_extra_header_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_monitor_win_breath_extra_header_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_win_breath_extra_header_main_default, 255);
    lv_style_set_text_letter_space(&style_monitor_win_breath_extra_header_main_default, 0);
    lv_style_set_text_line_space(&style_monitor_win_breath_extra_header_main_default, 2);
    lv_style_set_pad_top(&style_monitor_win_breath_extra_header_main_default, 5);
    lv_style_set_pad_right(&style_monitor_win_breath_extra_header_main_default, 5);
    lv_style_set_pad_bottom(&style_monitor_win_breath_extra_header_main_default, 5);
    lv_style_set_pad_left(&style_monitor_win_breath_extra_header_main_default, 5);
    lv_style_set_pad_column(&style_monitor_win_breath_extra_header_main_default, 5);
    lv_obj_add_style(lv_win_get_header(ui->monitor_win_breath), &style_monitor_win_breath_extra_header_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_breath_extra_btns_main_default
    static lv_style_t style_monitor_win_breath_extra_btns_main_default;
    ui_init_style(&style_monitor_win_breath_extra_btns_main_default);

    lv_style_set_radius(&style_monitor_win_breath_extra_btns_main_default, 8);
    lv_style_set_border_width(&style_monitor_win_breath_extra_btns_main_default, 0);
    lv_style_set_bg_opa(&style_monitor_win_breath_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_breath_extra_btns_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_monitor_win_breath_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_shadow_width(&style_monitor_win_breath_extra_btns_main_default, 0);
    lv_obj_add_style(ui->monitor_win_breath_item0, &style_monitor_win_breath_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_win_temperature
    ui->monitor_win_temperature = lv_win_create(ui->monitor, 20);
    lv_obj_t *monitor_win_temperature_title = lv_win_add_title(ui->monitor_win_temperature, "温度曲线");
    ui->monitor_win_temperature_item0 = lv_win_add_btn(ui->monitor_win_temperature, LV_SYMBOL_CLOSE, 40);
    lv_obj_t *monitor_win_temperature_label = lv_label_create(lv_win_get_content(ui->monitor_win_temperature));
    lv_label_set_text(monitor_win_temperature_label, "");
    lv_obj_set_scrollbar_mode(lv_win_get_content(ui->monitor_win_temperature), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_pos(ui->monitor_win_temperature, 57, 57);
    lv_obj_set_size(ui->monitor_win_temperature, 200, 150);
    lv_obj_set_scrollbar_mode(ui->monitor_win_temperature, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->monitor_win_temperature, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->monitor_win_temperature, LV_OBJ_FLAG_HIDDEN);

    // 新增：温度曲线图表
    ui->monitor_temperature_chart = lv_chart_create(lv_win_get_content(ui->monitor_win_temperature));
    lv_obj_set_size(ui->monitor_temperature_chart, 180, 110);
    lv_obj_set_pos(ui->monitor_temperature_chart, 0, 0);
    // 设置为折线图
    lv_chart_set_type(ui->monitor_temperature_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(ui->monitor_temperature_chart, LV_CHART_AXIS_PRIMARY_Y, 10, 40); // 温度范围
    lv_chart_set_point_count(ui->monitor_temperature_chart, 20);                        // 20个点
    lv_chart_set_update_mode(ui->monitor_temperature_chart, LV_CHART_UPDATE_MODE_SHIFT);
    // 设置主区域为淡橙色半透明背景
    lv_obj_set_style_bg_opa(ui->monitor_temperature_chart, 120, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui->monitor_temperature_chart, lv_palette_lighten(LV_PALETTE_ORANGE, 3), LV_PART_MAIN);
    ui->monitor_temperature_series = lv_chart_add_series(ui->monitor_temperature_chart, lv_palette_main(LV_PALETTE_ORANGE), LV_CHART_AXIS_PRIMARY_Y);
    for (int i = 0; i < 20; i++)
    {
        lv_chart_set_value_by_id(ui->monitor_temperature_chart, ui->monitor_temperature_series, i, 0);
    }
    lv_chart_refresh(ui->monitor_temperature_chart);

    // Write style for monitor_win_temperature, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_win_temperature, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_win_temperature, lv_color_hex(0xeeeef6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_win_temperature, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->monitor_win_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_win_temperature, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_temperature_extra_content_main_default
    static lv_style_t style_monitor_win_temperature_extra_content_main_default;
    ui_init_style(&style_monitor_win_temperature_extra_content_main_default);

    lv_style_set_bg_opa(&style_monitor_win_temperature_extra_content_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_temperature_extra_content_main_default, lv_color_hex(0xeeeef6));
    lv_style_set_bg_grad_dir(&style_monitor_win_temperature_extra_content_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_monitor_win_temperature_extra_content_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_monitor_win_temperature_extra_content_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_win_temperature_extra_content_main_default, 255);
    lv_style_set_text_letter_space(&style_monitor_win_temperature_extra_content_main_default, 0);
    lv_style_set_text_line_space(&style_monitor_win_temperature_extra_content_main_default, 2);
    lv_obj_add_style(lv_win_get_content(ui->monitor_win_temperature), &style_monitor_win_temperature_extra_content_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_temperature_extra_header_main_default
    static lv_style_t style_monitor_win_temperature_extra_header_main_default;
    ui_init_style(&style_monitor_win_temperature_extra_header_main_default);

    lv_style_set_bg_opa(&style_monitor_win_temperature_extra_header_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_temperature_extra_header_main_default, lv_color_hex(0xe6e6e6));
    lv_style_set_bg_grad_dir(&style_monitor_win_temperature_extra_header_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_monitor_win_temperature_extra_header_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_monitor_win_temperature_extra_header_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_win_temperature_extra_header_main_default, 255);
    lv_style_set_text_letter_space(&style_monitor_win_temperature_extra_header_main_default, 0);
    lv_style_set_text_line_space(&style_monitor_win_temperature_extra_header_main_default, 2);
    lv_style_set_pad_top(&style_monitor_win_temperature_extra_header_main_default, 5);
    lv_style_set_pad_right(&style_monitor_win_temperature_extra_header_main_default, 5);
    lv_style_set_pad_bottom(&style_monitor_win_temperature_extra_header_main_default, 5);
    lv_style_set_pad_left(&style_monitor_win_temperature_extra_header_main_default, 5);
    lv_style_set_pad_column(&style_monitor_win_temperature_extra_header_main_default, 5);
    lv_obj_add_style(lv_win_get_header(ui->monitor_win_temperature), &style_monitor_win_temperature_extra_header_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_temperature_extra_btns_main_default
    static lv_style_t style_monitor_win_temperature_extra_btns_main_default;
    ui_init_style(&style_monitor_win_temperature_extra_btns_main_default);

    lv_style_set_radius(&style_monitor_win_temperature_extra_btns_main_default, 8);
    lv_style_set_border_width(&style_monitor_win_temperature_extra_btns_main_default, 0);
    lv_style_set_bg_opa(&style_monitor_win_temperature_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_temperature_extra_btns_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_monitor_win_temperature_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_shadow_width(&style_monitor_win_temperature_extra_btns_main_default, 0);
    lv_obj_add_style(ui->monitor_win_temperature_item0, &style_monitor_win_temperature_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes monitor_win_humidity
    ui->monitor_win_humidity = lv_win_create(ui->monitor, 20);
    lv_obj_t *monitor_win_humidity_title = lv_win_add_title(ui->monitor_win_humidity, "湿度曲线");
    ui->monitor_win_humidity_item0 = lv_win_add_btn(ui->monitor_win_humidity, LV_SYMBOL_CLOSE, 40);
    lv_obj_t *monitor_win_humidity_label = lv_label_create(lv_win_get_content(ui->monitor_win_humidity));
    lv_label_set_text(monitor_win_humidity_label, "");
    lv_obj_set_scrollbar_mode(lv_win_get_content(ui->monitor_win_humidity), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_pos(ui->monitor_win_humidity, 57, 57);
    lv_obj_set_size(ui->monitor_win_humidity, 200, 150);
    lv_obj_set_scrollbar_mode(ui->monitor_win_humidity, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->monitor_win_humidity, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui->monitor_win_humidity, LV_OBJ_FLAG_HIDDEN);

    // 新增：湿度曲线图表
    ui->monitor_humidity_chart = lv_chart_create(lv_win_get_content(ui->monitor_win_humidity));
    lv_obj_set_size(ui->monitor_humidity_chart, 180, 110);
    lv_obj_set_pos(ui->monitor_humidity_chart, 0, 0);
    // 设置为折线图
    lv_chart_set_type(ui->monitor_humidity_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(ui->monitor_humidity_chart, LV_CHART_AXIS_PRIMARY_Y, 20, 60); // 湿度范围
    lv_chart_set_point_count(ui->monitor_humidity_chart, 20);                        // 20个点
    lv_chart_set_update_mode(ui->monitor_humidity_chart, LV_CHART_UPDATE_MODE_SHIFT);
    // 设置主区域为淡蓝色半透明背景
    lv_obj_set_style_bg_opa(ui->monitor_humidity_chart, 120, LV_PART_MAIN);
    lv_obj_set_style_bg_color(ui->monitor_humidity_chart, lv_palette_lighten(LV_PALETTE_CYAN, 3), LV_PART_MAIN);
    ui->monitor_humidity_series = lv_chart_add_series(ui->monitor_humidity_chart, lv_palette_main(LV_PALETTE_CYAN), LV_CHART_AXIS_PRIMARY_Y);
    for (int i = 0; i < 20; i++)
    {
        lv_chart_set_value_by_id(ui->monitor_humidity_chart, ui->monitor_humidity_series, i, 0);
    }
    lv_chart_refresh(ui->monitor_humidity_chart);

    // Write style for monitor_win_humidity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->monitor_win_humidity, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->monitor_win_humidity, lv_color_hex(0xeeeef6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->monitor_win_humidity, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->monitor_win_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->monitor_win_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_humidity_extra_content_main_default
    static lv_style_t style_monitor_win_humidity_extra_content_main_default;
    ui_init_style(&style_monitor_win_humidity_extra_content_main_default);

    lv_style_set_bg_opa(&style_monitor_win_humidity_extra_content_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_humidity_extra_content_main_default, lv_color_hex(0xeeeef6));
    lv_style_set_bg_grad_dir(&style_monitor_win_humidity_extra_content_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_monitor_win_humidity_extra_content_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_monitor_win_humidity_extra_content_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_win_humidity_extra_content_main_default, 255);
    lv_style_set_text_letter_space(&style_monitor_win_humidity_extra_content_main_default, 0);
    lv_style_set_text_line_space(&style_monitor_win_humidity_extra_content_main_default, 2);
    lv_obj_add_style(lv_win_get_content(ui->monitor_win_humidity), &style_monitor_win_humidity_extra_content_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_humidity_extra_header_main_default
    static lv_style_t style_monitor_win_humidity_extra_header_main_default;
    ui_init_style(&style_monitor_win_humidity_extra_header_main_default);

    lv_style_set_bg_opa(&style_monitor_win_humidity_extra_header_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_humidity_extra_header_main_default, lv_color_hex(0xe6e6e6));
    lv_style_set_bg_grad_dir(&style_monitor_win_humidity_extra_header_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_text_color(&style_monitor_win_humidity_extra_header_main_default, lv_color_hex(0x393c41));
    lv_style_set_text_font(&style_monitor_win_humidity_extra_header_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_monitor_win_humidity_extra_header_main_default, 255);
    lv_style_set_text_letter_space(&style_monitor_win_humidity_extra_header_main_default, 0);
    lv_style_set_text_line_space(&style_monitor_win_humidity_extra_header_main_default, 2);
    lv_style_set_pad_top(&style_monitor_win_humidity_extra_header_main_default, 5);
    lv_style_set_pad_right(&style_monitor_win_humidity_extra_header_main_default, 5);
    lv_style_set_pad_bottom(&style_monitor_win_humidity_extra_header_main_default, 5);
    lv_style_set_pad_left(&style_monitor_win_humidity_extra_header_main_default, 5);
    lv_style_set_pad_column(&style_monitor_win_humidity_extra_header_main_default, 5);
    lv_obj_add_style(lv_win_get_header(ui->monitor_win_humidity), &style_monitor_win_humidity_extra_header_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write style state: LV_STATE_DEFAULT for &style_monitor_win_humidity_extra_btns_main_default
    static lv_style_t style_monitor_win_humidity_extra_btns_main_default;
    ui_init_style(&style_monitor_win_humidity_extra_btns_main_default);

    lv_style_set_radius(&style_monitor_win_humidity_extra_btns_main_default, 8);
    lv_style_set_border_width(&style_monitor_win_humidity_extra_btns_main_default, 0);
    lv_style_set_bg_opa(&style_monitor_win_humidity_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_monitor_win_humidity_extra_btns_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_monitor_win_humidity_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_shadow_width(&style_monitor_win_humidity_extra_btns_main_default, 0);
    lv_obj_add_style(ui->monitor_win_humidity_item0, &style_monitor_win_humidity_extra_btns_main_default, LV_PART_MAIN | LV_STATE_DEFAULT);

    // The custom code of monitor.

    // Update current screen layout.
    lv_obj_update_layout(ui->monitor);

    // Init events for screen.
    events_init_monitor(ui);
}
