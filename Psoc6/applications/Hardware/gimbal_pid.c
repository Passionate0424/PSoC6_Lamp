#include <rtthread.h>
#include <gimbal_pid.h>
#include <math.h>
#include <float.h>

#define GIMBAL_CENTER_X 320  // 屏幕中心X坐标
#define GIMBAL_CENTER_Y 320  // 屏幕中心Y坐标
#define GIMBAL_MAX_ANGLE 180 // 舵机最大角度
#define GIMBAL_MIN_ANGLE 0   // 舵机最小角度

static rt_thread_t gimbal_track_thread = RT_NULL; // 云台追踪线程

gimbal_t gimbal;                              // 全局云台对象
point_t *my_target = NULL;                    // 目标点指针，用于传递给追踪任务
sg90_cfg_t my_servo_x = &my_sg90_config0;     // X轴舵机配置
sg90_cfg_t my_servo_y = &my_sg90_config1;     // Y轴舵机配置
rt_bool_t gimbal_tracking_enabled = RT_FALSE; // 云台追踪状态
rt_mutex_t gimbal_mutex;                      // 云台互斥锁，用于保护共享资源

// 新增：滤波开关，默认开启
rt_bool_t gimbal_filter_enabled = RT_FALSE;

static int is_nan(float x)
{
    return x != x;
}

// 卡尔曼滤波函数，对输入值进行滤波
static float kalman_filter(servo_pid_t *pid, float measurement)
{
    // nan保护
    if (is_nan(pid->kf_estimate))
        pid->kf_estimate = 0.0f;
    if (is_nan(pid->kf_p))
        pid->kf_p = 1.0f;
    if (is_nan(pid->kf_q) || pid->kf_q <= 0.0f)
        pid->kf_q = 1e-6f;
    if (is_nan(pid->kf_r) || pid->kf_r <= 0.0f)
        pid->kf_r = 1e-6f;
    // 预测更新
    pid->kf_p += pid->kf_q;
    float denom = pid->kf_p + pid->kf_r;
    if (denom == 0.0f)
        denom = 1e-6f;
    float k = pid->kf_p / denom;
    pid->kf_estimate = pid->kf_estimate + k * (measurement - pid->kf_estimate);
    pid->kf_p = (1 - k) * pid->kf_p;
    // nan保护
    if (is_nan(pid->kf_estimate))
        pid->kf_estimate = 0.0f;
    if (is_nan(pid->kf_p))
        pid->kf_p = 1.0f;
    return pid->kf_estimate;
}

// 积分限幅宏
#define PID_INTEGRAL_LIMIT 1000.0f

// PID计算函数
static float pid_calculate(servo_pid_t *pid, float setpoint, float measured)
{
    float error = setpoint - measured;
    if (is_nan(error))
        error = 0.0f;
    // 根据滤波开关决定是否卡尔曼滤波误差
    if (gimbal_filter_enabled)
        error = kalman_filter(pid, error);
    // 死区控制
    if (fabsf(error) < pid->deadband)
    {
        pid->integral = 0;
        pid->last_output = 0;
        pid->prev_error = 0; // 新增，彻底清零
        return 0;
    }
    if (is_nan(pid->integral))
        pid->integral = 0.0f;
    if (is_nan(pid->prev_error))
        pid->prev_error = 0.0f;
    if (is_nan(pid->last_output))
        pid->last_output = 0.0f;
    pid->integral += error;
    // 积分防止饱和
    if (pid->integral > PID_INTEGRAL_LIMIT)
        pid->integral = PID_INTEGRAL_LIMIT;
    else if (pid->integral < -PID_INTEGRAL_LIMIT)
        pid->integral = -PID_INTEGRAL_LIMIT;
    float derivative = error - pid->prev_error;
    float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
    pid->prev_error = error;
    // 一阶低通滤波
    output = pid->filter_alpha * output + (1.0f - pid->filter_alpha) * pid->last_output;
    if (is_nan(output))
        output = 0.0f;
    pid->last_output = output;
    return output;
}

