/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-06-29     Rbb666       first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "ntp.h"
#include "lcd_rtthread.h"
#include "gt911.h"
#include <rtdbg.h>
#include <time.h>

#include "drv_gpio.h"


#define LED_PIN GET_PIN(0, 1)

void thread_enty(void *para)
{
    lcd();
}

void hook_entry(struct rt_thread *from, struct rt_thread *to)
{
    rt_kprintf("from %s ------to %s\n", from->parent.name, to->parent.name);
}

int main(void)
{

    // rt_scheduler_sethook(hook_entry);

    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    // rt_thread_t thread;

    // thread = rt_thread_create("lcd", thread_enty, RT_NULL, 5120, 9, 200);
    // if (thread != RT_NULL)
    // {
    //     LOG_I("create thread successfully");
    //     rt_thread_startup(thread);
    // }
    // else
    // {
    // }
    
    

    for (;;)
    {
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
