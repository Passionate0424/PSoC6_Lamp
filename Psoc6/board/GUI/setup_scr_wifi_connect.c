/** * Copyright 2025 NXP
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
#include <string.h>
#include <stdbool.h>

/* ========================================================================
 * RT-Thread WLAN 集成点 #1: 添加 RT-Thread 相关头文件
 * ======================================================================== */
#include <rtthread.h>
#include <wlan_mgnt.h>
#include <wlan_cfg.h>
#include <wlan_prot.h>
#include <wlan_dev.h>

// =================== RT-Thread WLAN 实际集成代码（仅注释，供移植参考） ===================
// 1. 头文件引用
// #include <rtthread.h>
// #include <wlan_mgnt.h>
// #include <wlan_cfg.h>
// #include <wlan_prot.h>

// 2. WiFi 扫描并填充 wifi_networks
// void scan_and_update_wifi_list(void) {
//     struct rt_wlan_scan_result *scan_result = NULL;
//     int result_num = rt_wlan_scan();
//     if (result_num > 0) {
//         scan_result = rt_wlan_scan_get_result();
//         // 释放旧数据，分配新空间
//         if (wifi_networks) rt_free(wifi_networks);
//         wifi_networks = (wifi_network_t*)rt_malloc(sizeof(wifi_network_t) * result_num);
//         wifi_network_count = result_num;
//         for (int i = 0; i < result_num; i++) {
//             struct rt_wlan_info *info = &scan_result[i].info;
//             strncpy(wifi_networks[i].ssid, (char*)info->ssid.val, info->ssid.len);
//             wifi_networks[i].ssid[info->ssid.len] = '\0';
//             wifi_networks[i].rssi = info->rssi;
//             wifi_networks[i].is_secure = (info->security != SECURITY_OPEN);
//             wifi_networks[i].is_connected = rt_wlan_is_connected();
//         }
//         rt_wlan_scan_result_clean();
//     }
// }

// 3. 连接 WiFi
// void connect_wifi(const char* ssid, const char* password, bool is_secure) {
//     rt_wlan_connect_adv_info_t info;
//     rt_memset(&info, 0, sizeof(info));
//     strncpy((char*)info.ssid.val, ssid, sizeof(info.ssid.val));
//     info.ssid.len = strlen(ssid);
//     if (is_secure) {
//         strncpy((char*)info.key.val, password, sizeof(info.key.val));
//         info.key.len = strlen(password);
//         info.security = SECURITY_WPA2_MIXED_PSK; // 或根据扫描结果设置
//     } else {
//         info.security = SECURITY_OPEN;
//     }
//     rt_wlan_connect_adv(&info, password);
// }

// 4. 事件注册
// void register_wlan_events(void) {
//     rt_wlan_register_event_handler(RT_WLAN_EVT_SCAN_DONE, wlan_scan_done_handler, RT_NULL);
//     rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED, wlan_connected_handler, RT_NULL);
//     rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED_FAIL, wlan_connect_fail_handler, RT_NULL);
//     rt_wlan_register_event_handler(RT_WLAN_EVT_STA_DISCONNECTED, wlan_disconnected_handler, RT_NULL);
// }

/* ========================================================================
 * RT-Thread WLAN 集成点 #2: 替换模拟数据结构
 * ======================================================================== */
// WiFi网络信息结构体
typedef struct
{
    char ssid[32];     // 网络名称
    int rssi;          // 信号强度
    bool is_connected; // 是否已连接
    bool is_secure;    // 是否需要密码
} wifi_network_t;

// 动态分配的WiFi网络列表
static wifi_network_t *wifi_networks = NULL;
static uint16_t wifi_network_count = 0;

/* TODO: 将 wifi_network_count 改为动态计算扫描结果数量 */

// 列表项点击回调函数声明
static void wifi_list_item_click_event_cb(lv_event_t *e);
static void wifi_refresh_btn_event_cb(lv_event_t *e);
static void create_wifi_list_items(lv_ui *ui);

// 全局键盘对象指针，用于管理键盘生命周期
static lv_obj_t *current_keyboard = NULL;
static wifi_network_t *current_selected_network = NULL; // 添加当前选中网络的全局变量
static lv_obj_t *current_password_input = NULL;         // 添加当前密码输入框的全局引用
static lv_obj_t *current_progress_msgbox = NULL;        // 添加当前进度对话框的全局引用
static lv_timer_t *connection_timer = NULL;             // 添加连接计时器
static lv_timer_t *timeout_timer = NULL;                // 添加超时计时器
static bool connection_completed = false;               // 连接是否完成的标志

// 函数声明
static void connection_result_msgbox_event_cb(lv_event_t *e);
static void progress_msgbox_event_cb(lv_event_t *e);
static void wifi_connection_timer_cb(lv_timer_t *timer);
static void wifi_timeout_timer_cb(lv_timer_t *timer);
static void close_success_msgbox_timer_cb(lv_timer_t *timer);
static void close_failure_msgbox_timer_cb(lv_timer_t *timer);
static void close_timeout_msgbox_timer_cb(lv_timer_t *timer);
static void password_input_event_cb(lv_event_t *e);
static void open_network_msgbox_event_cb(lv_event_t *e);

// 清理键盘函数
static void cleanup_keyboard(void)
{
    if (current_keyboard != NULL)
    {
        lv_obj_del(current_keyboard);
        current_keyboard = NULL;
    }
}

