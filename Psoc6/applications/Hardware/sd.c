// 上电后自动挂载sd0到elm
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <board.h>

#define DBG_TAG "sd"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <drv_sdcard.h>

#define SD_CD_PIN GET_PIN(2, 6) // SD卡检测引脚，B0

static struct rt_semaphore sd_event_sem;
static volatile int sd_event_flag = 0; // 1:插卡 2:拔卡

rt_uint8_t sd_status = 0; // 0:未插入，1:已插入

// SD卡设备挂载
rt_err_t sd_device_register(void)
{
    rt_hw_sdio_init();
    return RT_EOK;
}

// SD卡设备卸载
rt_err_t sd_device_remove(void)
{
    struct rt_device *device = rt_device_find("sd0");
    if (device)
    {
        rt_device_unregister(device);
        // 释放分区锁（信号量）
        extern struct rthw_sdio *sdio;
        if (sdio && sdio->part.lock)
        {
            rt_sem_delete(sdio->part.lock);
            sdio->part.lock = RT_NULL;
        }
        // 释放SD卡硬件资源
        if (sdio)
        {
            cyhal_sdhc_free(&sdio->sdhc_obj);
            rt_free(sdio);
            sdio = RT_NULL;
        }
    }else
    {
        LOG_W("SD卡设备未找到，无法卸载!\n");
        return -RT_ERROR;
    }

    return RT_EOK;
}

static void mount_sd0(void *args)
{
    if (rt_pin_read(SD_CD_PIN) == PIN_LOW)
    {
        // SD卡已插入
        // 检查设备是否注册
        struct rt_device *device = rt_device_find("sd0");
        if (device == RT_NULL)
        {
            if (sd_device_register() != RT_EOK)
            {
                LOG_E("SD卡设备注册失败!\n");
                return;
            }
            device = rt_device_find("sd0");
        }
        if (device != RT_NULL)
        {
            if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
            {
                LOG_I("SD卡挂载成功: sd0 -> /\n");
            }
            else
            {
                LOG_E("SD卡挂载失败!\n");
                return;
            }
        }
    }
    else
    {
        if (rt_device_find("sd0") != RT_NULL)
        {
            if (dfs_unmount("/"))
            {
                LOG_I("SD卡卸载成功!\n");
            }
            else
            {
                LOG_E("SD卡卸载失败!\n");
                return;
            }
            if (sd_device_remove() != RT_EOK)
            {
                LOG_E("SD卡设备卸载失败!\n");
                return;
            }
        }
    }
    return;
}
// INIT_COMPONENT_EXPORT(mount_sd0);

// ISR中只发信号量
static void sd_cd_irq_callback(void *args)
{
    rt_sem_release(&sd_event_sem);
}

// 线程中处理实际挂载/卸载
static void sd_event_worker(void *parameter)
{
    int last_status = -1;
    while (1)
    {
        rt_sem_take(&sd_event_sem, RT_WAITING_FOREVER);
        rt_thread_mdelay(50); // 等待稳定状态
        int current_status = rt_pin_read(SD_CD_PIN);
        if (current_status != last_status)
        {
            last_status = current_status;
            if (current_status == PIN_LOW)
            {
                // 插卡时的挂载流程
                struct rt_device *device = rt_device_find("sd0");
                if (device == RT_NULL)
                {
                    if (sd_device_register() != RT_EOK)
                    {
                        LOG_E("SD卡设备注册失败!\n");
                        continue;
                    }
                    device = rt_device_find("sd0");
                }
                if (device != RT_NULL)
                {
                    if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
                    {
                        LOG_I("SD卡挂载成功: sd0 -> /\n");
                    }
                    else
                    {
                        LOG_E("SD卡挂载失败!\n");
                    }
                }
            }
            else
            {
                // 拔卡时的卸载流程
                if (rt_device_find("sd0") != RT_NULL)
                {
                    if (dfs_unmount("/") == 0)
                    {
                        LOG_I("SD卡卸载成功!\n");
                    }
                    else
                    {
                        LOG_E("SD卡卸载失败!\n");
                        continue;
                    }
                    if (sd_device_remove() != RT_EOK)
                    {
                        LOG_E("SD卡设备卸载失败!\n");
                    }
                }
            }
        }
    }
}

// sd卡热拔插
static int sd_cd_pin_init(void)
{
    //rt_pin_mode(SD_CD_PIN, PIN_MODE_INPUT_PULLUP); // 设置SD卡检测引脚为输入上拉模式
    rt_sem_init(&sd_event_sem, "sd_evt", 0, RT_IPC_FLAG_FIFO);
    if (rt_pin_attach_irq(SD_CD_PIN, PIN_IRQ_MODE_RISING_FALLING, sd_cd_irq_callback, RT_NULL) != RT_EOK)
    {
        LOG_E("SD卡检测引脚中断注册失败!\n");
        return -1;
    }
    if (rt_pin_irq_enable(SD_CD_PIN, PIN_IRQ_ENABLE) != RT_EOK)
    {
        LOG_E("SD卡检测引脚中断使能失败!\n");
        return -1;
    }
    // 启动事件处理线程
    rt_thread_t tid = rt_thread_create("sd_evt", sd_event_worker, RT_NULL, 2048, 20, 10);
    if (tid)
        rt_thread_startup(tid);
    else
        LOG_E("SD卡事件线程创建失败!\n");
    // 上电主动检测一次
    sd_cd_irq_callback(RT_NULL);
    return 0;
}
INIT_ENV_EXPORT(sd_cd_pin_init);
