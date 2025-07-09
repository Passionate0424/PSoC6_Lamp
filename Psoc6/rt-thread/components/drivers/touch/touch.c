/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-20     tyustli      the first version
 */

#include <rtdevice.h>
#include <string.h>
#include "gt911.h"
#include "lcd_rtthread.h"

#define DBG_TAG "touch"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>
#include <rtdef.h>

// lvgl适配开关
#define RT_TOUCH_LVGL_ADAPT 1 // 0关闭 1开启

static rt_sem_t touch_sem = RT_NULL;
static rt_thread_t gt911_thread = RT_NULL;
static rt_device_t touch_dev = RT_NULL;
rt_base_t level;

#ifdef RT_TOUCH_PIN_IRQ
static void touch_irq_callback(void *param)
{
    rt_device_control(touch_dev, RT_TOUCH_CTRL_DISABLE_INT, RT_NULL);
    // rt_kprintf("callback\n");
    rt_hw_touch_isr((rt_touch_t)param);
}
#endif

// 中断服务函数
static void touch_callback(void *param)
{

    // rt_kprintf("111\n");

    struct rt_touch_data *read_data;
    read_data = (struct rt_touch_data *)rt_malloc(sizeof(struct rt_touch_data) * 6);
    while (1)
    {

        rt_sem_take(touch_sem, RT_WAITING_FOREVER);
        rt_uint8_t write_buf[3];
        rt_device_open(touch_dev, RT_DEVICE_FLAG_INT_RX);

        rt_thread_delay(5);

        rt_uint8_t point_num = rt_device_read(touch_dev, 0, read_data, 5);

        if (point_num > 0)
        {
            for (rt_uint8_t i = 0; i < point_num; i++)
            {
                if (read_data[i].x_coordinate > 0 && read_data[i].x_coordinate < 240 && read_data[i].y_coordinate > 0 && read_data[i].y_coordinate < 320)
                    rt_kprintf("[GT911] Point %d: ID=%d, X=%4d, Y=%4d, Width=%3d, Event=%s\n",
                               i + 1,
                               read_data[i].track_id,
                               read_data[i].x_coordinate,
                               read_data[i].y_coordinate,
                               read_data[i].width,
                               read_data[i].event == RT_TOUCH_EVENT_DOWN ? "DOWN" : read_data[i].event == RT_TOUCH_EVENT_MOVE ? "MOVE"
                                                                                : read_data[i].event == RT_TOUCH_EVENT_UP     ? "UP"
                                                                                                                              : "NONE");
                LCD_DrawPoint(read_data[i].x_coordinate, read_data[i].y_coordinate, RED);
            }
        }
        write_buf[0] = (rt_uint8_t)((GT911_READ_STATUS >> 8) & 0xFF);
        write_buf[1] = (rt_uint8_t)(GT911_READ_STATUS & 0xFF);
        write_buf[2] = 0x00;
        gt911_write_reg(&gt911_client, write_buf, 3);
        rt_device_control(touch_dev, RT_TOUCH_CTRL_ENABLE_INT, RT_NULL);
        // rt_kprintf("int!!!");
    }
}

// 中断服务函数
rt_err_t touch_handler(rt_device_t dev, rt_size_t size)
{
    // rt_kprintf("222");
    rt_sem_release(touch_sem);
    return 0;
}

/* ISR for touch interrupt */
void rt_hw_touch_isr(rt_touch_t touch)
{
    RT_ASSERT(touch);
    if (touch->parent.rx_indicate == RT_NULL)
    {
        return;
    }

    if (touch->irq_handle != RT_NULL)
    {
        touch->irq_handle(touch);
    }

    touch->parent.rx_indicate(&touch->parent, 1);
}

