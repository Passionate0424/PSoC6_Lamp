// led_brightness_control.c
// LED亮度根据环境光自动/手动调节，支持事件控制和PWM输出

// 事件线程处理开关请求 亮度线程调节 手动—>根据滑条调光 自动->根据光照强度调节
// led_status直接由事件处理线程控制即可，不需要手动更改

#include <rtthread.h>
#include <rtdevice.h>
#include <math.h> // 用于Gamma修正计算
#include "led_brightness_control.h"
#include "radar.h"       // 引入雷达模块头文件
#include "events_init.h" // 引入事件初始化头文件

rt_bool_t led_bright_mode = 0;  // 0:自动调光, 1:手动调光
rt_bool_t led_status = 0;       // 0:关闭, 1:开启  **只供上传  只允许led_request_on和led_request_off修改
rt_uint8_t led_brightness = 50; // LED亮度值，手动亮度时使用，范围0-100
int led_sw_mode = 0;            // LED开关状态

rt_mutex_t led_mutex; // LED互斥锁，防止多线程访问冲突

#define DBG_TAG "led_control"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* 事件集定义 */
#define LED_EVENT_ON (1 << 0)
#define LED_EVENT_OFF (1 << 1)
static rt_event_t led_event = RT_NULL;

/* 亮度调节线程句柄 */
static rt_thread_t led_thread_t = RT_NULL;

/* PWM设备和通道定义 */
#define PWM_DEV_NAME "pwm0"     /* PWM设备名称 */
#define PWM_DEV_CHANNEL 4       /* PWM通道, 对应PWM0_CH3_PORT11 */
#define PWM_MAX_PERIOD 1000000  /* PWM周期 */
#define MAX_LIGHT_VALUE 65535   /* BH1750最大光照值 */
#define MIN_LIGHT_VALUE 0       /* BH1750最小光照值 */
#define GAMMA_FACTOR 2.2        /* Gamma修正因子, 标准值为2.2 */
#define LIGHT_THRESHOLD 1000.0f /* 光照强度阈值，确保使用浮点数 */

/* 线程参数定义 */
#define THREAD_STACK_SIZE 1024
#define THREAD_PRIORITY 15
#define THREAD_TIMESLICE 10

/* PWM设备句柄 */
static struct rt_device_pwm *pwm_dev = RT_NULL;

/* 声明外部函数，从BH1750_Test.c获取当前光照强度 */
extern float get_current_light_intensity(void);

/* ================= 主要功能函数实现 ================= */

/* Gamma修正函数，使LED亮度变化符合人眼感知 */
static rt_uint32_t gamma_correction(float light_value, float max_light)
{
    float x = light_value; // 光照值
    float y;               // 亮度百分比
    rt_uint32_t pwm_value;

    /* 确保光照值为非负数 */
    if (x < 0.0f)
        x = 0.0f;

    /* 根据光照值计算亮度百分比 */
    if (x >= 0.0f && x <= 300.0f)
        y = (7.0f / 30.0f) * x + 30.0f;
    else if (x > 300.0f)
        y = 100.0f - ((x - 300.0f) * 100.0f / (1600.0f - 300.0f));
    else
        y = 0.0f; // 默认值

    /* 确保亮度百分比在0-100之间 */
    if (y > 100.0f)
        y = 100.0f;
    if (y < 0.0f)
        y = 0.0f;

    /* 应用Gamma修正并计算PWM值 */
    pwm_value = (rt_uint32_t)(powf(y / 100.0f, GAMMA_FACTOR) * PWM_MAX_PERIOD);

    pwm_value = pwm_value * 1.2;
    /* 确保PWM值在有效范围内 */
    if (pwm_value > PWM_MAX_PERIOD)
        pwm_value = PWM_MAX_PERIOD;

    if (pwm_value < PWM_MAX_PERIOD * 0.05)
        pwm_value = PWM_MAX_PERIOD * 0.05; // 确保PWM值在有效范围内

    // rt_kprintf("Gamma correction: pwm_value=%d\n", pwm_value);
    return pwm_value;
}

