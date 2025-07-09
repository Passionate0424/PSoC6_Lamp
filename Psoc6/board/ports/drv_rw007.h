#ifndef __DRV_RW007_H__
#define __DRV_RW007_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
} beijing_time_t;

#ifdef __cplusplus
}
#endif

#endif /* __DRV_RW007_H__ */