// 云台初始化，直接使用全局舵机配置
void gimbal_init(void)
{
    gimbal.servo_x = my_servo_x;
    gimbal.servo_y = my_servo_y;
    gimbal.angle_x = 90; // 初始角度设为90度
    gimbal.angle_y = 80;
    // 设置PID参数（可根据实际调整）
    gimbal.pid_x.kp = 0.0025f;
    gimbal.pid_x.ki = 0.0f;
    gimbal.pid_x.kd = 0.003f;
    gimbal.pid_x.prev_error = 0;
    gimbal.pid_x.integral = 0;
    // 追加死区和滤波器参数
    gimbal.pid_x.deadband = 100.0f;
    gimbal.pid_x.filter_alpha = 0.5f;

    gimbal.pid_y.kp = 0.0025f;
    gimbal.pid_y.ki = 0.0f;
    gimbal.pid_y.kd = 0.003f;
    gimbal.pid_y.prev_error = 0;
    gimbal.pid_y.integral = 0;
    // 追加死区和滤波器参数
    gimbal.pid_y.deadband = 100.0f;
    gimbal.pid_y.filter_alpha = 0.5f;
    // 卡尔曼滤波参数
    gimbal.pid_x.kf_estimate = 0.0f; // 初始估计值
    gimbal.pid_x.kf_p = 1.0f;        // 初始误差协方差
    gimbal.pid_x.kf_q = 0.05f;       // 过程噪声
    gimbal.pid_x.kf_r = 1.0f;        // 测量噪声
    // 卡尔曼滤波参数初始化
    gimbal.pid_y.kf_q = 0.05f;
    gimbal.pid_y.kf_r = 1.0f;
    gimbal.pid_y.kf_p = 1.0f;
    gimbal.pid_y.kf_estimate = 0.0f;
    gimbal.pid_y.last_output = 0.0f;

    // 按demo方式初始化舵机
    if (gimbal.servo_x->name == RT_NULL)
    {
        sg90_init(gimbal.servo_x, 1); // 初始化X轴舵机
    }
    if (gimbal.servo_y->name == RT_NULL)
    {
        sg90_init(gimbal.servo_y, 0); // 初始化Y轴舵机
    }
    sg90_set_angle(gimbal.servo_x, gimbal.angle_x); // 设置初始角度
    sg90_set_angle(gimbal.servo_y, gimbal.angle_y);
}

// 云台追踪函数，输入目标xy坐标
void gimbal_track(point_t target)
{
    // 计算目标点与中心点的偏差
    float error_x = (float)(GIMBAL_CENTER_X - target.x);
    float error_y = (float)(GIMBAL_CENTER_Y - target.y);
    // PID输出，计算角度增量
    float delta_x = pid_calculate(&gimbal.pid_x, 0, error_x);
    float delta_y = pid_calculate(&gimbal.pid_y, 0, error_y);
    // 只在有输出时更新舵机角度
    if (delta_x != 0)
        gimbal.angle_x += delta_x;
    if (delta_y != 0)
        gimbal.angle_y -= delta_y;
    // 限幅，防止超出舵机物理范围
    // X轴角度限制在30~100
    if (gimbal.angle_x > 160)
        gimbal.angle_x = 160;
    if (gimbal.angle_x < 20)
        gimbal.angle_x = 20;
    // Y轴角度限制在0~170
    if (gimbal.angle_y > 100)
        gimbal.angle_y = 100;
    if (gimbal.angle_y < 40)
        gimbal.angle_y = 40;
    // 控制舵机转动到新角度
    sg90_set_angle(gimbal.servo_x, gimbal.angle_x);
    sg90_set_angle(gimbal.servo_y, gimbal.angle_y);
}

// 云台追踪任务，循环调用追踪函数，直接用全局my_target
void gimbal_track_task(void *parameter)
{
    point_t last_target = {320, 320};
    rt_tick_t last_update_tick = rt_tick_get();
    while (1)
    {
        if (my_target)
        {
            // 判断目标是否变化
            if (my_target->x != last_target.x || my_target->y != last_target.y)
            {
                last_update_tick = rt_tick_get();
                last_target.x = my_target->x;
                last_target.y = my_target->y;
            }
            // 判断是否1秒内有更新
            if (rt_tick_get() - last_update_tick <= rt_tick_from_millisecond(1000))
            {
                gimbal_track(*my_target); // 追踪目标
            }
            // 否则2秒未更新，舵机不动
        }
        rt_thread_mdelay(10); // 20Hz刷新
    }
}