/* 亮度调节线程入口函数，自动根据光照调节PWM */
static void led_brightness_thread_entry(void *parameter)
{
    rt_err_t ret = RT_EOK;
    float light;
    rt_uint32_t pwm_value;
    // 新增：平滑光照值和PWM值
    static float filtered_light = 0.0f;
    static float filtered_pwm = 0.0f;
    const float alpha_light = 0.2f; // 光照滤波系数
    const float alpha_pwm = 0.2f;   // PWM滤波系数

    /* 查找并打开PWM设备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        LOG_E("Cannot find %s device!", PWM_DEV_NAME);
        return;
    }

    /* 使能PWM输出 */
    ret = rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
    if (ret != RT_EOK)
    {
        LOG_E("Enable PWM failed, ret=%d", ret);
        return;
    }

    LOG_I("PWM initialized successfully");

    // 启动时采集一次光照并初始化滤波变量
    light = get_current_light_intensity();
    filtered_light = light;
    pwm_value = gamma_correction(filtered_light, LIGHT_THRESHOLD);
    filtered_pwm = pwm_value;

    while (1)
    {
        if (led_bright_mode == 0)
        {
            // 自动调光模式
            /* 从BH1750模块获取光照强度值 */
            light = get_current_light_intensity();

            // 1. 对光照值做低通滤波
            filtered_light = alpha_light * light + (1.0f - alpha_light) * filtered_light;

            // 2. 计算目标PWM值
            pwm_value = gamma_correction(filtered_light, LIGHT_THRESHOLD);

            // 3. 对PWM值做低通滤波
            filtered_pwm = alpha_pwm * pwm_value + (1.0f - alpha_pwm) * filtered_pwm;

            // rt_kprintf("Current light: %.2f, Filtered light: %.2f, PWM value: %d\n",light, filtered_light, (int)filtered_pwm);

            // 4. 设置新的PWM占空比（取整）
            ret = rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, PWM_MAX_PERIOD, PWM_MAX_PERIOD - (rt_uint32_t)filtered_pwm);
            if (ret != RT_EOK)
            {
                LOG_E("Update PWM duty cycle failed, ret=%d", ret);
            }
        }
        else
        {
            // 手动调光模式
            // 4. 设置新的PWM占空比（取整）
            ret = rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, PWM_MAX_PERIOD, PWM_MAX_PERIOD - (rt_uint32_t)(PWM_MAX_PERIOD * led_brightness / 100));
            if (ret != RT_EOK)
            {
                LOG_E("Update PWM duty cycle failed, ret=%d", ret);
            }
        }

        /* 每100毫秒调整一次 */
        rt_thread_mdelay(100);
    }
}

/* ================= 事件接口与线程控制 ================= */

/* 提供给外部调用的事件发送接口（开灯） */
void led_request_on(void)
{
    rt_mutex_take(led_mutex, RT_WAITING_FOREVER); // 获取互斥锁，防止多线程冲突
    if (led_event)
        rt_event_send(led_event, LED_EVENT_ON);
    led_status = 1;              // 更新LED状态为开启
    rt_mutex_release(led_mutex); // 释放互斥锁
}
/* 提供给外部调用的事件发送接口（关灯） */
void led_request_off(void)
{
    rt_mutex_take(led_mutex, RT_WAITING_FOREVER); // 获取互斥锁，防止多线程冲突
    if (led_event)
        rt_event_send(led_event, LED_EVENT_OFF);
    led_status = 0;              // 更新LED状态为关闭
    rt_mutex_release(led_mutex); // 释放互斥锁
}
/* 提供给外部调用的事件发送接口（切换为自动开关模式） */
void led_auto_switch(void)
{
    led_sw_mode = 1; // 设置为自动开关模式
    // 切换为自动，根据人体存在状态设置开或关
    if (human_presence_status == 1)
    {
        led_request_on(); // 如果人体存在，则发送开灯事件
    }
    else
    {
        led_request_off(); // 如果人体不存在，则发送关灯事件
    }
}

/* 提供给外部调用的事件发送接口（切换为手动模式） */
void led_manual_switch(void)
{
    led_sw_mode = 0; // 设置为手动开关模式
    // 切换为手动，自动关灯，根据btn_led状态设置开或关
    if (btn1_status == true)
    {
        led_request_on(); // 如果按钮状态为开，则发送开灯事件
    }
    else
    {
        led_request_off(); // 如果按钮状态为关，则发送关灯事件
    }
}

/* 启动LED亮度调节线程，仅本文件内部调用 */
static int led_brightness_control(void)
{
    /* 如果线程已经创建则直接返回 */
    if (led_thread_t != RT_NULL)
    {
        LOG_I("LED brightness control thread already exists");
        return RT_EOK;
    }

    /* 创建LED亮度控制线程 */
    led_thread_t = rt_thread_create("led_ctrl",
                                    led_brightness_thread_entry,
                                    RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY,
                                    THREAD_TIMESLICE);

    /* 启动线程 */
    if (led_thread_t != RT_NULL)
    {
        rt_thread_startup(led_thread_t);
        LOG_I("LED brightness control thread started");
        return RT_EOK;
    }
    else
    {
        LOG_E("LED brightness control thread create failed");
        return -RT_ERROR;
    }
}

