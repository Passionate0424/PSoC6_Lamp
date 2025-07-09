
#include "smtp_client.h"
#include "rtthread.h"
#include "smtp_mail.h"

// 若使用TLS加密则需要更大的堆栈空间
#ifdef SMTP_CLIENT_USING_TLS
#define SMTP_CLIENT_THREAD_STACK_SIZE 20480
#else
#define SMTP_CLIENT_THREAD_STACK_SIZE 4096
#endif

#define DBG_ENABLE
#define DBG_LEVEL 3
#define DBG_COLOR
#define DBG_SECTION_NAME "SMTP_EXAMPLE"
#include "rtdbg.h"

rt_mutex_t mail_mutex = RT_NULL;
rt_sem_t smtp_send_sem = RT_NULL;

/*
 *邮件信息相关宏定义
 */
// smtp 服务器域名
#define SMTP_SERVER_ADDR "smtp.qq.com"
// smtp 服务器端口号
#define SMTP_SERVER_PORT "587"
// smtp 登录用户名
#define SMTP_USERNAME "1852282973@qq.com"
// smtp 登录密码（或凭证）
#define SMTP_PASSWORD "uoyycpnlescidadc"
// 邮件主题
#define SMTP_SUBJECT "SMTP TEST"

// 邮件内容
char *content = "THIS IS SMTP TEST\r\n"
                "HELLO SMTP\r\n"
                "--------------------------------------\r\n"
                "based on --->   RT-Thread\r\n"
                "based on ---> SMTP_CLIENT\r\n";

uint8_t send_enable = 0;

static void smtp_thread(void *param)
{

    // 添加收件人1
    smtp_add_receiver("1852282973@qq.com");

    while (1)
    {
        // 等待发送信号量
        rt_sem_take(smtp_send_sem, RT_WAITING_FOREVER);

        //            smtp_add_attachment("/a.txt", "a.txt");
        //            smtp_add_attachment("/b.txt", "b.txt");
        // 发送邮件
        LOG_D("start to send mail");
        if (smtp_send_mail(SMTP_SUBJECT, content) == 0)
        {
            // 发送成功
            LOG_I("send mail success!");
        }
        else
        {
            // 发送失败
            LOG_E("send mail fail!");
        }
        // 清除附件
        smtp_clear_attachments();
        // 防止频繁发送
        rt_thread_mdelay(30000);
        send_enable = 0;
        // 释放互斥锁
        rt_mutex_release(mail_mutex);
    }
}

/*发送邮件函数
 *调用时需获取互斥锁 mail_mutex
 */
void send_mail(void)
{
    // smtp_add_attachment("/a.txt", "a.txt");
    // smtp_add_attachment("/b.txt", "b.txt");
    //  发送邮件
    LOG_D("start to send mail");
    if (smtp_send_mail(SMTP_SUBJECT, content) == 0)
    {
        // 发送成功
        LOG_I("send mail success!");
    }
    else
    {
        // 发送失败
        LOG_E("send mail fail!");
    }
    // 清除附件
    smtp_clear_attachments();
    rt_mutex_release(mail_mutex);
}

static int smtp_init(void)
{
    // 初始化smtp客户端
    smtp_client_init();
    // 设置服务器地址
    smtp_set_server_addr(SMTP_SERVER_ADDR, ADDRESS_TYPE_DOMAIN, SMTP_SERVER_PORT);
    // 设置服务器认证信息
    smtp_set_auth(SMTP_USERNAME, SMTP_PASSWORD);
    // 创建互斥锁
    mail_mutex = rt_mutex_create("mail_mutex", RT_IPC_FLAG_FIFO);
    if (mail_mutex == RT_NULL)
    {
        LOG_E("Failed to create mail mutex");
        return -RT_ERROR;
    }
    // 创建发送信号量
    smtp_send_sem = rt_sem_create("smtp_send_sem", 0, RT_IPC_FLAG_FIFO);
    if (smtp_send_sem == RT_NULL)
    {
        LOG_E("Failed to create smtp send semaphore");
        rt_mutex_delete(mail_mutex);
        return -RT_ERROR;
    }
    rt_thread_t smtp_client_tid;
    // 创建邮件发送线程（如果选择在主函数中直接调用邮件发送函数，需要注意主函数堆栈大小，必要时调大）
    smtp_client_tid = rt_thread_create("smtp", smtp_thread, RT_NULL, SMTP_CLIENT_THREAD_STACK_SIZE, 20, 5);
    if (smtp_client_tid != RT_NULL)
    {
        rt_thread_startup(smtp_client_tid);
    }
    return RT_EOK;
}
INIT_APP_EXPORT(smtp_init);

static int smtp_thread_entry(void)
{
    rt_thread_t smtp_client_tid;
    // 创建邮件发送线程（如果选择在主函数中直接调用邮件发送函数，需要注意主函数堆栈大小，必要时调大）·
    smtp_client_tid = rt_thread_create("smtp", smtp_thread, RT_NULL, SMTP_CLIENT_THREAD_STACK_SIZE, 20, 5);
    if (smtp_client_tid != RT_NULL)
    {
        rt_thread_startup(smtp_client_tid);
    }
    return RT_EOK;
}
INIT_APP_EXPORT(smtp_thread_entry);

int smtp_test(uint8_t argc, char *argv[])
{
    send_enable = 1;
    if (rt_mutex_trytake(mail_mutex) == RT_EOK)
    {
        // send_mail(); // 发送邮件
        rt_sem_release(smtp_send_sem); // 释放发送信号量，触发邮件发送线程
    }
    else
    {
        LOG_E("Failed to acquire mail mutex, another thread is sending mail");
    }
    return 0;
}
MSH_CMD_EXPORT(smtp_test, smtp test);