// 用户接口：初始化云台并启动追踪线程，直接用全局配置
void gimbal_tracking_init(void)
{
    gimbal_init(); // 初始化云台
    if (my_target == NULL)
    {
        my_target = rt_malloc(sizeof(point_t)); // 分配目标点内存
        if (my_target)
        {
            my_target->x = GIMBAL_CENTER_X; // 默认目标点为屏幕中心
            my_target->y = GIMBAL_CENTER_Y;
        }
    }
    if (gimbal_track_thread == RT_NULL)
    {
        gimbal_track_thread = rt_thread_create("gimbal_pid", gimbal_track_task, RT_NULL, 1024, 20, 10);
    }
    gimbal_mutex = rt_mutex_create("gimbal_mutex", RT_IPC_FLAG_FIFO); // 创建云台互斥锁
}
INIT_ENV_EXPORT(gimbal_tracking_init); // 初始化云台追踪

// 追踪线程开启接口，供外部调用（只需保证my_target已赋值）
void gimbal_start_tracking(void)
{
    my_target->x = GIMBAL_CENTER_X; // 重置目标点为中心
    my_target->y = GIMBAL_CENTER_Y;
    gimbal.angle_x = 90; // 设置初始角度
    gimbal.angle_y = 80; // 设置初始角度
    if (gimbal_tracking_enabled == RT_TRUE)
    {
        rt_kprintf("云台追踪已开启，无需重复启动！\n");
        return; // 如果已经在追踪中，直接返回
    }
    if (gimbal_track_thread == RT_NULL)
    {
        gimbal_track_thread = rt_thread_create("gimbal_pid", gimbal_track_task, RT_NULL, 1024, 20, 10);
    }
    if (gimbal_track_thread != RT_NULL)
    {
        rt_thread_startup(gimbal_track_thread);
        gimbal_tracking_enabled = RT_TRUE; // 设置追踪状态为开启
    }
}
// 停止追踪线程
void gimbal_stop_tracking()
{
    if (gimbal_track_thread != RT_NULL)
    {
        gimbal_tracking_enabled = RT_FALSE;    // 设置追踪状态为关闭
        rt_thread_delete(gimbal_track_thread); // 删除追踪线程
        gimbal_track_thread = RT_NULL;         // 重置线程指针
    }
    // 可在此处添加舵机复位逻辑
    // sg90_set_angle(gimbal.servo_x, 90); // 复位X轴舵机
    // sg90_set_angle(gimbal.servo_y, 90); // 复位Y轴舵机
}

// msh命令：调整云台PID参数
// 用法：gimbal_pid_set x/y kp ki kd
#include <stdlib.h>
#include <string.h>
// 扩展后的gimbal_pid_set命令
// 用法: gimbal_pid_set x/y 参数名 参数值
static void gimbal_pid_set(int argc, char **argv)
{
    if (argc != 4)
    {
        rt_kprintf("用法: gimbal_pid_set x/y 参数名 参数值\n");
        rt_kprintf("可调参数: kp ki kd deadband filter_alpha kf_q kf_r\n");
        return;
    }
    servo_pid_t *pid = NULL;
    if (strcmp(argv[1], "x") == 0)
        pid = &gimbal.pid_x;
    else if (strcmp(argv[1], "y") == 0)
        pid = &gimbal.pid_y;
    else
    {
        rt_kprintf("参数错误: 请选择x或y\n");
        return;
    }
    float value = (float)atof(argv[3]);
    if (strcmp(argv[2], "kp") == 0)
        pid->kp = value;
    else if (strcmp(argv[2], "ki") == 0)
        pid->ki = value;
    else if (strcmp(argv[2], "kd") == 0)
        pid->kd = value;
    else if (strcmp(argv[2], "deadband") == 0)
        pid->deadband = value;
    else if (strcmp(argv[2], "filter_alpha") == 0)
        pid->filter_alpha = value;
    else if (strcmp(argv[2], "kf_q") == 0)
        pid->kf_q = value;
    else if (strcmp(argv[2], "kf_r") == 0)
        pid->kf_r = value;
    else
    {
        rt_kprintf("参数名错误: 只支持kp ki kd deadband filter_alpha kf_q kf_r\n");
        return;
    }
    rt_kprintf("%c轴PID参数%s已设置为%.3f\n", argv[1][0], argv[2], value);
}
MSH_CMD_EXPORT(gimbal_pid_set, 设置云台PID参数 : gimbal_pid_set x / y 参数名 参数值);