// 统一的消息框关闭函数 - 使用成功的关闭方式
static void close_msgbox_by_title(const char *target_title)
{
    lv_obj_t *act_scr = lv_scr_act();
    uint32_t child_cnt = lv_obj_get_child_cnt(act_scr);

    for (uint32_t i = 0; i < child_cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(act_scr, i);
        if (child != NULL && lv_obj_check_type(child, &lv_msgbox_class))
        {
            lv_obj_t *title = lv_msgbox_get_title(child);
            if (title != NULL)
            {
                const char *title_text = lv_label_get_text(title);
                if (title_text && strcmp(title_text, target_title) == 0)
                {
                    lv_msgbox_close_async(child);
                    break;
                }
            }
        }
    }
}

// 关闭指定的消息框对象 - 使用遍历方式查找对象
static void close_msgbox_object(lv_obj_t *target_msgbox)
{
    if (target_msgbox == NULL)
        return;

    lv_obj_t *act_scr = lv_scr_act();
    uint32_t child_cnt = lv_obj_get_child_cnt(act_scr);

    for (uint32_t i = 0; i < child_cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(act_scr, i);
        if (child != NULL && child == target_msgbox && lv_obj_check_type(child, &lv_msgbox_class))
        {
            lv_msgbox_close_async(child);
            break;
        }
    }
}

