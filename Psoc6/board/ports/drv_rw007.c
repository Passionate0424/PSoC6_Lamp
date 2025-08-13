#include <rtthread.h>
#include <rtdbg.h>
#ifdef BSP_USING_RW007
#include <rtdevice.h>
#include <drv_spi.h>
#include <board.h>
#include <spi_wifi_rw007.h>
#include "gui_guider.h"

rt_thread_t periodic_scan_thread = RT_NULL;
#include <wlan_dev.h>
#include <wlan_cfg.h>
#include <wlan_mgnt.h>
#include <wlan_prot.h>
#include <wlan_workqueue.h>
#include "drv_rw007.h"

rt_sem_t scan_success_sem = RT_NULL;
rt_sem_t scan_fail_sem = RT_NULL;
rt_sem_t net_ready = RT_NULL;

static beijing_time_t my_time;

const char *password = "20050424";
const char *ssid = "Passionate的Mate70Pro＋";

static void wifi_ready_callback(int event, struct rt_wlan_buff *buff, void *parameter)
{
    // 回调：WiFi连接状态变化
    rt_kprintf("%s\n", __FUNCTION__);
    if (rt_wlan_is_connected())
    {
        rt_kprintf("Connected to AP: %s\n", ssid); // 只打印 ssid
    }
    else
    {
        rt_kprintf("Failed to connect to AP: %s\n", ssid); // 只打印 ssid
    }
    // 同步网络时间到RTC
    // ntp_sync_to_rtc(RT_NULL);
    // 获取 UTC 时间并转换为北京时间
    time_t utc_time = ntp_get_time(RT_NULL);
    static struct tm beijing_tm;
    if (utc_time > 0)
    {
        // 不要手动加8小时，直接转换
        localtime_r(&utc_time, &beijing_tm);
        my_time.year = beijing_tm.tm_year + 1900;
        my_time.month = beijing_tm.tm_mon + 1;
        my_time.day = beijing_tm.tm_mday;
        my_time.hour = beijing_tm.tm_hour;
        my_time.min = beijing_tm.tm_min;
        my_time.sec = beijing_tm.tm_sec;
        rt_kprintf("Beijing time: %04d-%02d-%02d %02d:%02d:%02d\n",
                   my_time.year, my_time.month, my_time.day,
                   my_time.hour, my_time.min, my_time.sec);

        // 设置数字时钟的时间
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
        // lv_dclock_set_text_fmt(guider_ui.home_digital_clock_1, "%d:%02d:%02d",
        //                        my_time.hour, my_time.min, my_time.sec);
        home_digital_clock_1_min_value = my_time.min;
        home_digital_clock_1_hour_value = my_time.hour;
        home_digital_clock_1_sec_value = my_time.sec;
        rt_mutex_release(lv_mutex);

        // 设置数字时钟的时间
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
        // lv_dclock_set_text_fmt(guider_ui.monitor_digital_clock_1, "%d:%02d:%02d",
        //                        my_time.hour, my_time.min, my_time.sec);
        monitor_digital_clock_1_min_value = my_time.min;
        monitor_digital_clock_1_hour_value = my_time.hour;
        monitor_digital_clock_1_sec_value = my_time.sec;
        rt_mutex_release(lv_mutex);

        // 设置数字时钟的时间
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
        // lv_dclock_set_text_fmt(guider_ui.settings_digital_clock_1, "%d:%02d:%02d",
        //                        my_time.hour, my_time.min, my_time.sec);
        settings_digital_clock_1_min_value = my_time.min;
        settings_digital_clock_1_hour_value = my_time.hour;
        settings_digital_clock_1_sec_value = my_time.sec;
        rt_mutex_release(lv_mutex);

        // 设置日期
        static char calendar_date[16];
        rt_snprintf(calendar_date, sizeof(calendar_date), "%04d/%02d/%02d", my_time.year, my_time.month, my_time.day);
        if (guider_ui.home_datetext_1 != RT_NULL)
        {
            rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
            lv_label_set_text(guider_ui.home_datetext_1, calendar_date);
            rt_mutex_release(lv_mutex);
        }
        if (guider_ui.monitor_datetext_1 != RT_NULL)
        {
            rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
            lv_label_set_text(guider_ui.monitor_datetext_1, calendar_date);
            rt_mutex_release(lv_mutex);
        }
        if (guider_ui.settings_datetext_1 != RT_NULL)
        {
            rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
            lv_label_set_text(guider_ui.settings_datetext_1, calendar_date);
            rt_mutex_release(lv_mutex);
        }
    }
    else
    {
        rt_kprintf("获取网络时间失败\n");
    }
    rt_sem_release(net_ready);
}

static void wifi_connect_callback(int event, struct rt_wlan_buff *buff, void *parameter)
{
    rt_kprintf("%s\n", __FUNCTION__);
    if ((buff != RT_NULL) && (buff->len == sizeof(struct rt_wlan_info)))
    {
        rt_kprintf("ssid : %s \n", ((struct rt_wlan_info *)buff->data)->ssid.val);
    }
}

