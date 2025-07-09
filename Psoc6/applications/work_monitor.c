// work_monitor.c - 检测模式评价算法实现文件
// 该文件实现了根据工作时间和玩手机时间计算分数的算法，并支持通过串口或按键切换模式。
//
// 包含内容：
// 1. calc_score：分数计算逻辑，支持两种模式（展示/实用）
// 2. switch_mode_by_uart：通过串口命令切换模式
// 3. switch_mode_by_key：通过按键切换模式
//
// 作者：xxx
// 日期：2025-07-06

// 检测模式评价算法
// score_logic.c
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "work_monitor.h"   // 包含工作监测相关头文件
#include "nnom_lstm_demo.h" // 包含NNOM LSTM模型相关头文件
#include "gui_guider.h"     // 包含GUI相关头文件

#define MODE_DEMO 0
#define MODE_REAL 1

uint8_t mode = MODE_DEMO; // 默认模式：展示
monitor_setting_t monitor_setting;
monitor_data_t monitor_data;
float energy = 100;                              // 初始能量值
float work_score = 100;                          // 初始工作分数
float carefulness = 100;                         // 初始专注度
static rt_thread_t energy_thread = RT_NULL;      // 能量计算线程
static rt_thread_t work_score_thread = RT_NULL;  // 工作分数计算线程
static rt_thread_t carefulness_thread = RT_NULL; // 专注度计算线程
static rt_mutex_t monitor_mutex = RT_NULL;       // 监测数据互斥锁
static int energy_thread_status = 0;             // 能量计算线程状态
static int work_score_status = 0;                // 工作分数计算线程状态
static int carefulness_thread_status = 0;        // 专注度计算线程状态

/*
 * 专注度计算线程
 * 只对工作时间内进行计算
 * 工作时间内未处于工作状态，分数下降，精力值越低下降越快
 * 根据玩手机时间占比进行扣分
 */
static void monitor_calc_carefulness_entry(void *parameter)
{
    static int temp_work_duration = 0;  // 上次计算时的工作持续时间
    static int temp_phone_duration = 0; // 上次计算时的玩手机持续时间
    static time_t temp_sum_time = NULL;       // 上次计算时的总时间
    temp_sum_time = time(RT_NULL); // 初始化临时总时间
    while (1)
    {
        int work_delta, phone_delta, sum_delta;
        // 加锁保护全局数据
        rt_mutex_take(monitor_mutex, RT_WAITING_FOREVER);
        work_delta = monitor_data.work_duration - temp_work_duration;
        phone_delta = monitor_data.phone_duration - temp_phone_duration;
        time_t now_time = time(RT_NULL);
        sum_delta = now_time - temp_sum_time;
        // 计算专注度
        rt_kprintf("工作持续时间: %.1f, 玩手机持续时间: %.1f, 总时间: %.1f\n",
                 monitor_data.work_duration, monitor_data.phone_duration, monitor_data.sum_time);
        if (monitor_data.is_working && phone_delta > 0 && sum_delta > 0)
        {
            float phone_ratio = (float)phone_delta / sum_delta;
            rt_kprintf("======================================工作持续时间: %.1f, 玩手机持续时间: %.1f, 总时间: %.1f, 玩手机占比: %.2f\n",
                     monitor_data.work_duration, monitor_data.phone_duration, monitor_data.sum_time, phone_ratio);
            carefulness -= phone_ratio * 0.1 * (150 - energy); // 玩手机时间占比越高，专注度下降越快
            if (carefulness < 0)
                carefulness = 0;
        }
        temp_work_duration = monitor_data.work_duration;   // 更新临时工作持续时间
        temp_phone_duration = monitor_data.phone_duration; // 更新临时玩手机持续时间
        temp_sum_time = now_time ;             // 更新临时总时间
        rt_kprintf("专注度更新: %.1f,", carefulness);
        rt_mutex_release(monitor_mutex);
        // 更新专注度显示
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
        lv_label_set_text_fmt(guider_ui.monitor_label_carefulness, "%.1f", carefulness);
        rt_mutex_release(lv_mutex);
        rt_thread_mdelay(1000); // 每秒更新一次专注度
    }
}

/* 计算分数，根据当前输入的工作/玩手机时间返回分数
 * 如果处于工作时间，玩手机扣分
 * 能量值越低时工作分数越低
 * 只对工作时间内进行判断
 */
