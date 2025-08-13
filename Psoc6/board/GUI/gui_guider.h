/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C"{
#endif

#include "lvgl.h"

extern rt_int32_t drink_count; // 喝水计数
extern int home_digital_clock_1_min_value;
extern int home_digital_clock_1_hour_value;
extern int home_digital_clock_1_sec_value;

extern int monitor_digital_clock_1_min_value;
extern int monitor_digital_clock_1_hour_value;
extern int monitor_digital_clock_1_sec_value;

extern int settings_digital_clock_1_min_value;
extern int settings_digital_clock_1_hour_value;
extern int settings_digital_clock_1_sec_value;

extern rt_mutex_t lv_mutex;
extern rt_sem_t lv_rest_end_sem;

typedef struct
{

	lv_obj_t *home;
	bool home_del;
	lv_obj_t *home_cont_1;
	lv_obj_t *home_digital_clock_1;
	lv_obj_t *home_datetext_1;
	lv_obj_t *home_btn_led;
	lv_obj_t *home_btn_led_label;
	lv_obj_t *home_bright_slider;
	lv_obj_t *home_sw_2;
	lv_obj_t *home_cont_3;
	lv_obj_t *home_btn_monitor;
	lv_obj_t *home_btn_monitor_label;
	lv_obj_t *home_btn_guard;
	lv_obj_t *home_btn_guard_label;
	lv_obj_t *home_btn_setting;
	lv_obj_t *home_btn_setting_label;
	lv_obj_t *home_label_2;
	lv_obj_t *home_label_3;
	lv_obj_t *home_label_4;
	lv_obj_t *home_label_5;
	lv_obj_t *home_btn_ai;
	lv_obj_t *home_btn_ai_label;
	lv_obj_t *monitor;
	bool monitor_del;
	lv_obj_t *monitor_cont_1;
	lv_obj_t *monitor_tabview_1;
	lv_obj_t *monitor_tabview_1_tab_1;
	lv_obj_t *monitor_tabview_1_tab_2;
	lv_obj_t *monitor_tabview_1_tab_3;
	lv_obj_t *monitor_cont_data_now;
	lv_obj_t *monitor_label_breath_now;
	lv_obj_t *monitor_temperature;
	lv_obj_t *monitor_co2;
	lv_obj_t *monitor_label_4;
	lv_obj_t *monitor_humidity;
	lv_obj_t *monitor_pressure;
	lv_obj_t *monitor_heart;
	lv_obj_t *monitor_tvoc;
	lv_obj_t *monitor_breath;
	lv_obj_t *monitor_label_humidity_now;
	lv_obj_t *monitor_label_temperature_now;
	lv_obj_t *monitor_label_pressure_now;
	lv_obj_t *monitor_label_co2_now;
	lv_obj_t *monitor_label_tvoc_now;
	lv_obj_t *monitor_label_heart_now;
	lv_obj_t *monitor_cont_score;
	lv_obj_t *monitor_label_28;
	lv_obj_t *monitor_label_27;
	lv_obj_t *monitor_label_26;
	lv_obj_t *monitor_label_stress_score;
	lv_obj_t *monitor_label_env_score;
	lv_obj_t *monitor_cont_pre;
	lv_obj_t *monitor_label_23;
	lv_obj_t *monitor_label_21;
	lv_obj_t *monitor_label_20;
	lv_obj_t *monitor_label_humidity_pre;
	lv_obj_t *monitor_label_temperature_pre;
	lv_obj_t *monitor_btn_env_advice;
	lv_obj_t *monitor_btn_env_advice_label;
	lv_obj_t *monitor_bar_energy;
	lv_obj_t *monitor_label_energy;
	lv_obj_t *monitor_label_carefulness;
	lv_obj_t *monitor_bar_carefulness;
	lv_obj_t *monitor_cont_5;
	lv_obj_t *monitor_label_7;
	lv_obj_t *monitor_cont_6;
	lv_obj_t *monitor_label_29;
	lv_obj_t *monitor_cont_work_score;
	lv_obj_t *monitor_cont_8;
	lv_obj_t *monitor_label_30;
	lv_obj_t *monitor_label_work_score;
	lv_obj_t *monitor_cont_drink;
	lv_obj_t *monitor_cont_10;
	lv_obj_t *monitor_label_32;
	lv_obj_t *monitor_label_drink;
	lv_obj_t *monitor_label_33;
	lv_obj_t *monitor_label_36;
	lv_obj_t *monitor_label_phone;
	lv_obj_t *monitor_label_work_duration;
	lv_obj_t *monitor_label_35;
	lv_obj_t *monitor_label_38;
	lv_obj_t *monitor_work_btn;
	lv_obj_t *monitor_work_btn_label;
	lv_obj_t *monitor_ddlist_work_time;
	lv_obj_t *monitor_label_5;
	lv_obj_t *monitor_spinner_1;
	lv_obj_t *monitor_label_6;
	lv_obj_t *monitor_ddlist_rest_time;
	lv_obj_t *monitor_label_9;
	lv_obj_t *monitor_ddlist_loop;
	lv_obj_t *monitor_ddlist_water;
	lv_obj_t *monitor_label_39;
	lv_obj_t *monitor_cont_2;
	lv_obj_t *monitor_datetext_1;
	lv_obj_t *monitor_digital_clock_1;
	lv_obj_t *monitor_btn_1;
	lv_obj_t *monitor_btn_1_label;
	lv_obj_t *monitor_label_wifi;
	lv_obj_t *monitor_win_heart;
	lv_obj_t *monitor_heart_chart;
	lv_obj_t *monitor_heart_series;
	lv_obj_t *monitor_win_heart_item0;
	lv_obj_t *monitor_win_breath;
	lv_obj_t *monitor_breath_chart;
	lv_obj_t *monitor_breath_series;
	lv_obj_t *monitor_win_breath_item0;
	lv_obj_t *settings_btn_up;
	lv_obj_t *settings_btn_down;
	lv_obj_t *settings_btn_left;
	lv_obj_t *settings_btn_right;
	lv_obj_t *monitor_win_temperature;
	lv_obj_t *monitor_temperature_chart;
	lv_obj_t *monitor_temperature_series;
	lv_obj_t *monitor_win_temperature_item0;
	lv_obj_t *monitor_win_humidity;
	lv_obj_t *monitor_humidity_chart;
	lv_obj_t *monitor_humidity_series;
	lv_obj_t *monitor_win_humidity_item0;
	lv_obj_t *settings;
	bool settings_del;
	lv_obj_t *settings_cont_1;
	lv_obj_t *settings_cont_2;
	lv_obj_t *settings_datetext_1;
	lv_obj_t *settings_digital_clock_1;
	lv_obj_t *settings_mode_l;
	lv_obj_t *settings_label_wifi;
	lv_obj_t *settings_btn_1;
	lv_obj_t *settings_btn_1_label;
	lv_obj_t *settings_ddlist_switch;
	lv_obj_t *settings_label_4;
	lv_obj_t *settings_ddlist_1;
	lv_obj_t *settings_btn_2;
	lv_obj_t *settings_btn_2_label;
	lv_obj_t *wifi_connect;
	bool wifi_connect_del;
	lv_obj_t *wifi_connect_cont_1;
	lv_obj_t *wifi_connect_btn_1;
	lv_obj_t *wifi_connect_btn_1_label;
	// WiFi list components
	lv_obj_t *wifi_connect_list;
	lv_obj_t *wifi_connect_title_label;
	lv_obj_t *wifi_connect_refresh_btn;
	lv_obj_t *wifi_connect_refresh_btn_label;
	lv_obj_t *guader;
	bool guader_del;
	lv_obj_t *guader_cont_1;
	lv_obj_t *guader_spinner_1;
	lv_obj_t *guader_label_1;
	lv_obj_t *guader_btn_1;
	lv_obj_t *guader_btn_1_label;
	lv_obj_t *guader_cont_2;
	lv_obj_t *guader_label_2;
	lv_obj_t *guader_label_times;
	lv_obj_t *guader_label_4;
	lv_obj_t *guader_label_5;
	lv_obj_t *guader_label_6;
	lv_obj_t *font;
	bool font_del;
	lv_obj_t *font_label_1;
	lv_obj_t *font_label_2;
	lv_obj_t *font_label_3;
	lv_obj_t *monitor_label_40;
	lv_obj_t *monitor_sw_demo;
} lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui *ui);

void ui_init_style(lv_style_t *style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t **new_scr, bool new_scr_del, bool *old_scr_del, ui_setup_scr_t setup_scr,
						   lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void *var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
				  uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
				  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;

void setup_scr_home(lv_ui *ui);
void setup_scr_monitor(lv_ui *ui);
void setup_scr_wifi_connect(lv_ui *ui);
void setup_scr_settings(lv_ui *ui);
void setup_scr_guader(lv_ui *ui);
void setup_scr_font(lv_ui *ui);

LV_FONT_DECLARE(lv_font_Alatsi_Regular_35)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_Alatsi_Regular_18)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_16)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_13)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_14)
LV_FONT_DECLARE(lv_font_iconfont_16)
LV_FONT_DECLARE(lv_font_iconfont_15)
LV_FONT_DECLARE(lv_font_iconfont_14)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_11)
LV_FONT_DECLARE(lv_font_montserratMedium_14)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_18)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_font_iconfont_40)
LV_FONT_DECLARE(lv_font_montserratMedium_15)
LV_FONT_DECLARE(lv_font_iconfont_25)

#ifdef __cplusplus
}
#endif
#endif