// 密码输入对话框回调
static void password_msgbox_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        uint32_t *btn_id_ptr = (uint32_t *)lv_event_get_param(e);
        if (btn_id_ptr == NULL)
            return;
        uint32_t btn_id = *btn_id_ptr;
        if (btn_id == 0) // "连接"
        {
            const char *password = "";
            if (current_password_input != NULL)
                password = lv_textarea_get_text(current_password_input);
            cleanup_keyboard();
            current_password_input = NULL;
            close_msgbox_object(obj);
            // 显示进度框
            if (current_selected_network != NULL)
            {
                static const char *progress_btns[] = {"取消", ""};
                char progress_text[100];
                sprintf(progress_text, "正在连接到 %s...\n", current_selected_network->ssid);
                lv_obj_t *progress_msgbox = lv_msgbox_create(lv_scr_act(), "WiFi连接", progress_text, progress_btns, true);
                lv_obj_set_size(progress_msgbox, 280, 150);
                lv_obj_center(progress_msgbox);
                // 设置弹窗所有字体为思源宋体，确保中文显示
                lv_obj_t *title_obj = lv_msgbox_get_title(progress_msgbox);
                if (title_obj)
                    lv_obj_set_style_text_font(title_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_t *btns_obj = lv_msgbox_get_btns(progress_msgbox);
                if (btns_obj)
                    lv_obj_set_style_text_font(btns_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_ITEMS | LV_STATE_DEFAULT);
                lv_obj_t *text_obj = lv_msgbox_get_text(progress_msgbox);
                if (text_obj)
                {
                    lv_obj_set_style_text_font(text_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(text_obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_width(text_obj, 240);
                    lv_label_set_long_mode(text_obj, LV_LABEL_LONG_WRAP);
                    lv_obj_set_style_text_align(text_obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                lv_obj_add_event_cb(progress_msgbox, progress_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
                lv_obj_add_event_cb(progress_msgbox, progress_msgbox_event_cb, LV_EVENT_DELETE, NULL);
                current_progress_msgbox = progress_msgbox;
                connection_completed = false;
                // 真实发起连接
                rt_wlan_connect(current_selected_network->ssid, password);
            }
        }
        else if (btn_id == 1) // "取消"
        {
            cleanup_keyboard();
            current_password_input = NULL;
            close_msgbox_object(obj);
        }
    }
    else if (code == LV_EVENT_DELETE)
    {
        cleanup_keyboard();
        current_password_input = NULL;
    }
}

// 键盘事件回调 - 处理键盘关闭
static void keyboard_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *kb = lv_event_get_target(e);

    if (code == LV_EVENT_CANCEL || code == LV_EVENT_READY)
    {
        // 用户按了取消或确定键，隐藏键盘
        cleanup_keyboard();
    }
}

// 密码输入对话框事件回调 - 处理重新唤起键盘
static void password_input_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED)
    {
        // 如果当前没有键盘或键盘已经被删除，重新创建键盘
        if (current_keyboard == NULL)
        {
            current_keyboard = lv_keyboard_create(lv_scr_act());
            lv_obj_set_size(current_keyboard, 320, 100);
            lv_obj_align(current_keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
            lv_keyboard_set_textarea(current_keyboard, ta);

            // 添加键盘事件回调
            lv_obj_add_event_cb(current_keyboard, keyboard_event_cb, LV_EVENT_CANCEL, NULL);
            lv_obj_add_event_cb(current_keyboard, keyboard_event_cb, LV_EVENT_READY, NULL);
        }
        else
        {
            // 如果键盘存在，重新关联到当前输入框
            lv_keyboard_set_textarea(current_keyboard, ta);
        }

        // 更新当前密码输入框的全局引用
        current_password_input = ta;
    }
}

// 列表项点击回调
static void wifi_list_item_click_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        // 获取被点击的网络索引
        uint32_t *index_ptr = (uint32_t *)lv_event_get_user_data(e);
        if (index_ptr == NULL || *index_ptr >= wifi_network_count)
            return;

        uint32_t index = *index_ptr;
        wifi_network_t *selected_network = &wifi_networks[index];

        current_selected_network = selected_network; // 更新当前选中网络

        if (selected_network->is_connected)
        {
            // 已连接的网络，显示简单消息
            static const char *btns[] = {"确定", ""};

            // 准备状态文本
            char msg_text[64];
            sprintf(msg_text, "已连接到 %s", selected_network->ssid);

            // 创建消息框时直接传入文本
            lv_obj_t *msgbox = lv_msgbox_create(lv_scr_act(), "WiFi状态", msg_text, btns, true);

            // 设置消息框大小
            lv_obj_set_size(msgbox, 280, 150);

            // 设置消息框标题字体
            lv_obj_t *title_obj = lv_msgbox_get_title(msgbox);
            if (title_obj)
            {
                lv_obj_set_style_text_font(title_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
            }

            // 设置按钮字体
            lv_obj_t *btns_obj = lv_msgbox_get_btns(msgbox);
            if (btns_obj)
            {
                lv_obj_set_style_text_font(btns_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_ITEMS | LV_STATE_DEFAULT);
            }

            // 获取并设置文本样式
            lv_obj_t *text_obj = lv_msgbox_get_text(msgbox);
            if (text_obj)
            {
                lv_obj_set_style_text_font(text_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT); // 思源宋体
                lv_obj_set_style_text_color(text_obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);              // 黑色文字
                lv_obj_set_width(text_obj, 240);                                                                             // 设置宽度
                lv_label_set_long_mode(text_obj, LV_LABEL_LONG_WRAP);                                                        // 允许换行
            }

            // 添加事件回调处理按钮点击
            lv_obj_add_event_cb(msgbox, connection_result_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

            lv_obj_center(msgbox);
        }
        else if (selected_network->is_secure)
        {
            // 首先清理任何现有的键盘
            cleanup_keyboard();

            // 需要密码的网络 - 创建完整的密码输入对话框与键盘
            static const char *btns[] = {"连接", "取消", ""};

            // 准备提示文本
            char msg_text[80];
            sprintf(msg_text, "请输入密码连接到\n%s", selected_network->ssid);

            // 创建消息框时直接传入文本
            lv_obj_t *msgbox = lv_msgbox_create(lv_scr_act(), "WiFi连接", msg_text, btns, true);

            // 设置消息框大小并大幅上移位置，避免键盘遮挡
            lv_obj_set_size(msgbox, 300, 160); // 进一步减小高度
            lv_obj_set_pos(msgbox, 10, -10);   // 大幅上移，甚至可以超出屏幕上边界一点

            // 设置消息框标题字体
            lv_obj_t *title_obj = lv_msgbox_get_title(msgbox);
            if (title_obj)
            {
                lv_obj_set_style_text_font(title_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
            }

            // 设置按钮字体
            lv_obj_t *btns_obj = lv_msgbox_get_btns(msgbox);
            if (btns_obj)
            {
                lv_obj_set_style_text_font(btns_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_ITEMS | LV_STATE_DEFAULT);
            }

            // 获取并设置文本样式
            lv_obj_t *text_obj = lv_msgbox_get_text(msgbox);
            if (text_obj)
            {
                lv_obj_set_style_text_font(text_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT); // 思源宋体
                lv_obj_set_style_text_color(text_obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);              // 黑色文字
                lv_obj_set_width(text_obj, 260);                                                                             // 设置宽度
                lv_label_set_long_mode(text_obj, LV_LABEL_LONG_WRAP);                                                        // 允许换行
                lv_obj_set_style_text_align(text_obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);                // 居中对齐
            }

            // 创建密码输入框 - 进一步调整位置
            lv_obj_t *ta = lv_textarea_create(msgbox);
            current_password_input = ta; // 保存密码输入框的全局引用
            lv_textarea_set_password_mode(ta, true);
            lv_textarea_set_placeholder_text(ta, "密码");
            lv_obj_set_width(ta, 260);
            lv_obj_set_height(ta, 35);                // 稍微减小高度
            lv_obj_align(ta, LV_ALIGN_CENTER, 0, 15); // 进一步上移输入框位置

            // 设置输入框字体和样式
            lv_obj_set_style_text_font(ta, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(ta, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ta, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(ta, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(ta, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(ta, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_all(ta, 6, LV_PART_MAIN | LV_STATE_DEFAULT); // 减小内边距

            // 添加密码输入框的点击事件回调 - 支持重新唤起键盘
            lv_obj_add_event_cb(ta, password_input_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_add_event_cb(ta, password_input_event_cb, LV_EVENT_FOCUSED, NULL);

            // 创建键盘并设置为当前键盘 - 使用底部对齐方式
            current_keyboard = lv_keyboard_create(lv_scr_act());
            lv_obj_set_size(current_keyboard, 320, 100);               // 键盘高度100px
            lv_obj_align(current_keyboard, LV_ALIGN_BOTTOM_MID, 0, 0); // 使用底部居中对齐
            lv_keyboard_set_textarea(current_keyboard, ta);            // 关联到文本输入框

            // 添加键盘事件回调
            lv_obj_add_event_cb(current_keyboard, keyboard_event_cb, LV_EVENT_CANCEL, NULL);
            lv_obj_add_event_cb(current_keyboard, keyboard_event_cb, LV_EVENT_READY, NULL);

            // 设置消息框事件回调
            lv_obj_add_event_cb(msgbox, password_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
            lv_obj_add_event_cb(msgbox, password_msgbox_event_cb, LV_EVENT_DELETE, NULL);

            // 不使用居中，直接使用设置的位置
        }
        else
        {
            // 开放网络，显示连接确认对话框
            static const char *btns[] = {"连接", "取消", ""};

            // 准备提示文本
            char msg_text[80];
            sprintf(msg_text, "连接到开放网络\n%s", selected_network->ssid);

            // 创建消息框
            lv_obj_t *msgbox = lv_msgbox_create(lv_scr_act(), "WiFi连接", msg_text, btns, true);

            // 设置消息框大小
            lv_obj_set_size(msgbox, 280, 150);
            lv_obj_center(msgbox);

            // 设置消息框标题字体
            lv_obj_t *title_obj = lv_msgbox_get_title(msgbox);
            if (title_obj)
                lv_obj_set_style_text_font(title_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT); // 思源宋体
            // 设置按钮字体
            lv_obj_t *btns_obj = lv_msgbox_get_btns(msgbox);
            if (btns_obj)
                lv_obj_set_style_text_font(btns_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_ITEMS | LV_STATE_DEFAULT); // 思源宋体
            // 获取并设置文本样式
            lv_obj_t *text_obj = lv_msgbox_get_text(msgbox);
            if (text_obj)
            {
                lv_obj_set_style_text_font(text_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT); // 思源宋体
                lv_obj_set_style_text_color(text_obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_width(text_obj, 240);
                lv_label_set_long_mode(text_obj, LV_LABEL_LONG_WRAP);
                lv_obj_set_style_text_align(text_obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            }

            // 添加事件回调处理按钮点击
            lv_obj_add_event_cb(msgbox, open_network_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
            lv_obj_add_event_cb(msgbox, open_network_msgbox_event_cb, LV_EVENT_DELETE, NULL);
        }
    }
}

// 刷新按钮回调
static void wifi_refresh_btn_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED)
    {
        // 调试输出
        rt_kprintf("[UI] wifi_refresh_btn_event_cb: 清空wifi_networks，开始扫描\n");
        // 每次刷新前清空列表
        if (wifi_networks)
        {
            rt_free(wifi_networks);
            wifi_networks = NULL;
        }
        wifi_network_count = 0;
        lv_ui *ui = (lv_ui *)lv_event_get_user_data(e);
        lv_obj_clean(ui->wifi_connect_list);
        create_wifi_list_items(ui);
        rt_wlan_scan(); // 真实扫描
    }
}

// 创建信号强度图标
static const char *get_signal_icon(int rssi)
{
    if (rssi > -60)
    { // 强信号
        return LV_SYMBOL_WIFI;
    }
    else if (rssi > -75)
    { // 中等信号
        return LV_SYMBOL_WIFI;
    }
    else
    { // 弱信号
        return LV_SYMBOL_WIFI;
    }
}

// 创建WiFi列表项
static void create_wifi_list_items(lv_ui *ui)
{
    static uint32_t network_indices[5]; // 存储索引的静态数组

    for (uint32_t i = 0; i < wifi_network_count; i++)
    {
        wifi_network_t *network = &wifi_networks[i];
        network_indices[i] = i;

        // 创建列表项按钮 - 增加高度以容纳更多信息
        lv_obj_t *item = lv_btn_create(ui->wifi_connect_list);
        lv_obj_set_width(item, 250); // 原270，整体更靠左
        lv_obj_set_height(item, 50); // 从35增加到50
        lv_obj_add_event_cb(item, wifi_list_item_click_event_cb, LV_EVENT_CLICKED, &network_indices[i]);

        // 设置按钮样式
        lv_obj_set_style_bg_color(item, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(item, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(item, lv_color_hex(0xdddddd), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(item, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_all(item, 8, LV_PART_MAIN | LV_STATE_DEFAULT); // 增加内边距

        // 创建SSID标签 - 使用更大的字体和明确的位置
        lv_obj_t *ssid_label = lv_label_create(item);
        lv_label_set_text(ssid_label, network->ssid);
        lv_obj_set_style_text_font(ssid_label, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT); // 思源宋体
        lv_obj_set_style_text_color(ssid_label, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);              // 黑色文字
        lv_obj_set_pos(ssid_label, 10, 2);                                                                             // 明确设置位置
        lv_obj_set_width(ssid_label, 150);                                                                             // 设置宽度确保显示
        lv_obj_set_style_text_font(ssid_label, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT); // 强制设置为思源宋体，确保中文显示

        // 创建信号强度图标
        lv_obj_t *signal_icon = lv_label_create(item);
        lv_label_set_text(signal_icon, get_signal_icon(network->rssi));
        lv_obj_set_style_text_font(signal_icon, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);    // 更大图标
        lv_obj_set_style_text_color(signal_icon, lv_color_hex(0x2196F3), LV_PART_MAIN | LV_STATE_DEFAULT); // 蓝色图标
        lv_obj_set_pos(signal_icon, 240, 5);                                                               // 明确位置

        // 创建连接状态标识（只在已连接时显示）
        if (network->is_connected)
        {
            lv_obj_t *connected_label = lv_label_create(item);
            lv_label_set_text(connected_label, "已连接");
            lv_obj_set_style_text_color(connected_label, lv_color_hex(0x4CAF50), LV_PART_MAIN | LV_STATE_DEFAULT);              // 绿色
            lv_obj_set_style_text_font(connected_label, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT); // 思源宋体
            lv_obj_set_pos(connected_label, 180, 8);
            lv_obj_set_style_text_font(connected_label, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT); // 强制设置为思源宋体，确保中文显示
        }

        // 创建信号强度文本
        char rssi_str[15];
        sprintf(rssi_str, "%ddBm", network->rssi);
        lv_obj_t *rssi_label = lv_label_create(item);
        lv_label_set_text(rssi_label, rssi_str);
        lv_obj_set_style_text_font(rssi_label, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT); // 思源宋体
        lv_obj_set_style_text_color(rssi_label, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT);              // 灰色
        lv_obj_set_pos(rssi_label, 10, 25);                                                                            // 放在SSID下方
        lv_obj_set_style_text_font(rssi_label, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT); // 强制设置为思源宋体，确保中文显示
    }
}

void setup_scr_wifi_connect(lv_ui *ui)
{
    // Write codes wifi_connect
    ui->wifi_connect = lv_obj_create(NULL);
    lv_obj_set_size(ui->wifi_connect, 320, 240);
    lv_obj_set_scrollbar_mode(ui->wifi_connect, LV_SCROLLBAR_MODE_OFF);

    // Write style for wifi_connect, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_connect, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes wifi_connect_cont_1
    ui->wifi_connect_cont_1 = lv_obj_create(ui->wifi_connect);
    lv_obj_set_pos(ui->wifi_connect_cont_1, 0, 0);
    lv_obj_set_size(ui->wifi_connect_cont_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->wifi_connect_cont_1, LV_SCROLLBAR_MODE_OFF);

    // Write style for wifi_connect_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->wifi_connect_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->wifi_connect_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_connect_cont_1, lv_color_hex(0xccffe0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_dither_mode(ui->wifi_connect_cont_1, LV_DITHER_ORDERED, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_connect_cont_1, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->wifi_connect_cont_1, lv_color_hex(0x4dffb8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->wifi_connect_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->wifi_connect_cont_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->wifi_connect_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->wifi_connect_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->wifi_connect_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->wifi_connect_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_connect_cont_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes wifi_connect_btn_1
    ui->wifi_connect_btn_1 = lv_btn_create(ui->wifi_connect);
    ui->wifi_connect_btn_1_label = lv_label_create(ui->wifi_connect_btn_1);
    lv_label_set_text(ui->wifi_connect_btn_1_label, "");
    lv_label_set_long_mode(ui->wifi_connect_btn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->wifi_connect_btn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->wifi_connect_btn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->wifi_connect_btn_1_label, LV_PCT(100));
    lv_obj_set_pos(ui->wifi_connect_btn_1, 5, 107);
    lv_obj_set_size(ui->wifi_connect_btn_1, 23, 23);

    // Write style for wifi_connect_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->wifi_connect_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->wifi_connect_btn_1, lv_color_hex(0xf9dfff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->wifi_connect_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_connect_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_btn_1, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->wifi_connect_btn_1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->wifi_connect_btn_1, lv_color_hex(0xb8a0bd), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->wifi_connect_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->wifi_connect_btn_1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_x(ui->wifi_connect_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(ui->wifi_connect_btn_1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_connect_btn_1, lv_color_hex(0x575757), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->wifi_connect_btn_1, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->wifi_connect_btn_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->wifi_connect_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // The custom code of wifi_connect.

    // 创建标题标签
    ui->wifi_connect_title_label = lv_label_create(ui->wifi_connect);
    lv_label_set_text(ui->wifi_connect_title_label, "可用WiFi网络");
    lv_obj_set_pos(ui->wifi_connect_title_label, 20, 10);
    lv_obj_set_size(ui->wifi_connect_title_label, 200, 20);
    lv_obj_set_style_text_font(ui->wifi_connect_title_label, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->wifi_connect_title_label, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 创建刷新按钮
    ui->wifi_connect_refresh_btn = lv_btn_create(ui->wifi_connect);
    lv_obj_set_pos(ui->wifi_connect_refresh_btn, 260, 8);
    lv_obj_set_size(ui->wifi_connect_refresh_btn, 40, 25);
    lv_obj_add_event_cb(ui->wifi_connect_refresh_btn, wifi_refresh_btn_event_cb, LV_EVENT_CLICKED, ui);

    ui->wifi_connect_refresh_btn_label = lv_label_create(ui->wifi_connect_refresh_btn);
    lv_label_set_text(ui->wifi_connect_refresh_btn_label, LV_SYMBOL_REFRESH);
    lv_obj_set_style_text_font(ui->wifi_connect_refresh_btn_label, &lv_font_iconfont_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui->wifi_connect_refresh_btn_label);

    // 设置刷新按钮样式
    lv_obj_set_style_bg_color(ui->wifi_connect_refresh_btn, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_connect_refresh_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_refresh_btn, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 创建WiFi列表容器
    ui->wifi_connect_list = lv_obj_create(ui->wifi_connect);
    lv_obj_set_pos(ui->wifi_connect_list, 30, 40);    // 将X坐标由20改为50，整体右移30像素
    lv_obj_set_size(ui->wifi_connect_list, 280, 185); // 宽度由280缩小为250，避免超出右侧
    lv_obj_set_scrollbar_mode(ui->wifi_connect_list, LV_SCROLLBAR_MODE_AUTO);

    // 设置列表容器样式
    lv_obj_set_style_bg_color(ui->wifi_connect_list, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->wifi_connect_list, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->wifi_connect_list, lv_color_hex(0xcccccc), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->wifi_connect_list, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->wifi_connect_list, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 设置列表布局为列布局
    lv_obj_set_flex_flow(ui->wifi_connect_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui->wifi_connect_list, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // 创建WiFi列表项
    create_wifi_list_items(ui);

    /* ========================================================================
     * RT-Thread WLAN 集成点 #7: 添加 RT-Thread WLAN 初始化和事件注册
     * ======================================================================== */
    // =================== RT-Thread WLAN 初始化替换 ===================
    // // 替换下方 TODO 为实际初始化：
    // rt_wlan_init();
    // register_wlan_events();
    // rt_wlan_start();
    // scan_and_update_wifi_list();
    // =================== END ===================

    // 初始化事件处理
    events_init_wifi_connect(ui);
    // 补充：确保事件注册，并调试输出
    rt_kprintf("[UI] setup_scr_wifi_connect: register_wlan_events called, ui=%p\n", ui);
    register_wlan_events(ui);
}

// 连接结果回调
static void connection_result_msgbox_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        lv_obj_t *msgbox = lv_event_get_target(e);

        // 获取按钮ID - 使用多种方法确保获取成功
        uint32_t btn_id = 0;
        bool btn_id_valid = false;

        // 方法1：从事件参数获取
        uint32_t *btn_id_ptr = (uint32_t *)lv_event_get_param(e);
        if (btn_id_ptr != NULL)
        {
            btn_id = *btn_id_ptr;
            btn_id_valid = true;
            printf("Got button ID from event param: %u\n", btn_id);
        }
        else
        {
            // 方法2：从按钮矩阵获取选中的按钮
            lv_obj_t *btns_obj = lv_msgbox_get_btns(msgbox);
            if (btns_obj != NULL)
            {
                btn_id = lv_btnmatrix_get_selected_btn(btns_obj);
                if (btn_id != LV_BTNMATRIX_BTN_NONE)
                {
                    btn_id_valid = true;
                    printf("Got button ID from btnmatrix: %u\n", btn_id);
                }
            }
        }

        // 如果无法获取按钮ID，强制关闭对话框
        if (!btn_id_valid)
        {
            printf("Failed to get button ID, force closing dialog\n");
            close_msgbox_object(msgbox);
            return;
        }

        // 获取对话框标题来判断是哪种类型的对话框
        lv_obj_t *title_obj = lv_msgbox_get_title(msgbox);
        const char *title_text = "";
        if (title_obj != NULL)
        {
            title_text = lv_label_get_text(title_obj);
        }

        printf("Button clicked: ID=%u, Title='%s'\n", btn_id, title_text);

        // 处理所有错误类型对话框的按钮（连接失败、连接超时等）
        if (strcmp(title_text, "连接失败") == 0 || strcmp(title_text, "连接超时") == 0)
        {
            printf("Processing error connection dialog: %s\n", title_text);
            // 直接关闭对话框，不需要重试功能
            close_msgbox_object(msgbox);
        }
        else
        {
            // 处理其他类型的对话框（成功、状态等）
            printf("Processing other dialog: %s, button: %u\n", title_text, btn_id);
            // 直接关闭对话框
            close_msgbox_object(msgbox);
        }
    }
    else if (code == LV_EVENT_DELETE)
    {
        // 对话框被删除时也要清理键盘和引用
        cleanup_keyboard();
        current_password_input = NULL;
    }
}

// 进度对话框事件回调
static void progress_msgbox_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        // 用户点击了"取消"按钮，停止连接
        if (connection_timer != NULL)
        {
            lv_timer_del(connection_timer);
            connection_timer = NULL;
        }
        if (timeout_timer != NULL)
        {
            lv_timer_del(timeout_timer);
            timeout_timer = NULL;
        }
        connection_completed = true; // 标记连接已完成（被取消）
        current_progress_msgbox = NULL;
        close_msgbox_object(lv_event_get_target(e)); // 使用统一的关闭函数
    }
    else if (code == LV_EVENT_DELETE)
    {
        current_progress_msgbox = NULL;
    }
}

// 开放网络消息框事件回调
static void open_network_msgbox_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        uint32_t *btn_id_ptr = (uint32_t *)lv_event_get_param(e);
        if (btn_id_ptr == NULL)
            return;
        uint32_t btn_id = *btn_id_ptr;
        if (btn_id == 0) // "连接"
        {
            close_msgbox_object(obj);
            if (current_selected_network != NULL)
            {
                static const char *progress_btns[] = {"取消", ""};
                char progress_text[100];
                sprintf(progress_text, "正在连接到 %s...\n", current_selected_network->ssid);
                lv_obj_t *progress_msgbox = lv_msgbox_create(lv_scr_act(), "WiFi连接", progress_text, progress_btns, true);
                lv_obj_set_size(progress_msgbox, 280, 150);
                lv_obj_center(progress_msgbox);
                // 设置弹窗所有字体为思源宋体，确保中文显示
                lv_obj_t *title_obj = lv_msgbox_get_title(progress_msgbox);
                if (title_obj)
                    lv_obj_set_style_text_font(title_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_t *btns_obj = lv_msgbox_get_btns(progress_msgbox);
                if (btns_obj)
                    lv_obj_set_style_text_font(btns_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_ITEMS | LV_STATE_DEFAULT);
                lv_obj_t *text_obj = lv_msgbox_get_text(progress_msgbox);
                if (text_obj)
                {
                    lv_obj_set_style_text_font(text_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(text_obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_width(text_obj, 240);
                    lv_label_set_long_mode(text_obj, LV_LABEL_LONG_WRAP);
                    lv_obj_set_style_text_align(text_obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                lv_obj_add_event_cb(progress_msgbox, progress_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
                lv_obj_add_event_cb(progress_msgbox, progress_msgbox_event_cb, LV_EVENT_DELETE, NULL);
                current_progress_msgbox = progress_msgbox;
                connection_completed = false;
                // 真实发起连接
                rt_wlan_connect(current_selected_network->ssid, NULL);
            }
        }
        else if (btn_id == 1) // "取消"
        {
            close_msgbox_object(obj);
        }
    }
    else if (code == LV_EVENT_DELETE)
    {
        cleanup_keyboard();
        current_password_input = NULL;
    }
}

// 声明SCAN_REPORT事件处理函数，避免未声明错误
static void wlan_scan_report_handler(int event, struct rt_wlan_buff *buff, void *parameter);

// 扫描结果回调，填充wifi_networks
//  SCAN_DONE事件回调
static void wlan_scan_done_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    rt_kprintf("[UI] SCAN_DONE: wifi_network_count=%d\n", wifi_network_count);
    // 获取当前连接信息
    struct rt_wlan_info current_info;
    int got_info = rt_wlan_get_info(&current_info);
    // 标记已连接热点
    if (wifi_networks && got_info == RT_EOK && current_info.ssid.len > 0)
    {
        for (uint16_t i = 0; i < wifi_network_count; i++)
            if (strncmp(wifi_networks[i].ssid, (char *)current_info.ssid.val, current_info.ssid.len) == 0)
                wifi_networks[i].is_connected = true;
            else
                wifi_networks[i].is_connected = false;
    }
    lv_ui *ui = (lv_ui *)parameter;
    lv_obj_clean(ui->wifi_connect_list);
    create_wifi_list_items(ui);
}

// 连接结果事件回调
static void wlan_connected_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    // 连接成功，关闭进度框，弹出成功提示
    if (current_progress_msgbox)
    {
        lv_msgbox_close_async(current_progress_msgbox);
        current_progress_msgbox = NULL;
    }
    
    static const char *success_btns[] = {""};
    char success_text[100];
    // 优先使用current_selected_network->ssid，如果为空则用当前连接的ssid
    const char *ssid_to_show = NULL;
    if (current_selected_network && current_selected_network->ssid[0] != '\0')
    {
        ssid_to_show = current_selected_network->ssid;
    }
    else
    {
        struct rt_wlan_info info;
        if (rt_wlan_get_info(&info) == RT_EOK && info.ssid.len > 0)
        {
            static char ssid_buf[33];
            strncpy(ssid_buf, (char *)info.ssid.val, info.ssid.len);
            ssid_buf[info.ssid.len] = '\0';
            ssid_to_show = ssid_buf;
        }
    }
    if (ssid_to_show)
        sprintf(success_text, "成功连接到 %s", ssid_to_show);
    else
        sprintf(success_text, "成功连接到 <未知SSID>");
    lv_obj_t *success_msgbox = lv_msgbox_create(lv_scr_act(), "WIFI连接成功", success_text, success_btns, true);
    lv_obj_set_size(success_msgbox, 280, 150);
    lv_obj_center(success_msgbox);
    // 设置所有弹窗字体为思源宋体，确保中文显示
    lv_obj_t *title_obj = lv_msgbox_get_title(success_msgbox);
    if (title_obj)
        lv_obj_set_style_text_font(title_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *btns_obj = lv_msgbox_get_btns(success_msgbox);
    if (btns_obj)
        lv_obj_set_style_text_font(btns_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_t *text_obj = lv_msgbox_get_text(success_msgbox);
    if (text_obj)
        lv_obj_set_style_text_font(text_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(success_msgbox, connection_result_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    // 刷新连接状态
    if (wifi_networks && current_selected_network)
    {
        bool found = false;
        for (uint16_t i = 0; i < wifi_network_count; i++)
        {
            wifi_networks[i].is_connected = false;
            if (strcmp(wifi_networks[i].ssid, current_selected_network->ssid) == 0)
            {
                wifi_networks[i].is_connected = true;
                found = true;
                rt_kprintf("成功连接到: %s\n", wifi_networks[i].ssid);
            }
        }
        // 如果current_selected_network未被赋值（自动连接场景），用当前连接的ssid
        if (!found)
        {
            struct rt_wlan_info info;
            if (rt_wlan_get_info(&info) == RT_EOK && info.ssid.len > 0)
            {
                char ssid_buf[33] = {0};
                strncpy(ssid_buf, (char *)info.ssid.val, info.ssid.len);
                ssid_buf[info.ssid.len] = '\0';
                sprintf(success_text, "成功连接到 %s", ssid_buf);
            }
        }
    }
    connection_completed = true;
    rt_kprintf("连接完成，状态: %s\n", connection_completed ? "成功" : "失败");
}

static void wlan_connect_fail_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    // 自动连接场景不弹窗
    struct rt_wlan_info info;
    int got_info = rt_wlan_get_info(&info);
    // 判断是否为自动连接场景（current_selected_network为空或ssid为空，且当前连接的ssid为目标WiFi）
    if ((current_selected_network == NULL || current_selected_network->ssid[0] == '\0') &&
        got_info == RT_EOK && info.ssid.len > 0 && strncmp((char *)info.ssid.val, "Passionate的Mate70Pro＋", info.ssid.len) == 0)
    {
        rt_kprintf("[UI] 自动连接失败，不弹窗\n");
        return;
    }
    // 连接失败，关闭进度框，弹出失败提示
    if (current_progress_msgbox)
    {
        lv_msgbox_close_async(current_progress_msgbox);
        current_progress_msgbox = NULL;
    }
    static const char *fail_btns[] = {""};
    char fail_text[100];
    sprintf(fail_text, "连接到 %s 失败\n请检查密码或信号强度", current_selected_network ? current_selected_network->ssid : "");
    lv_obj_t *fail_msgbox = lv_msgbox_create(lv_scr_act(), "WIFI连接失败", fail_text, fail_btns, true);
    lv_obj_set_size(fail_msgbox, 280, 150);
    lv_obj_center(fail_msgbox);
    // 设置所有弹窗字体为思源宋体，确保中文显示
    lv_obj_t *title_obj = lv_msgbox_get_title(fail_msgbox);
    if (title_obj)
        lv_obj_set_style_text_font(title_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *btns_obj = lv_msgbox_get_btns(fail_msgbox);
    if (btns_obj)
        lv_obj_set_style_text_font(btns_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_t *text_obj = lv_msgbox_get_text(fail_msgbox);
    if (text_obj)
        lv_obj_set_style_text_font(text_obj, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    connection_completed = true;
}

// 新增：处理每个AP的SCAN_REPORT事件
void wlan_disconnected_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    // 断开连接，刷新UI
    if (wifi_networks)
    {
        for (uint16_t i = 0; i < wifi_network_count; i++)
        {
            wifi_networks[i].is_connected = false;
        }
    }
    // 可弹窗提示断开
}

// 注册事件
void register_wlan_events(lv_ui *ui)
{
    rt_kprintf("[UI] register_wlan_events called, ui=%p\n", ui);
    // 注册SCAN_REPORT事件，动态添加AP到wifi_networks并刷新UI
    rt_wlan_register_event_handler(RT_WLAN_EVT_SCAN_REPORT, wlan_scan_report_handler, ui);
    rt_wlan_register_event_handler(RT_WLAN_EVT_SCAN_DONE, wlan_scan_done_handler, ui);
    rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED, wlan_connected_handler, NULL);
    rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED_FAIL, wlan_connect_fail_handler, NULL);
    // rt_wlan_register_event_handler(RT_WLAN_EVT_STA_DISCONNECTED, wlan_disconnected_handler, NULL);
}

// 新增：处理每个AP的SCAN_REPORT事件
void wlan_scan_report_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
    lv_ui *ui = (lv_ui *)parameter;
    struct rt_wlan_info *info = (struct rt_wlan_info *)buff->data;
    if (!info || info->ssid.len == 0)
        return;
    rt_kprintf("[UI] SCAN_REPORT: SSID=%s, RSSI=%d, SEC=%d\n", info->ssid.val, info->rssi, info->security);
    // 动态扩展wifi_networks
    wifi_networks = (wifi_network_t *)rt_realloc(wifi_networks, sizeof(wifi_network_t) * (wifi_network_count + 1));
    if (!wifi_networks)
    {
        rt_kprintf("[UI] wifi_networks realloc failed!\n");
        return;
    }
    wifi_network_t *network = &wifi_networks[wifi_network_count];
    strncpy(network->ssid, (char *)info->ssid.val, info->ssid.len);
    network->ssid[info->ssid.len] = '\0';
    network->rssi = info->rssi;
    network->is_secure = (info->security != SECURITY_OPEN);
    network->is_connected = false;
    wifi_network_count++;
    rt_kprintf("[UI] wifi_network_count now: %d\n", wifi_network_count);

    // 兼容drv_rw007自动连接逻辑：发现目标WiFi自动连接
    if (rt_wlan_is_connected() != 1)
    {
        if (strncmp((char *)info->ssid.val, "Passionate的Mate70Pro＋", info->ssid.len) == 0)
        {
            struct rt_wlan_info current_info;
            int got_info = rt_wlan_get_info(&current_info);
            // 未连接目标WiFi时自动连接
            if (got_info != RT_EOK || current_info.ssid.len == 0 || strncmp((char *)current_info.ssid.val, "Passionate的Mate70Pro＋", current_info.ssid.len) != 0)
            {
                rt_kprintf("[UI] SCAN_REPORT: 自动连接目标WiFi\n");
                rt_wlan_connect("Passionate的Mate70Pro＋", "20050424");
            }
        }
    }
}
