// SHT31温湿度传感器
//  该代码用于测试SHT31温湿度传感器，并将数据发送到IOT线程或Display线程。
//  该代码使用Adafruit SHT31库进行传感器操作，并通过RTduino框架实现多线程数据处理。

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "rtthread.h"
#include "RTduino.h"
#include "Config.h"

extern "C"
{
#include "rtthread.h"
#include "lcd_rtthread.h"
#include "D:\RT-ThreadStudio\workspace\Psoc6_spi\board\GUI\gui_guider.h"
}

static char disp_SHT_buffer[15];
// char temp1_SHT_buffer[10];
// char temp2_SHT_buffer[10];

#if defined IOT_THREAD_ENABLE

#define SHT_EVENT 1 << 0 // SHT采集数据

extern rt_event_t IOT_EVENT;              // IOT事件
rt_mailbox_t Temp_Humi_MailBox = RT_NULL; // 传递给IOT线程的消息邮箱

#endif

#if defined DIS_THREAD_ENABLE
rt_mailbox_t Temp_Humi_MailBox_Dis = RT_NULL; // 传递给Dis线程的消息邮箱
#endif

float T_H_Data[2] = {0};  ////数据收集
extern float Air_Data[3]; // 外部BMP温度数据数组

bool enableHeater = false;
uint8_t loopCnt = 0;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void SHT_setup()
{
    Serial.begin();

#if defined IOT_THREAD_ENABLE
    Temp_Humi_MailBox = rt_mb_create("T_H_MB", 10, RT_IPC_FLAG_PRIO);
    if (Temp_Humi_MailBox == RT_NULL)
    {
        Serial.println("MailBox_IOT Create Error");
    }
#endif

#if defined DIS_THREAD_ENABLE
    Temp_Humi_MailBox_Dis = rt_mb_create("T_H_Dis", 10, RT_IPC_FLAG_PRIO);
    if (Temp_Humi_MailBox_Dis == RT_NULL)
    {
        Serial.println("MailBox_Dis Create Error");
    }
#endif

    while (!Serial)
        delay(100); // will pause Zero, Leonardo, etc until serial console opens

    // Serial.println("SHT31 test");
    if (!sht31.begin(0x44))
    { // Set to 0x45 for alternate i2c addr
        Serial.println("Couldn't find SHT31");
        while (1)
            delay(1);
    }

    Serial.print("Heater Enabled State: ");
    if (sht31.isHeaterEnabled())
        Serial.println("ENABLED");
    else
        Serial.println("DISABLED");
}

rt_uint16_t SHT_index = 0; // 计数，降低刷新图标频率

void SHT_loop()
{
    SHT_index++; // 每次循环增加计数

    float t = (sht31.readTemperature() + Air_Data[0]) / 2 - 6; // 读取温度，平均与BMP温度
    float h = sht31.readHumidity();

    T_H_Data[0] = t;
    T_H_Data[1] = h;

#ifndef NO_USING_DATA_OUTPUT
    if (!isnan(t))
    { // check if 'is not a number'
      //        Serial.print("Temp *C = ");
      //        Serial.print(t);
      //        Serial.print("\t\t");
    }
    else
    {
        Serial.println("Failed to read temperature");
    }

    if (!isnan(h))
    { // check if 'is not a number'
      //        Serial.print("Hum. % = ");
      //        Serial.println(h);
    }
    else
    {
        Serial.println("Failed to read humidity");
    }

    // lvgl 显示数据
    rt_snprintf(disp_SHT_buffer, sizeof(disp_SHT_buffer), "%.2f°C", T_H_Data[0]);
    if (guider_ui.monitor_label_temperature_now != RT_NULL  && guider_ui.monitor_temperature_chart != RT_NULL && guider_ui.monitor_temperature_series != RT_NULL)
    {
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
        lv_label_set_text(guider_ui.monitor_label_temperature_now, disp_SHT_buffer);
        if (SHT_index == 60) // 每分钟更新图表
        {
            lv_chart_set_next_value(guider_ui.monitor_temperature_chart, (lv_chart_series_t *)guider_ui.monitor_temperature_series, (rt_int16_t)T_H_Data[0]); // 更新温度图表
            lv_chart_refresh(guider_ui.monitor_temperature_chart); 
            rt_kprintf("=========================================================Temperature chart updated\n");                                                                                           // 刷新温度图表显示
        }
        rt_mutex_release(lv_mutex); // 释放互斥锁
    }
    rt_snprintf(disp_SHT_buffer, sizeof(disp_SHT_buffer), "%.2f%%", T_H_Data[1]);
    if (guider_ui.monitor_label_humidity_now != RT_NULL  && guider_ui.monitor_humidity_chart != RT_NULL && guider_ui.monitor_humidity_series != RT_NULL)
    {
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
        lv_label_set_text(guider_ui.monitor_label_humidity_now, disp_SHT_buffer);
        if (SHT_index == 60)
        {
            lv_chart_set_next_value(guider_ui.monitor_humidity_chart, (lv_chart_series_t *)guider_ui.monitor_humidity_series, (rt_int16_t)T_H_Data[1]); // 更新湿度图表
            lv_chart_refresh(guider_ui.monitor_humidity_chart);                                                                                         // 刷新湿度图表显示
            SHT_index = 0;                                                                                                                              // 重置计数                                                                                                                                  // 重置索引
        }
        rt_mutex_release(lv_mutex); // 释放互斥锁
    }

    // rt_kprintf("T: %.2fC\n", t);
    //  sprintf(temp1_SHT_buffer, "T: %.2fC", t);
    //  sprintf(temp2_SHT_buffer, "H: %.2f%%", h);
    //  sprintf(disp_SHT_buffer, "%s %s", temp1_SHT_buffer, temp2_SHT_buffer);
    // rt_kprintf("H: %.2f%%\n", h);
    // LCD_ShowString(10, 210, 260, 20, 16, (rt_uint8_t *)disp_SHT_buffer, GREEN, BLACK);
#endif

#if defined IOT_THREAD_ENABLE
    // 当启动IOT线程的时候
    if (IOT_EVENT != RT_NULL)
    {
        // 触发事件，通知IOT线程来取
        if (rt_event_send(IOT_EVENT, SHT_EVENT) != RT_EOK)
        {
            Serial.println("Send EVENT Error\n");
        }
        // 使用消息队列，将数据传输给IOT线程
        if (rt_mb_send_wait(Temp_Humi_MailBox, (rt_ubase_t)&T_H_Data, 100) != RT_EOK)
        {
            Serial.println("Send EMail To IOT Error\n");
        }
    }
#endif

#if defined DIS_THREAD_ENABLE
    // 使用消息邮箱，将数据传输到Display线程
    if (rt_mb_send_wait(Temp_Humi_MailBox_Dis, (rt_ubase_t)&T_H_Data, 1000) != RT_EOK)
    {
        Serial.println("Send EMail To Display Error\n");
    }
#endif

    delay(800);
}

// RTduino线程初始化
static int rtduino_init(void)
{
    rtduino_sketch_loader_create("RT_SHT", SHT_setup, SHT_loop);
    return 0;
}
INIT_COMPONENT_EXPORT(rtduino_init);
