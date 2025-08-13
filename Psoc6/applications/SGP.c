// /*
//  * Copyright (c) 2020, RudyLo <luhuadong@163.com>
//  *
//  * SPDX-License-Identifier: Apache-2.0
//  *
//  * Change Logs:
//  * Date           Author       Notes
//  * 2020-02-22     luhuadong    the first version
//  */

// #include <rtthread.h>
// #include <rtdevice.h>
// #include <rtdbg.h>
// #include <board.h>
// #include "sgp30.h"
// #include <math.h>
// #include "gui_guider.h"
// #include <stdio.h>

// rt_sem_t sem_sgp = RT_NULL;

// #define SGP30_I2C_BUS_NAME "i2c1"
// #define SGP30_I2C_ADDRESS PKG_USING_SGP30_I2C_ADDRESS

//  extern float T_H_Data[2];
//  int tvoc = 0;
//  int eco2 = 0;

// static char display_buffer1[15];
// static char display_buffer2[15];

// static void read_tvoc_entry(void *args)
// {
//     rt_device_t tvoc_dev = RT_NULL;
//     struct rt_sensor_data sensor_data;

//     tvoc_dev = rt_device_find(args);
//     if (!tvoc_dev)
//     {
//         rt_kprintf("Can't find TVOC device.\n");
//         return;
//     }

//     if (rt_device_open(tvoc_dev, RT_DEVICE_FLAG_RDWR))
//     {
//         rt_kprintf("Open TVOC device failed.\n");
//         return;
//     }

//     while (1)
//     {
//         if (1 != rt_device_read(tvoc_dev, 0, &sensor_data, 1))
//         {
//             rt_kprintf("Read TVOC data failed.\n");
//             continue;
//         }
//         // rt_kprintf("[%d] TVOC: %d ppb\n", sensor_data.timestamp, sensor_data.data.tvoc);
//         tvoc = sensor_data.data.tvoc;
//         // lvgl 显示数据

//         rt_snprintf(display_buffer1, sizeof(display_buffer1), "%dppb", tvoc);
//         if (guider_ui.monitor_label_tvoc_now != RT_NULL)
//         {
//             rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
//             lv_label_set_text(guider_ui.monitor_label_tvoc_now, display_buffer1);
//             rt_mutex_release(lv_mutex); // 释放互斥锁
//         }

        

//         rt_kprintf("[tvoc]TVOC: %d ppb\n", tvoc);

//         rt_thread_mdelay(2000);
//     }

//     rt_device_close(tvoc_dev);
// }

// static void read_eco2_entry(void *args)
// {
//     rt_device_t eco2_dev = RT_NULL;
//     struct rt_sensor_data sensor_data;

//     eco2_dev = rt_device_find(args);
//     if (!eco2_dev)
//     {
//         rt_kprintf("Can't find eCO2 device.\n");
//         return;
//     }

//     if (rt_device_open(eco2_dev, RT_DEVICE_FLAG_RDWR))
//     {
//         rt_kprintf("Open eCO2 device failed.\n");
//         return;
//     }

//     while (1)
//     {
//         if (1 != rt_device_read(eco2_dev, 0, &sensor_data, 1))
//         {
//             rt_kprintf("Read eCO2 data failed.\n");
//             continue;
//         }

//         eco2 = sensor_data.data.eco2;
//         // lvgl 显示数据
//         rt_snprintf(display_buffer2, sizeof(display_buffer2), "%dppm", eco2);
//         if (guider_ui.monitor_label_co2_now != RT_NULL)
//         {
//             rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
//             lv_label_set_text(guider_ui.monitor_label_co2_now, display_buffer2);
//             rt_mutex_release(lv_mutex); // 释放互斥锁
//         }
//         // rt_kprintf("eCO2: %d ppm\n", eco2);

//         rt_thread_mdelay(2000);
//     }

//     rt_device_close(eco2_dev);
// }

