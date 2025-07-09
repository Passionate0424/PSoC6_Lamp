#ifndef __SMTP_MAIL_H__
#define __SMTP_MAIL_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void send_mail(void);

    extern uint8_t send_enable;
    extern rt_mutex_t mail_mutex;
    extern rt_sem_t smtp_send_sem;

#ifdef __cplusplus
}
#endif

#endif // __SMTP_MAIL_H__