static void wifi_disconnect_callback(int event, struct rt_wlan_buff *buff, void *parameter)
{
    rt_kprintf("%s\n", __FUNCTION__);
    if ((buff != RT_NULL) && (buff->len == sizeof(struct rt_wlan_info)))
    {
        rt_kprintf("ssid : %s \n", ((struct rt_wlan_info *)buff->data)->ssid.val);
    }
    rt_sem_release(scan_fail_sem); /* 释放扫描失败信号量 */
}

static void wifi_connect_fail_callback(int event, struct rt_wlan_buff *buff, void *parameter)
{
    rt_kprintf("%s\n", __FUNCTION__);
    if ((buff != RT_NULL) && (buff->len == sizeof(struct rt_wlan_info)))
    {
        rt_kprintf("ssid : %s \n", ((struct rt_wlan_info *)buff->data)->ssid.val);
    }
    rt_sem_release(scan_fail_sem); /* 释放扫描失败信号量 */
}

static void wifi_scan_success_callback(int event, struct rt_wlan_buff *buff, void *parameter)
{
    // 回调：WiFi扫描成功
    rt_kprintf("%s\n", __FUNCTION__);
    if (buff != RT_NULL)
    {
        struct rt_wlan_info *result = (struct rt_wlan_info *)buff->data;
        if (result->ssid.len > 0)
        {
            rt_kprintf("Scan result: SSID: %s, BSSID: %02x:%02x:%02x:%02x:%02x:%02x\n",
                       result->ssid.val,
                       result->bssid[0],
                       result->bssid[1],
                       result->bssid[2],
                       result->bssid[3],
                       result->bssid[4],
                       result->bssid[5]);
            //            if (result->ssid.len == strlen(ssid) && memcmp(result->ssid.val, ssid, result->ssid.len) != 0)
            //            {
            //
            //                rt_kprintf("Failed to cmp SSID.\n");
            //                return;
            //            }
            // 下面为正确版本
            if (result->ssid.len != strlen(ssid) || memcmp(result->ssid.val, ssid, result->ssid.len) != 0)
            {

                rt_kprintf("Failed to cmp SSID.\n");
                return;
            }
            if (rt_wlan_is_connected() != 1)
            {
                if (rt_wlan_connect_adv(result, password) != RT_EOK) /* 尝试连接 */
                {
                    rt_kprintf("Connection attempt failed.\n");
                }
            }

            // rt_sem_release(scan_success_sem); /* 释放扫描成功信号量 */
        }
        else
        {
            rt_kprintf("Scan result: No valid SSID found.\n");
        }
    }
}

// 上电后自动连接线程入口
void wifi_auto_connect_entry(void *p)
{
    // 自动连接线程入口
    rt_thread_mdelay(100);
    struct rt_wlan_info info;
    INVALID_INFO(&info);   /* 初始化 info */
    SSID_SET(&info, ssid); /* 指定 SSID */
    rt_int8_t i = 3;
    rt_err_t err;
    while (i > 0)
    {
        err = rt_wlan_scan_with_info(&info); /* 开始同步扫描 */
        if (err == RT_EOK)
        {
        }
        else
        {
            rt_kprintf("wifi_auto_connect_entry:Scan error: %d\n", err);
        }
        rt_thread_mdelay(20000);
        if (rt_wlan_is_connected()) /* 尝试获取扫描成功信号量 */
        {
            goto end; /* 如果获取成功，跳转到结束 */
        }
        i--;
        rt_thread_mdelay(1000); /* 等待一秒后重试 */
    }
    rt_sem_release(scan_fail_sem); /* 释放扫描失败信号量 */

end: /* 如果获取成功，跳转到结束 */
    return;
}

// 连接失败或者未找到时进入周期性扫描线程
void periodic_scan_entry(void *p)
{
    while (1)
    {
        rt_sem_take(scan_fail_sem, RT_WAITING_FOREVER); /* 等待初始化扫描失败信号量 */
        if (rt_wlan_is_connected() != 1)
        {
            while (1)
            {
                rt_kprintf("Starting periodic scan...\n");
                struct rt_wlan_info info; /* 初始化 info */
                INVALID_INFO(&info);      /* 初始化 info */
                SSID_SET(&info, ssid);    /* 指定 SSID */
                rt_err_t err = rt_wlan_scan_with_info(&info);
                if (err == RT_EOK)
                {
                }
                else
                {
                    rt_kprintf("periodic_scan_entry:Scan error: %d\n", err);
                }

                rt_thread_mdelay(20000);
                if (rt_wlan_is_connected()) /* 尝试获取扫描成功信号量 */
                {
                    break;
                }

                rt_thread_mdelay(60000); /* 每60秒扫描一次 */
            }
        }
    }
}

extern void spi_wifi_isr(int vector);