static void monitor_calc_score_entry(void *parameter)
{
    static int temp_work_duration = 0;  // 上次计算时的工作持续时间
    static int temp_phone_duration = 0; // 上次计算时的玩手机持续时间
    static time_t temp_sum_time = NULL;       // 上次计算时的总时间
    temp_sum_time = time(RT_NULL); // 初始化临时总时间
    while (1)
    {
        int work_delta, phone_delta, sum_delta;
        rt_mutex_take(monitor_mutex, RT_WAITING_FOREVER);
        work_delta = monitor_data.work_duration - temp_work_duration;
        phone_delta = monitor_data.phone_duration - temp_phone_duration;
        time_t  now_time = time(RT_NULL);
        sum_delta = now_time - temp_sum_time;
        if (monitor_data.is_working)
        {
            if (work_delta >= 0 && phone_delta >= 0 && sum_delta >= 0)
            {
                // 工作时，玩手机扣分
                work_score -= phone_delta * 0.1f; // 玩手机每秒钟扣0.1分
                if (work_score < 0)
                    work_score = 0;
                // 能量值越低，工作分数下降越快
                if (energy <= 40 && energy > 30)
                {
                    work_score -= (100 - energy) * work_delta * 0.001f; // 能量值每下降1分，工作分数每秒下降0.001分
                }
                else if (energy <= 30)
                {
                    work_score -= (100 - energy) * work_delta * 0.003f; // 能量值每下降1分，工作分数每秒下降0.002分
                }
            }
        }
        temp_work_duration = monitor_data.work_duration;
        temp_phone_duration = monitor_data.phone_duration;
        temp_sum_time = now_time; // 更新临时总时间
        rt_mutex_release(monitor_mutex);
        // 更新分数显示
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
        lv_label_set_text_fmt(guider_ui.monitor_label_work_score, "%.1f", work_score);
        rt_mutex_release(lv_mutex);
        rt_thread_mdelay(1000); // 每秒更新一次分数
    }
}

/*
 * 根据工作时间、压力状态、环境评分计算精力值
 * 工作时精力值下降，玩手机、静息状态增加
 * 处于压力状态、环境状态较差时，工作精力下降更快、恢复更慢
 */
static void monitor_calc_energy_entry(void *parameter)
{
    static int temp_work_duration = 0;  // 上次计算时的工作持续时间
    static int temp_phone_duration = 0; // 上次计算时的玩手机持续时间
    static int temp_sum_time = 0;       // 上次计算时的总时间
    while (1)
    {
        float param = 0.0f;
        if (stress_pre >= 0)
        {
            param = (10 - stress_pre) * 0.1f + env_pre * 0.1f; // 假设压力值越高，精力恢复越慢
        }
        int work_delta, phone_delta, sum_delta;
        rt_mutex_take(monitor_mutex, RT_WAITING_FOREVER);
        work_delta = monitor_data.work_duration - temp_work_duration;
        phone_delta = monitor_data.phone_duration - temp_phone_duration;
        sum_delta = monitor_data.sum_time - temp_sum_time;
        int energy_change = 0;
        if (work_delta > 0)
            energy_change -= (work_delta / 60) * (20 - param); // 工作每分钟消耗能量
        if (phone_delta > 0)
            energy_change += (phone_delta / 60) * 3 * param; // 玩手机每分钟恢复能量
        int rest_delta = sum_delta - work_delta - phone_delta;
        if (rest_delta > 0)
            energy_change += (rest_delta / 60) * 4 * param; // 静息每分钟恢复能量
        energy += energy_change;
        if (energy < 0)
            energy = 0;
        if (energy > 100)
            energy = 100;
        temp_work_duration = monitor_data.work_duration;
        temp_phone_duration = monitor_data.phone_duration;
        temp_sum_time = monitor_data.sum_time;
        rt_kprintf("能量值更新: %d,", (int)energy);
        rt_mutex_release(monitor_mutex);
        // 更新能量值显示
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER);
        lv_label_set_text_fmt(guider_ui.monitor_label_energy, "%d", (int)energy);
        rt_mutex_release(lv_mutex);
        rt_thread_mdelay(1000); // 每秒更新一次能量值
    }
}

