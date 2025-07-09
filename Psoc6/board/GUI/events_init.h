/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


#ifndef EVENTS_INIT_H_
#define EVENTS_INIT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"
#include "work_monitor.h"

extern bool btn1_status;
extern bool guard_status;
extern rt_timer_t water_timer;


void events_init(lv_ui *ui);

void events_init_home(lv_ui *ui);
void events_init_monitor(lv_ui *ui);
void events_init_settings(lv_ui *ui);
void events_init_guader(lv_ui *ui);
void drink_timer_restart(void);

#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