static void rw007_gpio_init(void)
{
    /* Configure IO */
    rt_pin_mode(IFX_RW007_RST_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(IFX_RW007_INT_BUSY_PIN, PIN_MODE_INPUT_PULLDOWN);

    /* Reset rw007 and config mode */
    rt_pin_write(IFX_RW007_RST_PIN, PIN_LOW);
    rt_thread_delay(rt_tick_from_millisecond(100));
    rt_pin_write(IFX_RW007_RST_PIN, PIN_HIGH);

    /* Wait rw007 ready(exit busy stat) */
    while (!rt_pin_read(IFX_RW007_INT_BUSY_PIN))
    {
        rt_thread_delay(5);
    }

    rt_thread_delay(rt_tick_from_millisecond(200));
    rt_pin_mode(IFX_RW007_INT_BUSY_PIN, PIN_MODE_INPUT_PULLUP);
}

static struct rt_spi_device rw007_dev;

int wifi_spi_device_init(void)
{
    rt_thread_mdelay(500);
    cyhal_gpio_free(GET_PIN(11, 2));

    char sn_version[32];
    uint32_t cs_pin = IFX_RW007_CS_PIN;

    rw007_gpio_init();
    rt_hw_spi_device_attach(IFX_RW007_SPI_BUS_NAME, "wspi", cs_pin);
    rt_hw_wifi_init("wspi");

    rt_wlan_set_mode(RT_WLAN_DEVICE_STA_NAME, RT_WLAN_STATION);
    rt_wlan_set_mode(RT_WLAN_DEVICE_AP_NAME, RT_WLAN_AP);

    rw007_sn_get(sn_version);
    rt_kprintf("\nrw007  sn: [%s]\n", sn_version);
    rw007_version_get(sn_version);
    rt_kprintf("rw007 ver: [%s]\n\n", sn_version);

    rt_wlan_register_event_handler(RT_WLAN_EVT_READY, wifi_ready_callback, RT_NULL);
    // rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED, wifi_connect_callback, RT_NULL);
    // rt_wlan_register_event_handler(RT_WLAN_EVT_STA_DISCONNECTED, wifi_disconnect_callback, RT_NULL);
    // rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED_FAIL, wifi_connect_fail_callback, RT_NULL);
    // 确认驱动层的SCAN_REPORT事件注册已被注释，避免覆盖UI层注册
    // rt_wlan_register_event_handler(RT_WLAN_EVT_SCAN_REPORT, wifi_scan_success_callback, RT_NULL);

    // wifi启动自动连接线程
    rt_thread_t wifi_auto_connect_thread = rt_thread_create("wifi_auto_connect",
                                                            wifi_auto_connect_entry,
                                                            RT_NULL,
                                                            1024,
                                                            19,
                                                            10);

    // 扫描成功信号量
    scan_success_sem = rt_sem_create("scan_success", 0, RT_IPC_FLAG_FIFO);
    if (scan_success_sem == RT_NULL)
    {
        LOG_E("Failed to create  scan success semaphore");
        return -1;
    }

    // 扫描失败信号量，启动周期性扫描线程
    scan_fail_sem = rt_sem_create("scan_fail", 0, RT_IPC_FLAG_FIFO);
    if (scan_fail_sem == RT_NULL)
    {
        LOG_E("Failed to create scan fail semaphore");
        return -1;
    }

    // 网络就绪信号量
    net_ready = rt_sem_create("net_ready", 0, RT_IPC_FLAG_FIFO);
    if (net_ready == RT_NULL)
    {
        LOG_E("Failed to create net ready semaphore");
        return -1;
    }
    // 周期性扫描线程
    periodic_scan_thread = rt_thread_create("periodic_scan",
                                            periodic_scan_entry,
                                            RT_NULL,
                                            1024,
                                            20,
                                            10);
    if (periodic_scan_thread != RT_NULL)
    {
        rt_thread_startup(periodic_scan_thread);
    }
    else
    {
        LOG_E("Failed to create periodic scan thread");
        return -1;
    }

    if (wifi_auto_connect_thread != RT_NULL)
    {
        rt_thread_startup(wifi_auto_connect_thread);
    }
    else
    {
        LOG_E("Failed to create wifi auto connect thread");
        return -1;
    }
    //    rt_wlan_config_autoreconnect(RT_TRUE); /* 启用自动重连 */
    //    rt_kprintf("%d\n", rt_wlan_get_autoreconnect_mode());

    return 0;
}
INIT_APP_EXPORT(wifi_spi_device_init);

static void int_wifi_irq(void *p)
{
    ((void)p);
    spi_wifi_isr(0);
}

void spi_wifi_hw_init(void)
{
    rt_pin_attach_irq(IFX_RW007_INT_BUSY_PIN, PIN_IRQ_MODE_FALLING, int_wifi_irq, 0);
    rt_pin_irq_enable(IFX_RW007_INT_BUSY_PIN, RT_TRUE);
}

#endif /* BSP_USING_RW007 */