// 通过串口接收的命令切换模式，支持 "MODE=0" 和 "MODE=1"
void switch_mode_by_uart(char *rx_buf)
{
    if (strncmp(rx_buf, "MODE=0", 6) == 0)
    {
        mode = MODE_DEMO;
        printf("切换至 展示模式\n");
    }
    else if (strncmp(rx_buf, "MODE=1", 6) == 0)
    {
        mode = MODE_REAL;
        printf("切换至 实用模式\n");
    }
}

// 线程初始化
int work_monitor_thread_init(void)
{
    // 初始化监测数据
    monitor_data.sum_time = 0;
    monitor_data.work_duration = 0;
    monitor_data.phone_duration = 0;
    monitor_data.is_working = 0;
    energy = 100;
    work_score = 100;
    carefulness = 100;
    // 创建互斥锁
    if (monitor_mutex == RT_NULL)
    {
        monitor_mutex = rt_mutex_create("monitor_mutex", RT_IPC_FLAG_FIFO);
        if (monitor_mutex == RT_NULL)
        {
            rt_kprintf("monitor_mutex 创建失败！\n");
            return -1;
        }
    }
    // 创建能量计算线程
    energy_thread = rt_thread_create("energy_calc", monitor_calc_energy_entry, RT_NULL, 2048, 20, 10);
    // 创建工作分数计算线程
    work_score_thread = rt_thread_create("work_score_calc", monitor_calc_score_entry, RT_NULL, 2048, 20, 10);
    // 创建专注度计算线程
    carefulness_thread = rt_thread_create("carefulness_calc", monitor_calc_carefulness_entry, RT_NULL, 2048, 20, 10);
    return 1;
}
INIT_ENV_EXPORT(work_monitor_thread_init); // 初始化工作监测线程

// 计算线程启动，外部调用
void monitor_calc_start(void)
{
    if (energy_thread != RT_NULL)
    {
        if (energy_thread_status == 0)
        {
            rt_thread_startup(energy_thread);
            energy_thread_status = 1; // 设置能量计算状态为启动
        }
    }
    else
    {
        energy_thread = rt_thread_create("energy_calc", monitor_calc_energy_entry, RT_NULL, 2048, 20, 10);
        if (energy_thread != RT_NULL)
        {
            rt_thread_startup(energy_thread);
            energy_thread_status = 1; // 设置能量计算状态为启动
        }
        else
        {
            rt_kprintf("精力计算线程创建失败！\n");
        }
    }
    if (work_score_thread != RT_NULL)
    {
        if (work_score_status == 0)
        {
            rt_thread_startup(work_score_thread);
            work_score_status = 1; // 设置工作分数计算状态为启动
        }
    }
    else
    {
        work_score_thread = rt_thread_create("work_score_calc", monitor_calc_score_entry, RT_NULL, 2048, 20, 10);
        if (work_score_thread != RT_NULL)
        {
            rt_thread_startup(work_score_thread);
            work_score_status = 1; // 设置工作分数计算状态为启动
        }
        else
        {
            rt_kprintf("工作分数计算线程创建失败！\n");
        }
    }
    if (carefulness_thread != RT_NULL)
    {
        if (carefulness_thread_status == 0)
        {
            rt_thread_startup(carefulness_thread);
            carefulness_thread_status = 1; // 设置专注度计算状态为启动
        }
    }
    else
    {
        carefulness_thread = rt_thread_create("carefulness_calc", monitor_calc_carefulness_entry, RT_NULL, 2048, 20, 10);
        if (carefulness_thread != RT_NULL)
        {
            rt_thread_startup(carefulness_thread);
            carefulness_thread_status = 1; // 设置专注度计算状态为启动
        }
        else
        {
            rt_kprintf("专注度计算线程创建失败！\n");
        }
    }
}

// 计算线程停止，外部调用
void monitor_calc_stop(void)
{
    if (energy_thread != RT_NULL)
    {
        rt_thread_delete(energy_thread);
        energy_thread = RT_NULL; // 重置线程指针
    }
    if (work_score_thread != RT_NULL)
    {
        rt_thread_delete(work_score_thread);
        work_score_thread = RT_NULL; // 重置线程指针
    }
    if (carefulness_thread != RT_NULL)
    {
        rt_thread_delete(carefulness_thread);
        carefulness_thread = RT_NULL; // 重置线程指针
    }
}
