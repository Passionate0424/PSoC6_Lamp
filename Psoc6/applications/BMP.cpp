// BMP_Test.cpp - BMP280 传感器数据采集与线程通信示例
//
// 本文件演示如何在 RT-Thread + RTduino 环境下，使用 Adafruit_BMP280 采集温度、气压和海拔数据，
// 并通过事件和消息队列与 IOT/Display 线程通信。
//
// 主要功能：
// 1. 初始化 BMP280 传感器
// 2. 采集温度、气压、海拔数据
// 3. 通过消息队列和事件与其他线程通信
// 4. 数据格式化输出

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include "RTduino.h"
#include "Config.h"

// C 语言头文件，包含 RT-Thread 相关接口和 LCD 显示接口
extern "C"
{
#include <rtthread.h>
#include "lcd_rtthread.h"
#include "D:\RT-ThreadStudio\workspace\Psoc6_spi\board\GUI\gui_guider.h"
}

// 显示用缓冲区
// char disp_BMP_buffer[40];
static char temp1_BMP_buffer[15];
// char temp2_BMP_buffer[15];
// char temp3_BMP_buffer[15];

#if defined IOT_THREAD_ENABLE

#define BMP_EVENT 1 << 1 // BMP采集数据事件位

extern rt_event_t IOT_EVENT;       // IOT事件集
rt_mq_t Air_Pressure_MQ = RT_NULL; // 传递给IOT线程的消息队列

#endif

#if defined DIS_THREAD_ENABLE

rt_mq_t Air_Pressure_MQ_Dis = RT_NULL; // 传递给Dis线程的消息队列

#endif

float Air_Data[3] = {0}; // 存储温度、气压、海拔



Adafruit_BMP280 bmp; // I2C 方式初始化 BMP280

// BMP280 初始化函数
void BMP_setup()
{
    Serial.begin();

#if defined IOT_THREAD_ENABLE
    // 创建 IOT 线程消息队列
    Air_Pressure_MQ = rt_mq_create("Air_Mq", 8, 10, RT_IPC_FLAG_PRIO);
    if (Air_Pressure_MQ == RT_NULL)
    {
        Serial.println("MessageQueue_IOT Create Error");
    }
#endif

#if defined DIS_THREAD_ENABLE
    // 创建 Display 线程消息队列
    Air_Pressure_MQ_Dis = rt_mq_create("Air_Mq_Dis", 8, 10, RT_IPC_FLAG_PRIO);
    if (Air_Pressure_MQ_Dis == RT_NULL)
    {
        Serial.println("MessageQueue_Dis Create Error");
    }
#endif

    while (!Serial)
        delay(100); // 等待串口就绪
    // Serial.println(F("BMP280 test"));
    unsigned status;
    status = bmp.begin();
    if (!status)
    {
        // 传感器初始化失败，死循环等待
        while (1)
            delay(10);
    }

    /* 默认配置 */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* 工作模式 */
                    Adafruit_BMP280::SAMPLING_X2,     /* 温度过采样 */
                    Adafruit_BMP280::SAMPLING_X16,    /* 气压过采样 */
                    Adafruit_BMP280::FILTER_X16,      /* 滤波 */
                    Adafruit_BMP280::STANDBY_MS_500); /* 待机时间 */
}

// BMP280 数据采集与线程通信主循环
void BMP_loop()
{
    Air_Data[0] = bmp.readTemperature();     // 读取温度
    Air_Data[1] = bmp.readPressure() / 100;  // 读取气压
    Air_Data[2] = bmp.readAltitude(1013.25); /* 读取海拔，1013.25为标准大气压 */

#ifndef NO_USING_DATA_OUTPUT

    

    /* 打印释放的内存地址 */
    // if (temp1_BMP_buffer != RT_NULL)
    // {
    //     rt_kprintf("temp1_BMP_buffer address: 0x%08X\n", (rt_uint32_t)temp1_BMP_buffer);
    // }
    // lvgl 显示数据
    rt_snprintf(temp1_BMP_buffer, sizeof(temp1_BMP_buffer), "%dhPa", (rt_int16_t)Air_Data[1]);
    if (guider_ui.monitor_label_pressure_now != RT_NULL)
    {
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
        lv_label_set_text(guider_ui.monitor_label_pressure_now, temp1_BMP_buffer);
        rt_mutex_release(lv_mutex); // 释放互斥锁
    }

    // 格式化数据到缓冲区
    // sprintf(temp1_BMP_buffer, "T: %.2fC", Air_Data[0]);
    // sprintf(temp2_BMP_buffer, "P: %.2fhPa", Air_Data[1] / 100);
    // sprintf(temp3_BMP_buffer, "A: %.2fm", Air_Data[2]);
    // sprintf(disp_BMP_buffer, "%s %s %s", temp1_BMP_buffer, temp2_BMP_buffer, temp3_BMP_buffer);
    // 可用于 LCD 显示
    // LCD_ShowString(10, 190, 280, 20, 16, (rt_uint8_t *)disp_BMP_buffer, GREEN, BLACK);
#endif

#if defined IOT_THREAD_ENABLE
    // 向 IOT 线程发送事件和消息队列
    if (IOT_EVENT != RT_NULL)
    {
        // 触发事件，通知IOT线程来取
        if (rt_event_send(IOT_EVENT, BMP_EVENT) != RT_EOK)
        {
            Serial.println("Air_P Event Send Error");
        }
        // 使用消息队列，将数据传输给IOT线程
        if (rt_mq_send_wait(Air_Pressure_MQ, &Air_Data, 8, 100) != RT_EOK)
        {
            Serial.println("Air_P MQ Send To IOT Error");
        }
    }
#endif

#if defined DIS_THREAD_ENABLE
    // 使用消息队列，将数据传输到Display线程
    if (rt_mq_send_wait(Air_Pressure_MQ_Dis, &Air_Data, 8, 1000) != RT_EOK)
    {
        Serial.println("Air_P MQ Send To Dis Error");
    }
#endif

    delay(1000); // 采集周期 1s
}

// 初始化RTduino线程，注册 BMP_setup/BMP_loop
static int rtduino_init(void)
{
    rtduino_sketch_loader_create("RT_BMP", BMP_setup, BMP_loop);
    return 0;
}
INIT_APP_EXPORT(rtduino_init);
