#ifndef __GIMBAL_PID_H__
#define __GIMBAL_PID_H__

#include <rtthread.h>
#include <sg90.h>

#define GIMBAL_CENTER_X 320
#define GIMBAL_CENTER_Y 320
#define GIMBAL_MAX_ANGLE 180
#define GIMBAL_MIN_ANGLE 0

// PID参数结构体
typedef struct
{
    float kp;
    float ki;
    float kd;
    float prev_error;
    float integral;
    float deadband;     // 死区阈值
    float filter_alpha; // 一阶滤波系数 0~1
    float last_output;  // 上次输出（滤波用）
    // 卡尔曼滤波参数
    float kf_estimate; // 卡尔曼当前估计值
    float kf_p;        // 估计误差协方差
    float kf_q;        // 过程噪声
    float kf_r;        // 测量噪声
} servo_pid_t;

// 云台结构体，包含两个舵机和PID参数
typedef struct
{
    sg90_cfg_t servo_x;
    sg90_cfg_t servo_y;
    float angle_x;
    float angle_y;
    servo_pid_t pid_x;
    servo_pid_t pid_y;
} gimbal_t;

// 目标点结构体
typedef struct
{
    int x;
    int y;
} point_t;

extern point_t *my_target; // 目标点指针，用于传递给追踪任务
extern gimbal_t gimbal;    // 全局云台对象
extern rt_mutex_t gimbal_mutex;

void gimbal_start_tracking(void);
void gimbal_stop_tracking(void);

#endif // __GIMBAL_PID_H__