/* touch interrupt initialization function */
static rt_err_t rt_touch_irq_init(rt_touch_t touch)
{
#ifdef RT_TOUCH_PIN_IRQ
    if (touch->config.irq_pin.pin == PIN_IRQ_PIN_NONE)
    {
        // rt_kprintf("PIN_IRQ_PIN_NONE\n");
        return -RT_EINVAL;
    }

    rt_pin_mode(touch->config.irq_pin.pin, touch->config.irq_pin.mode);

    if (touch->config.irq_pin.mode == PIN_MODE_INPUT_PULLDOWN)
    {
        // rt_kprintf("INPUT_PULLDOWN\n");
        rt_pin_attach_irq(touch->config.irq_pin.pin, PIN_IRQ_MODE_RISING, touch_irq_callback, (void *)touch);
    }
    else if (touch->config.irq_pin.mode == PIN_MODE_INPUT_PULLUP)
    {
        // rt_kprintf("INPUT_PULLUP\n");
        rt_pin_attach_irq(touch->config.irq_pin.pin, PIN_IRQ_MODE_FALLING, touch_irq_callback, (void *)touch);
    }
    else if (touch->config.irq_pin.mode == PIN_MODE_INPUT)
    {
        // rt_kprintf("INPUT\n");
        rt_pin_attach_irq(touch->config.irq_pin.pin, PIN_IRQ_MODE_RISING_FALLING, touch_irq_callback, (void *)touch);
    }
    if (rt_pin_irq_enable(touch->config.irq_pin.pin, PIN_IRQ_ENABLE) != RT_EOK)
    {
        LOG_E("rt touch pin irq init enable error %d\n", rt_pin_irq_enable(touch->config.irq_pin.pin, PIN_IRQ_ENABLE));
    }
    else
    {
        // rt_kprintf("pin irq enable success\n");
    }

#endif
    return RT_EOK;
}

/* touch interrupt enable */
static void rt_touch_irq_enable(rt_touch_t touch)
{
#ifdef RT_TOUCH_PIN_IRQ
    if (touch->config.irq_pin.pin != PIN_IRQ_PIN_NONE)
    {
        rt_pin_irq_enable(touch->config.irq_pin.pin, RT_TRUE);
    }
#else
    touch->ops->touch_control(touch, RT_TOUCH_CTRL_ENABLE_INT, RT_NULL);
#endif
}

/* touch interrupt disable */
static void rt_touch_irq_disable(rt_touch_t touch)
{
#ifdef RT_TOUCH_PIN_IRQ
    if (touch->config.irq_pin.pin != PIN_IRQ_PIN_NONE)
    {
        rt_pin_irq_enable(touch->config.irq_pin.pin, RT_FALSE);
    }
#else
    touch->ops->touch_control(touch, RT_TOUCH_CTRL_DISABLE_INT, RT_NULL);
#endif
}

static rt_err_t rt_touch_open(rt_device_t dev, rt_uint16_t oflag)
{
    rt_touch_t touch;
    RT_ASSERT(dev != RT_NULL);
    touch = (rt_touch_t)dev;

    if (oflag & RT_DEVICE_FLAG_INT_RX && dev->flag & RT_DEVICE_FLAG_INT_RX)
    {
        /* Initialization touch interrupt */
        rt_touch_irq_init(touch);
    }

    return RT_EOK;
}

static rt_err_t rt_touch_close(rt_device_t dev)
{
    rt_touch_t touch;
    RT_ASSERT(dev != RT_NULL);
    touch = (rt_touch_t)dev;

    /* touch disable interrupt */
    rt_touch_irq_disable(touch);

    return RT_EOK;
}

static rt_ssize_t rt_touch_read(rt_device_t dev, rt_off_t pos, void *buf, rt_size_t len)
{
    rt_touch_t touch;
    rt_size_t result = 0;
    RT_ASSERT(dev != RT_NULL);
    touch = (rt_touch_t)dev;

    if (buf == NULL || len == 0)
    {
        return 0;
    }

    result = touch->ops->touch_readpoint(touch, buf, len);

    return result;
}

