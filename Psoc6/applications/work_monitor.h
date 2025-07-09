#ifndef WORK_MONITOR_H
#define WORK_MONITOR_H

#include <stdint.h>
#include <rtthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

// 模式定义
#define MODE_DEMO 0
#define MODE_REAL 1

    // typedef struct
    // {
    //     int energy;      // 能量值
    //     int carefulness; // 小心值
    //     int work_score;  // 工作分数
    // } monitor_score_t;
    // extern monitor_score_t monitor_score;

    typedef struct
    {
        int work_time; // 工作时间（秒）
        int rest_time; // 休息时间（秒）
        int loop;      // 循环次数
    } monitor_setting_t;
    extern monitor_setting_t monitor_setting;

    typedef struct
    {
        rt_int32_t sum_time;            // 总时间（秒）
        rt_int32_t work_duration;       // 工作持续时间（秒）
        rt_int32_t phone_duration;      // 玩手机持续时间（秒）
        rt_bool_t is_working;           // 是否正在工作
    } monitor_data_t;
    extern monitor_data_t monitor_data;

    extern uint8_t mode;
    extern float energy;               // 初始能量值
    extern float work_score;           // 初始工作分数
    extern float carefulness;          // 初始专注度

    // 计算分数，根据当前模式和输入的工作/玩手机时间返回分数
    float calc_score(int work_time, int phone_time);

    void monitor_calc_start(void);
    void monitor_calc_stop(void);

#ifdef __cplusplus
}
#endif

#endif // WORK_MONITOR_H