// /*!
//  *  @brief  Set humidity to SGP30 sensor
//  */
// static void sgp30_set_humi(rt_uint32_t absolute_humidity)
// {
//     rt_device_t sgp30_dev = RT_NULL;

//     sgp30_dev = rt_device_find("tvoc_sg3");
//     if (!sgp30_dev)
//     {
//         rt_kprintf("Can't find SGP30 device.\n");
//         return;
//     }

//     if (rt_device_open(sgp30_dev, RT_DEVICE_FLAG_RDWR))
//     {
//         rt_kprintf("Open SGP30 device failed.\n");
//         return;
//     }

//     if (RT_EOK != rt_device_control(sgp30_dev, RT_SENSOR_CTRL_SET_HUMIDITY, (void *)absolute_humidity))
//     {
//         rt_kprintf("Set humidity failed.\n");
//     }
//     rt_kprintf("Set humidity %d finished.\n", absolute_humidity);

//     rt_device_close(sgp30_dev);
// }

// /**
//  * @brief  由温度和相对湿度计算绝对湿度，并设置给SGP30
//  * @param  T_H_Data[0] 温度(°C)，T_H_Data[1] 相对湿度(%RH)
//  */
// static void sgp30_set_humi_by_array(float T_H_Data[2])
// {
//     float temperature = T_H_Data[0];
//     float rel_humidity = T_H_Data[1];
//     // 计算绝对湿度（mg/m³）
//     float abs_humi = 216.7f * (rel_humidity / 100.0f) * (6.112f * expf(17.62f * temperature / (243.12f + temperature)) / (273.15f + temperature));
//     rt_uint32_t absolute_humidity = (rt_uint32_t)(abs_humi * 1000.0f + 0.5f); // 四舍五入
//     sgp30_set_humi(absolute_humidity);
// }

// static int sgp30_read_sample(void)
// {
//     rt_thread_mdelay(5000);
//     sgp30_set_humi_by_array(T_H_Data);
//     rt_kprintf("SGP30 humidity set based on T_H_Data: %f, %f\n", T_H_Data[0], T_H_Data[1]);
//     rt_thread_mdelay(15000);
//     rt_thread_t tvoc_thread, eco2_thread;

//     tvoc_thread = rt_thread_create("tvoc_th", read_tvoc_entry,
//                                    "tvoc_sg3", 2048,
//                                    RT_THREAD_PRIORITY_MAX / 2, 20);

//     eco2_thread = rt_thread_create("eco2_th", read_eco2_entry,
//                                    "eco2_sg3", 2048,
//                                    RT_THREAD_PRIORITY_MAX / 2, 20);

//     if (tvoc_thread)
//     {
//         rt_thread_startup(tvoc_thread);
//     }
//     else
//     {
//         LOG_E("Failed to create TVOC thread.\n");
//         return -RT_ERROR;
//     }

//     if (eco2_thread)
//     {
//         rt_thread_startup(eco2_thread);
//     }
//     else
//     {
//         LOG_E("Failed to create eCO2 thread.\n");
//         return -RT_ERROR;
//     }

//     rt_sem_release(sem_sgp); // 释放信号量，表示SGP30初始化完成

//     return RT_EOK;
// }

// //INIT_APP_EXPORT(sgp30_read_sample);

// static int rt_hw_sgp30_port(void)
// {
//     struct rt_sensor_config cfg;

//     cfg.intf.type = RT_SENSOR_INTF_I2C;
//     cfg.intf.dev_name = SGP30_I2C_BUS_NAME;
//     cfg.intf.user_data = (void *)SGP30_I2C_ADDRESS;
//     rt_hw_sgp30_init("sg3", &cfg);

//     sem_sgp = rt_sem_create("sem_sgp", 0, RT_IPC_FLAG_FIFO);
//     if (sem_sgp == RT_NULL)
//     {
//         LOG_E("Create sem_sgp failed.\n");
//         return -RT_ERROR;
//     }

//     return RT_EOK;
// }
// //INIT_COMPONENT_EXPORT(rt_hw_sgp30_port);