static rt_err_t rt_touch_control(rt_device_t dev, int cmd, void *args)
{
    rt_touch_t touch;
    rt_err_t result = RT_EOK;
    RT_ASSERT(dev != RT_NULL);
    touch = (rt_touch_t)dev;

    switch (cmd)
    {
    case RT_TOUCH_CTRL_SET_MODE:
        result = touch->ops->touch_control(touch, RT_TOUCH_CTRL_SET_MODE, args);

        if (result == RT_EOK)
        {
            rt_uint16_t mode;
            mode = *(rt_uint16_t *)args;
            if (mode == RT_DEVICE_FLAG_INT_RX)
            {
                rt_touch_irq_enable(touch); /* enable interrupt */
            }
        }

        break;
    case RT_TOUCH_CTRL_SET_X_RANGE:
        result = touch->ops->touch_control(touch, RT_TOUCH_CTRL_SET_X_RANGE, args);

        if (result == RT_EOK)
        {
            touch->info.range_x = *(rt_int32_t *)args;
            LOG_D("set x coordinate range :%d\n", touch->info.range_x);
        }

        break;
    case RT_TOUCH_CTRL_SET_Y_RANGE:
        result = touch->ops->touch_control(touch, RT_TOUCH_CTRL_SET_Y_RANGE, args);

        if (result == RT_EOK)
        {
            touch->info.range_y = *(rt_uint32_t *)args;
            LOG_D("set y coordinate range :%d \n", touch->info.range_x);
        }

        break;
    case RT_TOUCH_CTRL_DISABLE_INT:
        rt_touch_irq_disable(touch);
        break;
    case RT_TOUCH_CTRL_ENABLE_INT:
        rt_touch_irq_enable(touch);
        break;

    case RT_TOUCH_CTRL_GET_ID:
    case RT_TOUCH_CTRL_GET_INFO:
    default:
        return touch->ops->touch_control(touch, cmd, args);
    }

    return result;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops rt_touch_ops =
    {
        RT_NULL,
        rt_touch_open,
        rt_touch_close,
        rt_touch_read,
        RT_NULL,
        rt_touch_control};
#endif

/*
 * touch register
 */
int rt_hw_touch_register(rt_touch_t touch,
                         const char *name,
                         rt_uint32_t flag,
                         void *data)
{
    rt_err_t result;
    rt_device_t device;
    RT_ASSERT(touch != RT_NULL);

    device = &touch->parent;

#ifdef RT_USING_DEVICE_OPS
    device->ops = &rt_touch_ops;
#else
    device->init = RT_NULL;
    device->open = rt_touch_open;
    device->close = rt_touch_close;
    device->read = rt_touch_read;
    device->write = RT_NULL;
    device->control = rt_touch_control;
#endif
    device->type = RT_Device_Class_Touch;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;
    device->user_data = data;

    result = rt_device_register(device, name, flag | RT_DEVICE_FLAG_STANDALONE);

    if (result != RT_EOK)
    {
        LOG_E("rt_touch register err code: %d", result);
        return result;
    }

    LOG_I("rt_touch init success");

    return RT_EOK;
}

static void touch(void *param)
{

    struct rt_touch_data data[GT911_MAX_TOUCH];
    rt_size_t point_num;

    static rt_uint32_t touch_count = 0;

    /* 查找触摸设备 */
    touch_dev = rt_device_find("gt911");
    if (touch_dev == RT_NULL)
    {
        rt_kprintf("[GT911] Error: Touch device not found!\n");
    }

    /* 配置触摸设备 */
    //    if (rt_touch_control(touch_dev, RT_TOUCH_CTRL_SET_MODE, (void *)RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    //    {
    //        rt_kprintf("[GT911] Error: Set int failed!\n");
    //    }

    // 信号量
    touch_sem = rt_sem_create("tsem", 0, RT_IPC_FLAG_PRIO);
    if (touch_sem == RT_NULL)
    {
        LOG_E("create dynamic semaphore failed.\n");
    }

#ifndef RT_TOUCH_LVGL_ADAPT
    // 未使用lvgl 使用中断读取触摸
    /* 打开触摸设备 */
    if (rt_touch_open(touch_dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        rt_kprintf("[GT911] Error: Open device failed!\n");
    }

    // 中断函数一定要先于其他control
    if (rt_device_set_rx_indicate(touch_dev, touch_handler) != RT_EOK)
    {
        rt_kprintf("[GT911] Error: Set int handler failed!\n");
    }
    rt_uint16_t x = 240;
    if (rt_touch_control(touch_dev, RT_TOUCH_CTRL_SET_X_RANGE, &x) != RT_EOK)
    {
        rt_kprintf("[GT911] Error: Set x failed!\n");
    }
    rt_uint16_t y = 320;
    if (rt_touch_control(touch_dev, RT_TOUCH_CTRL_SET_Y_RANGE, &y) != RT_EOK)
    {
        rt_kprintf("[GT911] Error: Set y failed!\n");
    }

#else
    // 使用lvgl 使用轮询读取触摸
    rt_uint16_t x = 240;
    if (rt_device_control(touch_dev, RT_TOUCH_CTRL_SET_X_RANGE, &x) != RT_EOK)
    {
        rt_kprintf("[GT911] Error: Set x failed!\n");
    }
    rt_uint16_t y = 320;
    if (rt_device_control(touch_dev, RT_TOUCH_CTRL_SET_Y_RANGE, &y) != RT_EOK)
    {
        rt_kprintf("[GT911] Error: Set y failed!\n");
    }

    /* 配置触摸设备 */
    if (rt_device_control(touch_dev, RT_TOUCH_CTRL_SET_MODE, (void *)RT_DEVICE_FLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("[GT911] Error: Set int failed!\n");
    }
    /* 打开触摸设备 */
    if (rt_device_open(touch_dev, RT_DEVICE_FLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("[GT911] Error: Open device failed!\n");
    }

#endif // RT_TOUCH_LVGL_ADAPT

    /* 获取触摸屏信息 */
    struct rt_touch_info info;
    if (rt_touch_control(touch_dev, RT_TOUCH_CTRL_GET_INFO, &info) != RT_EOK)
    {
        rt_kprintf("[GT911] Error: Get touch info failed!\n");
    }

    //    rt_kprintf("\n[GT911] Touch screen test start\n");
    //    rt_kprintf("type       :%d\n", info.type);      /* 类型：电容型/电阻型*/
    //    rt_kprintf("vendor     :%s\n", info.vendor);    /* 厂商 */
    //    rt_kprintf("point_num  :%d\n", info.point_num); /* 支持的触点个数 */
    //    rt_kprintf("range_x    :%d\n", info.range_x);   /* X 轴分辨率 */
    //    rt_kprintf("range_y    :%d\n", info.range_y);   /* Y 轴分辨率*/
    //    rt_kprintf("[GT911] Press any point to test, press ESC to exit\n\n");

    gt911_thread = rt_thread_create("gt911", touch_callback, RT_NULL, 10240, 14, 200);
    if (gt911_thread != RT_NULL)
    {
        rt_thread_startup(gt911_thread);
    }
    //     while (1)
    //     {
    //         rt_kprintf("1\n", touch_count);
    //         /* 读取触摸点数据 */
    //         point_num = rt_device_read((rt_device_t)touch_dev, 0, &data, GT911_MAX_TOUCH);
    //
    //         if (point_num > 0)
    //         {
    //             touch_count++;
    //             rt_kprintf("[GT911] ----- Touch #%d -----\n", touch_count);
    //
    //             /* 打印每个触摸点的详细信息 */
    //             for (int i = 0; i < point_num; i++)
    //             {
    //                 /* 检查坐标是否在有效范围内 */
    //                 if (data[i].x_coordinate < 0 || data[i].x_coordinate > info.range_x ||
    //                     data[i].y_coordinate < 0 || data[i].y_coordinate > info.range_y)
    //                 {
    //                     rt_kprintf("[GT911] Warning: Invalid coordinates! X=%d, Y=%d\n",
    //                                data[i].x_coordinate, data[i].y_coordinate);
    //                     continue;
    //                 }
    //
    //                 rt_kprintf("[GT911] Point %d: ID=%d, X=%4d, Y=%4d, Width=%3d, Event=%s\n",
    //                            i + 1,
    //                            data[i].track_id,
    //                            data[i].x_coordinate,
    //                            data[i].y_coordinate,
    //                            data[i].width,
    //                            data[i].event == RT_TOUCH_EVENT_DOWN ? "DOWN" : data[i].event == RT_TOUCH_EVENT_MOVE ? "MOVE"
    //                                                                        : data[i].event == RT_TOUCH_EVENT_UP     ? "UP"
    //                                                                                                                 : "NONE");
    //
    //                 /* 在屏幕上显示触摸点 - 根据事件类型使用不同颜色 */
    //                 rt_uint32_t color = RED;
    //                 if (data[i].event == RT_TOUCH_EVENT_DOWN)
    //                     color = GREEN;
    //                 else if (data[i].event == RT_TOUCH_EVENT_MOVE)
    //                     color = BLUE;
    //                 else if (data[i].event == RT_TOUCH_EVENT_UP)
    //                     color = YELLOW;
    //                 LCD_DrawPoint(data[i].x_coordinate, data[i].y_coordinate, RED);
    //
    ////                 /* 画一个十字标记触摸点 */
    ////                 LCD_DrawLine(data[i].x_coordinate - 5, data[i].y_coordinate,
    ////                              data[i].x_coordinate + 5, data[i].y_coordinate, color);
    ////                 LCD_DrawLine(data[i].x_coordinate, data[i].y_coordinate - 5,
    ////                              data[i].x_coordinate, data[i].y_coordinate + 5, color);
    //             }
    //             rt_kprintf("\n");
    //         }
    //
    ////         rt_thread_mdelay(5); // 20ms采样间隔
    //     }

    /* 关闭触摸设备 */
    // rt_device_close((rt_device_t)touch_dev);
    // rt_kprintf("[GT911] Test stopped\n");
}

int touch_start(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("touch", touch, RT_NULL, 5120, 16, 2000);
    if (thread != RT_NULL)
    {
        LOG_I("create thread touch successfully");
    }
    else
    {
    }
    rt_thread_startup(thread);
}

INIT_APP_EXPORT(touch_start);
