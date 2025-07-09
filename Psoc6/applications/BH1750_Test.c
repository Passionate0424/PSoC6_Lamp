// BH1750光照传感器测试应用
//  该应用程序用于测试BH1750光照传感器的功能
//  包括初始化、读取光照强度、以及提供接口供其他模块获取当前光照强度

#include <rtthread.h>
#include <rtdevice.h>
#include "bh1750.h"

#define DBG_TAG "bh1750_test"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* 定义线程句柄 */
static rt_thread_t bh1750_thread_t = RT_NULL;
/* 定义线程栈大小 */
#define THREAD_STACK_SIZE 1024
/* 定义线程优先级 */
#define THREAD_PRIORITY 15
/* 定义线程时间片 */
#define THREAD_TIMESLICE 10
/* BH1750设备对象 */
static struct bh1750_device bh1750_dev;

/* 添加全局光照强度变量，供其他模块访问 */
float current_light_intensity = 0.0;
/* 互斥锁，保护共享数据 */
static rt_mutex_t light_mutex = RT_NULL;

/* 获取当前光照强度的接口函数 */
float get_current_light_intensity(void)
{
    float light_value;

    /* 获取互斥锁 */
    if (light_mutex != RT_NULL)
    {
        rt_mutex_take(light_mutex, RT_WAITING_FOREVER);
        light_value = current_light_intensity;
        rt_mutex_release(light_mutex);
    }
    else
    {
        light_value = current_light_intensity;
    }

    return light_value;
}

/* 线程入口函数 */
static void bh1750_thread_entry(void *parameter)
{
    rt_err_t ret = RT_EOK;
    float light;

    /* 初始化BH1750传感器 */
    ret = bh1750_init(&bh1750_dev, "i2c4");
    if (ret != RT_EOK)
    {
        LOG_E("BH1750 init failed, ret=%d", ret);
        return;
    }

    /* 打开BH1750传感器电源 */
    ret = bh1750_power_on(&bh1750_dev);
    if (ret != RT_EOK)
    {
        LOG_E("BH1750 power on failed, ret=%d", ret);
        return;
    }

    LOG_I("BH1750 sensor initialized successfully");

    while (1)
    {
        /* 读取光照强度值 */
        // light = bh1750_read_light(&bh1750_dev) * 10;
        light = bh1750_read_light(&bh1750_dev);

        /* 更新全局光照强度变量 */
        if (light_mutex != RT_NULL)
        {
            rt_mutex_take(light_mutex, RT_WAITING_FOREVER);
            /* 如果读取到的光照强度值为0，则不更新 */
            if (light != 0)
                /* 更新当前光照强度 */
                current_light_intensity = light;
            rt_mutex_release(light_mutex);
        }
        else
        {
            if (light != 0)
                current_light_intensity = light;
        }

        /* 打印光照强度值 */
        //rt_kprintf("BH1750 light intensity: %f lux\n", current_light_intensity);

        /* 每1秒读取一次 */
        rt_thread_mdelay(100);
    }
}

/* 应用入口函数 */
int bh1750_sample(void)
{
    /* 如果线程已经创建则直接返回 */
    if (bh1750_thread_t != RT_NULL)
    {
        LOG_I("BH1750 sample thread already exists");
        return RT_EOK;
    }

    /* 创建互斥锁 */
    if (light_mutex == RT_NULL)
    {
        light_mutex = rt_mutex_create("light_lock", RT_IPC_FLAG_FIFO);
        if (light_mutex == RT_NULL)
        {
            LOG_E("Create light mutex failed");
            return -RT_ERROR;
        }
    }

    /* 创建BH1750线程 */
    bh1750_thread_t = rt_thread_create("bh1750",
                                       bh1750_thread_entry,
                                       RT_NULL,
                                       THREAD_STACK_SIZE,
                                       THREAD_PRIORITY,
                                       THREAD_TIMESLICE);

    /* 启动线程 */
    if (bh1750_thread_t != RT_NULL)
    {
        rt_thread_startup(bh1750_thread_t);
        LOG_I("BH1750 sample thread started");
        return RT_EOK;
    }
    else
    {
        LOG_E("BH1750 sample thread create failed");
        return -RT_ERROR;
    }
}

/* 停止BH1750采样的函数 */
int bh1750_sample_stop(void)
{
    rt_err_t ret = RT_EOK;

    /* 检查线程是否存在 */
    if (bh1750_thread_t == RT_NULL)
    {
        LOG_I("BH1750 sample thread does not exist");
        return -RT_ERROR;
    }

    /* 关闭BH1750电源 */
    ret = bh1750_power_down(&bh1750_dev);
    if (ret != RT_EOK)
    {
        LOG_E("BH1750 power down failed, ret=%d", ret);
    }

    /* 删除线程 */
    ret = rt_thread_delete(bh1750_thread_t);
    if (ret == RT_EOK)
    {
        bh1750_thread_t = RT_NULL;
        LOG_I("BH1750 sample thread stopped");

        /* 删除互斥锁 */
        if (light_mutex != RT_NULL)
        {
            rt_mutex_delete(light_mutex);
            light_mutex = RT_NULL;
        }

        return RT_EOK;
    }
    else
    {
        LOG_E("BH1750 sample thread stop failed");
        return -RT_ERROR;
    }
}

/* 导出到msh命令列表 */
MSH_CMD_EXPORT(bh1750_sample, bh1750 sensor sample);
INIT_APP_EXPORT(bh1750_sample);
MSH_CMD_EXPORT(bh1750_sample_stop, stop bh1750 sensor sample);