/* 停止LED亮度调节线程，仅本文件内部调用 */
static int led_brightness_control_stop(void)
{
    rt_err_t ret = RT_EOK;

    /* 检查线程是否存在 */
    if (led_thread_t == RT_NULL)
    {
        LOG_I("LED brightness control thread does not exist");
        return -RT_ERROR;
    }

    /* 禁用PWM输出 */
    if (pwm_dev != RT_NULL)
    {
        ret = rt_pwm_disable(pwm_dev, PWM_DEV_CHANNEL);
        if (ret != RT_EOK)
        {
            LOG_E("Disable PWM failed, ret=%d", ret);
        }
    }

    /* 删除线程 */
    ret = rt_thread_delete(led_thread_t);
    if (ret == RT_EOK)
    {
        led_thread_t = RT_NULL;
        LOG_I("LED brightness control thread stopped");
        return RT_EOK;
    }
    else
    {
        LOG_E("LED brightness control thread stop failed");
        return -RT_ERROR;
    }
}

/* 事件处理线程，响应开关灯事件并控制亮度线程 */
static void led_event_thread_entry(void *parameter)
{
    rt_uint32_t e;
    while (1)
    {
        if (rt_event_recv(led_event, LED_EVENT_ON | LED_EVENT_OFF, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &e) == RT_EOK)
        {
            if (e & LED_EVENT_ON)
            {
                if (led_thread_t == RT_NULL)
                {
                    led_brightness_control(); // 启动亮度调节线程
                    led_status = 1;
                }
            }
            if (e & LED_EVENT_OFF)
            {
                if (led_thread_t != RT_NULL)
                {
                    led_brightness_control_stop(); // 停止亮度调节线程
                    led_status = 0;
                }
            }
        }
    }
}

/* 事件集初始化和事件线程启动，应用初始化时自动调用 */
int led_event_system_init(void)
{
    led_mutex = rt_mutex_create("led_mutex", RT_IPC_FLAG_FIFO);
    if (led_mutex == RT_NULL)
    {
        LOG_E("Failed to create LED mutex");
        return -RT_ERROR;
    }

    if (led_event == RT_NULL)
    {
        led_event = rt_event_create("led_evt", RT_IPC_FLAG_FIFO);
    }
    rt_thread_t evt_tid = rt_thread_create("led_evt", led_event_thread_entry, RT_NULL, 1024, 16, 10);
    if (evt_tid)
        rt_thread_startup(evt_tid);
    return 0;
}
INIT_ENV_EXPORT(led_event_system_init);

// ================== MSH命令导出 ==================
/* 导出到msh命令列表，便于命令行手动控制 */
MSH_CMD_EXPORT(led_brightness_control, start led brightness control based on light intensity);
MSH_CMD_EXPORT(led_brightness_control_stop, stop led brightness control);

// ================== PWM测试线程实现 ==================
static rt_thread_t pwm_test_thread = RT_NULL;
static volatile int test_pwm_duty_cycle = 50; // 默认50%

static void pwm_test_thread_entry(void *parameter)
{
    struct rt_device_pwm *dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (dev == RT_NULL)
    {
        rt_kprintf("Cannot find %s device!\n", PWM_DEV_NAME);
        return;
    }
    rt_pwm_enable(dev, PWM_DEV_CHANNEL);
    while (1)
    {
        int duty = test_pwm_duty_cycle;
        if (duty < 0)
            duty = 0;
        if (duty > 100)
            duty = 100;
        rt_pwm_set(dev, PWM_DEV_CHANNEL, PWM_MAX_PERIOD, PWM_MAX_PERIOD * (100 - duty) / 100);
        rt_thread_mdelay(100);
    }
}

static void pwm_test(int argc, char **argv)
{
    if (argc < 2)
    {
        rt_kprintf("Usage: pwm_test <duty(0-100)>\n");
        return;
    }
    int duty = atoi(argv[1]);
    if (duty < 0)
        duty = 0;
    if (duty > 100)
        duty = 100;
    test_pwm_duty_cycle = duty;
    if (pwm_test_thread == RT_NULL)
    {
        pwm_test_thread = rt_thread_create("pwm_test", pwm_test_thread_entry, RT_NULL, 512, 18, 10);
        if (pwm_test_thread)
            rt_thread_startup(pwm_test_thread);
    }
    rt_kprintf("PWM test started, duty=%d\n", test_pwm_duty_cycle);
}
MSH_CMD_EXPORT(pwm_test, pwm_test<duty(0 - 100)> : start pwm test thread and set duty cycle);