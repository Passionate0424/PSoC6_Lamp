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



void setup_scr_guader(lv_ui *ui)
{
    //Write codes guader
    ui->guader = lv_obj_create(NULL);
    lv_obj_set_size(ui->guader, 320, 240);
    lv_obj_set_scrollbar_mode(ui->guader, LV_SCROLLBAR_MODE_OFF);

    //Write style for guader, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->guader, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes guader_cont_1
    ui->guader_cont_1 = lv_obj_create(ui->guader);
    lv_obj_set_pos(ui->guader_cont_1, 0, 0);
    lv_obj_set_size(ui->guader_cont_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->guader_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for guader_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->guader_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->guader_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->guader_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->guader_cont_1, lv_color_hex(0xbb57ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_dither_mode(ui->guader_cont_1, LV_DITHER_ORDERED, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->guader_cont_1, LV_GRAD_DIR_HOR, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->guader_cont_1, lv_color_hex(0x7b57ff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->guader_cont_1, 87, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->guader_cont_1, 153, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->guader_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->guader_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->guader_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->guader_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes guader_spinner_1
    ui->guader_spinner_1 = lv_spinner_create(ui->guader, 3000, 50);
    lv_obj_set_pos(ui->guader_spinner_1, 100, 24);
    lv_obj_set_size(ui->guader_spinner_1, 120, 120);

    //Write style for guader_spinner_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_pad_top(ui->guader_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->guader_spinner_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->guader_spinner_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->guader_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->guader_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->guader_spinner_1, 20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->guader_spinner_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->guader_spinner_1, lv_color_hex(0xd5d6de), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_spinner_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for guader_spinner_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->guader_spinner_1, 20, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->guader_spinner_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->guader_spinner_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes guader_label_1
    ui->guader_label_1 = lv_label_create(ui->guader);
    lv_label_set_text(ui->guader_label_1, "");
    lv_label_set_long_mode(ui->guader_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->guader_label_1, 142, 62);
    lv_obj_set_size(ui->guader_label_1, 40, 40);

    //Write style for guader_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->guader_label_1, lv_color_hex(0xaffe07), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->guader_label_1, &lv_font_iconfont_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->guader_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->guader_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes guader_btn_1
    ui->guader_btn_1 = lv_btn_create(ui->guader);
    ui->guader_btn_1_label = lv_label_create(ui->guader_btn_1);
    lv_label_set_text(ui->guader_btn_1_label, "退\n出");
    lv_label_set_long_mode(ui->guader_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->guader_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->guader_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->guader_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->guader_btn_1, 5, 91);
    lv_obj_set_size(ui->guader_btn_1, 23, 40);

    //Write style for guader_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->guader_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->guader_btn_1, lv_color_hex(0xf9dfff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->guader_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->guader_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->guader_btn_1, 25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_btn_1, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->guader_btn_1, lv_color_hex(0xb8a0bd), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->guader_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->guader_btn_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->guader_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->guader_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->guader_btn_1, lv_color_hex(0x575757), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->guader_btn_1, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->guader_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->guader_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes guader_cont_2
    ui->guader_cont_2 = lv_obj_create(ui->guader);
    lv_obj_set_pos(ui->guader_cont_2, 100, 182);
    lv_obj_set_size(ui->guader_cont_2, 120, 30);
    lv_obj_set_scrollbar_mode(ui->guader_cont_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for guader_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->guader_cont_2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->guader_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->guader_cont_2, lv_color_hex(0x7fc2fa), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->guader_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->guader_cont_2, 14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->guader_cont_2, 123, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->guader_cont_2, lv_color_hex(0x52e0fe), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->guader_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->guader_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->guader_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->guader_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->guader_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_cont_2, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->guader_cont_2, lv_color_hex(0x7fc2fa), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->guader_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->guader_cont_2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->guader_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->guader_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes guader_label_2
    ui->guader_label_2 = lv_label_create(ui->guader_cont_2);
    lv_label_set_text(ui->guader_label_2, "记录次数：");
    lv_label_set_long_mode(ui->guader_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->guader_label_2, 4, 5);
    lv_obj_set_size(ui->guader_label_2, 70, 17);

    //Write style for guader_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->guader_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->guader_label_2, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->guader_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->guader_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes guader_label_times
    ui->guader_label_times = lv_label_create(ui->guader_cont_2);
    lv_label_set_text(ui->guader_label_times, "--");
    lv_label_set_long_mode(ui->guader_label_times, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->guader_label_times, 72, 5);
    lv_obj_set_size(ui->guader_label_times, 25, 17);

    //Write style for guader_label_times, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->guader_label_times, lv_color_hex(0xfffe34), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->guader_label_times, &lv_font_montserratMedium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->guader_label_times, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->guader_label_times, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_label_times, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes guader_label_4
    ui->guader_label_4 = lv_label_create(ui->guader);
    lv_label_set_text(ui->guader_label_4, "");
    lv_label_set_long_mode(ui->guader_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->guader_label_4, 31, 144);
    lv_obj_set_size(ui->guader_label_4, 46, 27);

    //Write style for guader_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->guader_label_4, lv_color_hex(0xf44d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->guader_label_4, &lv_font_iconfont_25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->guader_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->guader_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes guader_label_5
    ui->guader_label_5 = lv_label_create(ui->guader);
    lv_label_set_text(ui->guader_label_5, "");
    lv_label_set_long_mode(ui->guader_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->guader_label_5, 245, 144);
    lv_obj_set_size(ui->guader_label_5, 46, 28);

    //Write style for guader_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->guader_label_5, lv_color_hex(0xf44d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->guader_label_5, &lv_font_iconfont_25, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->guader_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->guader_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes guader_label_6
    ui->guader_label_6 = lv_label_create(ui->guader);
    lv_label_set_text(ui->guader_label_6, "您在监控范围内的行为都将被记录");
    lv_label_set_long_mode(ui->guader_label_6, LV_LABEL_LONG_SCROLL);
    lv_obj_set_pos(ui->guader_label_6, 89, 150);
    lv_obj_set_size(ui->guader_label_6, 147, 16);

    //Write style for guader_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->guader_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->guader_label_6, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->guader_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->guader_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->guader_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of guader.


    //Update current screen layout.
    lv_obj_update_layout(ui->guader);

    //Init events for screen.
    events_init_guader(ui);
}
