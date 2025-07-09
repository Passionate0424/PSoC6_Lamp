
#include <sg90.h>

sg90_cfg my_sg90_config0 = {
    .period = PKG_USING_SG90_PERIOD, // 20ms 周期
    .dir = SG90_CLOCKWISE,
    .pulse = PKG_USING_MAX_ANGLE, // 1.5ms 脉冲宽度
    .name = RT_NULL,
    .channel = PKG_USING_PWM_DEV_CHANNEL,
};
sg90_cfg my_sg90_config1 = {
    .period = PKG_USING_SG90_PERIOD, // 20ms 周期
    .dir = SG90_CLOCKWISE,
    .pulse = PKG_USING_MAX_ANGLE, // 1.5ms 脉冲宽度
    .name = RT_NULL,
    .channel = PKG_USING_PWM_DEV_CHANNEL,
};

// NUM0对应pwm0舵机，adc4
// NUM1对应pwm1舵机，adc5

/*
 * @brief sg90_init 初始化舵机
 * @param cfg sg90_cfg_t 结构体指针
 * @param num 舵机编号，0或1
 * @return RT_EOK 成功，RT_ERROR 失败
 */
rt_err_t sg90_init(sg90_cfg_t cfg, uint8_t num)
{
    if (num == 0)
    {
        /* 查找设备 */
        cfg->name = (struct rt_device_pwm *)rt_device_find(PKG_USING_PWM_DEV_NAME);
        if (cfg->name == RT_NULL)
        {
            rt_kprintf("pwm sample run failed! can't find %s device!\n", cfg->name);
            return RT_ERROR;
        }
        /* 设置PWM周期和脉冲宽度默认值 */
        rt_pwm_set(cfg->name, cfg->channel, cfg->period, cfg->pulse);
        /* 使能设备 */
        rt_pwm_enable(cfg->name, cfg->channel);
    }
    else if (num == 1)
    {
        /* 查找设备 */
        cfg->name = (struct rt_device_pwm *)rt_device_find("pwm1");
        if (cfg->name == RT_NULL)
        {
            rt_kprintf("pwm sample run failed! can't find %s device!\n", cfg->name);
            return RT_ERROR;
        }
        /* 设置PWM周期和脉冲宽度默认值 */
        rt_pwm_set(cfg->name, cfg->channel, cfg->period, cfg->pulse);
        /* 使能设备 */
        rt_pwm_enable(cfg->name, cfg->channel);
    }
    else
    {
        return RT_ERROR;
    }
    return RT_EOK;
}

/*
 * @brief sg90_deinit 反初始化舵机
 * @param cfg sg90_cfg_t 结构体指针
 * @return RT_EOK 成功，RT_ERROR 失败
 */
rt_err_t sg90_deinit(sg90_cfg_t cfg)
{
    if (cfg->name != RT_NULL)
    {
        /* code */
        rt_pwm_disable(cfg->name, cfg->channel);

        return RT_EOK;
    }

    return RT_ERROR;
}

/*
 * @brief sg90_set_angle 设置舵机角度
 * @param cfg sg90_cfg_t 结构体指针
 * @param angle 旋转角度
 * @return RT_EOK 成功，RT_ERROR 失败
 */
rt_err_t sg90_set_angle(sg90_cfg_t cfg, float angle)
{

    if (cfg->name == RT_NULL)
    {
        rt_kprintf("not find the sg90 device\n");

        return RT_ERROR;
    }
    else
    {

        cfg->pulse = ANGEL2PULSE(angle);
        rt_pwm_set(cfg->name, cfg->channel, cfg->period, cfg->pulse);

        return RT_EOK;
    }
}

/*
 * @brief sg90_control 控制舵机旋转
 * @param cfg sg90_cfg_t 结构体指针
 * @param angle 旋转角度
 * @param dir 旋转方向 SG90_CLOCKWISE 或 SG90_ANTICLOCKWISE
 * @return RT_EOK 成功，RT_ERROR 失败
 */
rt_err_t sg90_control(sg90_cfg_t cfg, int angle, int dir)
{

    if (cfg->dir == SG90_CLOCKWISE)
    {
        cfg->pulse += ANGEL2PULSE(angle);
        if (cfg->pulse >= PKG_USING_MAX_ANGLE)
        {
            cfg->pulse = PKG_USING_MAX_ANGLE;
            rt_kprintf("sg90 is max angle\n");
        }
        return RT_EOK;
    }
    else if (cfg->dir == SG90_ANTICLOCKWISE)
    {
        cfg->pulse -= ANGEL2PULSE(angle);
        if (cfg->pulse <= PKG_USING_MIN_ANGLE)
        {
            cfg->pulse = PKG_USING_MIN_ANGLE;
            rt_kprintf("sg90 is min angle\n");
        }
        return RT_EOK;
    }

    return RT_ERROR;
}

/*
 * @brief sg90_get 获取舵机当前状态
 * @param cfg sg90_cfg_t 结构体指针
 * @return RT_EOK 成功，RT_ERROR 失败
 */

rt_err_t sg90_get(sg90_cfg_t cfg)
{
    if (cfg->name == RT_NULL)
    {
        rt_kprintf("not find the sg90 device\n");

        return RT_ERROR;
    }

    rt_kprintf("sg90 get device is %s\n", PKG_USING_PWM_DEV_NAME);
    rt_kprintf("sg90 get channel is %d\n", cfg->channel);
    switch (cfg->dir)
    {
    case SG90_ANTICLOCKWISE:
        rt_kprintf("sg90 get direction is %s\n", "ANTICLOCKWISE");
        break;

    case SG90_CLOCKWISE:
        rt_kprintf("sg90 get direction is %s\n", "CLOCKWISE");
        break;

    default:
        break;
    }

    rt_kprintf("sg90 get pulse is %d\n", cfg->pulse);
    rt_kprintf("sg90 get period is %d\n", cfg->period);
    rt_kprintf("sg90 get angle is %d\n", PULSE2ANGEL(cfg->pulse));

    return RT_EOK;
}

/*
 * @brief sg90_get_pulse 获取舵机当前脉冲宽度
 * @param cfg sg90_cfg_t 结构体指针
 * @return 当前脉冲宽度，单位为纳秒ns
 *         RT_ERROR 如果未找到设备
 */
int sg90_get_pulse(sg90_cfg_t cfg)
{
    int *pulse = &cfg->pulse;
    if (cfg->name == RT_NULL)
    {
        rt_kprintf("not find the sg90 device\n");

        return RT_ERROR;
    }
    else
    {

        return cfg->pulse;
    }
}

/*
 * @brief sg90_get_angle 获取舵机当前角度
 * @param cfg sg90_cfg_t 结构体指针
 * @return 当前角度，单位为度
 *         RT_ERROR 如果未找到设备
 */
int sg90_get_angle(sg90_cfg_t cfg)
{
    if (cfg->name == RT_NULL)
    {
        rt_kprintf("not find the sg90 device\n");

        return RT_ERROR;
    }
    else
    {
        return PULSE2ANGEL(sg90_get_pulse(cfg));
    }
}

//
/* 导出到 msh 命令列表中 */
// INIT_DEVICE_EXPORT(init_sg90);