// 参数显示命令
// 用法: gimbal_pid_show x/y
static void gimbal_pid_show(int argc, char **argv)
{
    if (argc != 2)
    {
        rt_kprintf("用法: gimbal_pid_show x/y\n");
        return;
    }
    servo_pid_t *pid = NULL;
    if (strcmp(argv[1], "x") == 0)
        pid = &gimbal.pid_x;
    else if (strcmp(argv[1], "y") == 0)
        pid = &gimbal.pid_y;
    else
    {
        rt_kprintf("参数错误: 请选择x或y\n");
        return;
    }
    rt_kprintf("%c轴PID参数如下：\n", argv[1][0]);
    rt_kprintf("kp=%.3f ki=%.3f kd=%.3f\n", pid->kp, pid->ki, pid->kd);
    rt_kprintf("deadband=%.3f filter_alpha=%.3f\n", pid->deadband, pid->filter_alpha);
    rt_kprintf("kf_q=%.3f kf_r=%.3f\n", pid->kf_q, pid->kf_r);
    rt_kprintf("integral=%.3f prev_error=%.3f last_output=%.3f\n", pid->integral, pid->prev_error, pid->last_output);
    rt_kprintf("kf_estimate=%.3f kf_p=%.3f\n", pid->kf_estimate, pid->kf_p);
}
MSH_CMD_EXPORT(gimbal_pid_show, 显示云台PID参数 : gimbal_pid_show x / y);

// MSH命令：滤波开关设置
// 用法: gimbal_filter_set on/off
static void gimbal_filter_set(int argc, char **argv)
{
    if (argc != 2)
    {
        rt_kprintf("用法: gimbal_filter_set on/off\n");
        return;
    }
    if (strcmp(argv[1], "on") == 0)
    {
        gimbal_filter_enabled = RT_TRUE;
        rt_kprintf("云台PID滤波已开启\n");
    }
    else if (strcmp(argv[1], "off") == 0)
    {
        gimbal_filter_enabled = RT_FALSE;
        rt_kprintf("云台PID滤波已关闭\n");
    }
    else
    {
        rt_kprintf("参数错误: 请输入on或off\n");
    }
}
MSH_CMD_EXPORT(gimbal_filter_set, 设置云台PID滤波开关 : gimbal_filter_set on / off);

// MSH命令：显示滤波状态
static void gimbal_filter_show(int argc, char **argv)
{
    RT_UNUSED(argc);
    RT_UNUSED(argv);
    rt_kprintf("云台PID滤波当前状态: %s\n", gimbal_filter_enabled ? "开启" : "关闭");
}
MSH_CMD_EXPORT(gimbal_filter_show, 显示云台PID滤波状态 : gimbal_filter_show);

// MSH命令：自动调参
// 用法: gimbal_pid_autotune x/y [step] [max_iter]
static void gimbal_pid_autotune_single(int argc, char **argv)
{
    if (argc < 2 || argc > 4)
    {
        rt_kprintf("用法: gimbal_pid_autotune x/y [step] [max_iter]\n");
        return;
    }
    servo_pid_t *pid = NULL;
    if (strcmp(argv[1], "x") == 0)
        pid = &gimbal.pid_x;
    else if (strcmp(argv[1], "y") == 0)
        pid = &gimbal.pid_y;
    else
    {
        rt_kprintf("参数错误: 请选择x或y\n");
        return;
    }
    float step = 0.0005f;
    int max_iter = 10;
    if (argc >= 3)
        step = (float)atof(argv[2]);
    if (argc == 4)
        max_iter = atoi(argv[3]);
    float best_kp = pid->kp, best_ki = pid->ki, best_kd = pid->kd;
    float min_error = FLT_MAX;
    float orig_kp = pid->kp, orig_ki = pid->ki, orig_kd = pid->kd;
    float test_setpoint = 0.0f;
    float test_measured = 100.0f; // 假设初始偏差较大
    for (int i = 0; i < max_iter; i++)
    {
        float test_kp = orig_kp + (i - max_iter / 2) * step;
        if (test_kp < 0)
            test_kp = 0;
        pid->kp = test_kp;
        pid->ki = orig_ki;
        pid->kd = orig_kd;
        float sum_error = 0;
        float measured = test_measured;
        pid->integral = 0;
        pid->prev_error = 0;
        pid->last_output = 0;
        // 简单仿真10步响应
        for (int t = 0; t < 10; t++)
        {
            float out = pid_calculate(pid, test_setpoint, measured);
            measured += out; // 假设系统响应=输出
            sum_error += fabsf(test_setpoint - measured);
        }
        if (sum_error < min_error)
        {
            min_error = sum_error;
            best_kp = test_kp;
        }
    }
    pid->kp = best_kp;
    rt_kprintf("自动调参完成，最佳kp=%.5f，误差=%.3f\n", best_kp, min_error);
}
MSH_CMD_EXPORT(gimbal_pid_autotune_single, 自动调参 : gimbal_pid_autotune x / y[step][max_iter]);

// MSH命令：自动调参（扩展为kp、ki、kd三参数）
// 用法: gimbal_pid_autotune x/y [kp_step] [ki_step] [kd_step] [max_iter]
static void gimbal_pid_autotune(int argc, char **argv)
{
    if (argc < 2 || argc > 6)
    {
        rt_kprintf("用法: gimbal_pid_autotune x/y [kp_step] [ki_step] [kd_step] [max_iter]\n");
        return;
    }
    servo_pid_t *pid = NULL;
    if (strcmp(argv[1], "x") == 0)
        pid = &gimbal.pid_x;
    else if (strcmp(argv[1], "y") == 0)
        pid = &gimbal.pid_y;
    else
    {
        rt_kprintf("参数错误: 请选择x或y\n");
        return;
    }
    float kp_step = 0.0005f, ki_step = 0.0001f, kd_step = 0.0002f;
    int max_iter = 5;
    if (argc >= 3)
        kp_step = (float)atof(argv[2]);
    if (argc >= 4)
        ki_step = (float)atof(argv[3]);
    if (argc >= 5)
        kd_step = (float)atof(argv[4]);
    if (argc == 6)
        max_iter = atoi(argv[5]);
    float orig_kp = pid->kp, orig_ki = pid->ki, orig_kd = pid->kd;
    float best_kp = orig_kp, best_ki = orig_ki, best_kd = orig_kd;
    float min_error = FLT_MAX;
    float test_setpoint = 0.0f;
    float test_measured = 100.0f;
    for (int i = 0; i < max_iter; i++)
    {
        float test_kp = orig_kp + (i - max_iter / 2) * kp_step;
        if (test_kp < 0)
            test_kp = 0;
        for (int j = 0; j < max_iter; j++)
        {
            float test_ki = orig_ki + (j - max_iter / 2) * ki_step;
            if (test_ki < 0)
                test_ki = 0;
            for (int k = 0; k < max_iter; k++)
            {
                float test_kd = orig_kd + (k - max_iter / 2) * kd_step;
                if (test_kd < 0)
                    test_kd = 0;
                pid->kp = test_kp;
                pid->ki = test_ki;
                pid->kd = test_kd;
                float sum_error = 0;
                float measured = test_measured;
                pid->integral = 0;
                pid->prev_error = 0;
                pid->last_output = 0;
                for (int t = 0; t < 10; t++)
                {
                    float out = pid_calculate(pid, test_setpoint, measured);
                    measured += out;
                    sum_error += fabsf(test_setpoint - measured);
                }
                if (sum_error < min_error)
                {
                    min_error = sum_error;
                    best_kp = test_kp;
                    best_ki = test_ki;
                    best_kd = test_kd;
                }
            }
        }
    }
    pid->kp = best_kp;
    pid->ki = best_ki;
    pid->kd = best_kd;
    rt_kprintf("自动调参完成，最佳kp=%.5f ki=%.5f kd=%.5f，误差=%.3f\n", best_kp, best_ki, best_kd, min_error);
}
MSH_CMD_EXPORT(gimbal_pid_autotune, 自动调参 : gimbal_pid_autotune x / y[kp_step][ki_step][kd_step][max_iter]);

// 真实舵机驱动自动调参
// 用法: gimbal_pid_autotune_real x/y [kp_step] [ki_step] [kd_step] [max_iter] [wait_ms]
static void gimbal_pid_autotune_real(int argc, char **argv)
{
    if (argc < 2 || argc > 7)
    {
        rt_kprintf("用法: gimbal_pid_autotune_real x/y [kp_step] [ki_step] [kd_step] [max_iter] [wait_ms]\n");
        return;
    }
    servo_pid_t *pid = NULL;
    if (strcmp(argv[1], "x") == 0)
        pid = &gimbal.pid_x;
    else if (strcmp(argv[1], "y") == 0)
        pid = &gimbal.pid_y;
    else
    {
        rt_kprintf("参数错误: 请选择x或y\n");
        return;
    }
    // 新增：确保追踪线程已开启
    gimbal_start_tracking();
    float kp_step = 0.0005f, ki_step = 0.0001f, kd_step = 0.0002f;
    int max_iter = 3;
    int wait_ms = 1000;
    if (argc >= 3)
        kp_step = (float)atof(argv[2]);
    if (argc >= 4)
        ki_step = (float)atof(argv[3]);
    if (argc >= 5)
        kd_step = (float)atof(argv[4]);
    if (argc >= 6)
        max_iter = atoi(argv[5]);
    if (argc == 7)
        wait_ms = atoi(argv[6]);
    float orig_kp = pid->kp, orig_ki = pid->ki, orig_kd = pid->kd;
    float best_kp = orig_kp, best_ki = orig_ki, best_kd = orig_kd;
    float min_error = FLT_MAX;
    for (int i = 0; i < max_iter; i++)
    {
        float test_kp = orig_kp + (i - max_iter / 2) * kp_step;
        if (test_kp < 0)
            test_kp = 0;
        for (int j = 0; j < max_iter; j++)
        {
            float test_ki = orig_ki + (j - max_iter / 2) * ki_step;
            if (test_ki < 0)
                test_ki = 0;
            for (int k = 0; k < max_iter; k++)
            {
                float test_kd = orig_kd + (k - max_iter / 2) * kd_step;
                if (test_kd < 0)
                    test_kd = 0;
                // 设置PID参数
                pid->kp = test_kp;
                pid->ki = test_ki;
                pid->kd = test_kd;
                pid->integral = 0;
                pid->prev_error = 0;
                pid->last_output = 0;
                rt_kprintf("[调参] 测试kp=%.5f ki=%.5f kd=%.5f, 等待%ums...\n", test_kp, test_ki, test_kd, wait_ms);
                rt_thread_mdelay(wait_ms);
                // 采集当前误差
                float error = 0;
                if (pid == &gimbal.pid_x)
                    error = fabsf(GIMBAL_CENTER_X - my_target->x);
                else
                    error = fabsf(GIMBAL_CENTER_Y - my_target->y);
                rt_kprintf("[调参] 当前误差=%.3f\n", error);
                if (error < min_error)
                {
                    min_error = error;
                    best_kp = test_kp;
                    best_ki = test_ki;
                    best_kd = test_kd;
                    rt_kprintf("[调参] 新最优参数: kp=%.5f ki=%.5f kd=%.5f 误差=%.3f\n", best_kp, best_ki, best_kd, min_error);
                }
                rt_thread_mdelay(200);
            }
        }
    }
    pid->kp = best_kp;
    pid->ki = best_ki;
    pid->kd = best_kd;
    rt_kprintf("真实舵机调参完成，最佳kp=%.5f ki=%.5f kd=%.5f，误差=%.3f\n", best_kp, best_ki, best_kd, min_error);
}
MSH_CMD_EXPORT(gimbal_pid_autotune_real, 真实舵机调参 : gimbal_pid_autotune_real x / y[kp_step][ki_step][kd_step][max_iter][wait_ms]);
