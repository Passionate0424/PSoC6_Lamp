#include "smtp_client.h"
#include "rtthread.h"
#include "smtp_mail.h"
#include <dfs_posix.h> // 添加头文件，确保文件操作可用

// 若使用TLS加密则需要更大的堆栈空间
#ifdef SMTP_CLIENT_USING_TLS
#define SMTP_CLIENT_THREAD_STACK_SIZE 23000
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
extern rt_mutex_t image_file_mutex; // 声明图片互斥锁

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
char *content =
    "<div style=\"font-family:微软雅黑,Arial,sans-serif;font-size:18px;color:#222;line-height:1.7;\">"
    "<div style=\"font-size:22px;color:#333;font-weight:bold;margin-bottom:10px;\">PSoC6 RT-Thread台灯自动告警</div>"
    "<div style=\"color:#444;\">该邮件由您的PSoC6 RT-Thread台灯自动发送</div>"
    "<div style=\"color:#e53935;font-size:16px;margin:8px 0;\">~~~~~~~~~~~~~请勿回复~~~~~~~~~~~~~</div>"
    "<div style=\"color:#d32f2f;font-size:18px;\">我是您的守卫助手，您的工作区域有人闯入</div>"
    "<div style=\"color:#1976d2;\">监控直播已自动打开，您可进入ThingsPanel APP或点击下方链接实时观看监控画面</div>"
    "<div style=\"margin:8px 0;\"><a href=\"http://49.234.22.239:9000/players/srs_player.html?schema=http&port=9000&api=9000\" style=\"color:#1565c0;font-size:16px;\">点击观看</a></div>"
    "<div style=\"color:#388e3c;\">下方附件为拍摄的闯入者图片</div>"
    "<div style=\"color:#888;font-size:14px;margin-top:12px;\">based on &rarr; RT-Thread</div>"
    "<div style=\"color:#888;font-size:14px;\">based on &rarr; SMTP_CLIENT</div>"
    "</div>";

uint8_t send_enable = 1;

// 检查图片文件是否存在且非空
static int check_image_file()
{
    int fd = open("/recv_image.jpg", O_RDONLY, 0);
    if (fd < 0)
    {
        rt_kprintf("图片文件不存在！\n");
        return 0;
    }
    int size = lseek(fd, 0, SEEK_END);
    close(fd);
    if (size <= 0)
    {
        rt_kprintf("图片文件为空！\n");
        return 0;
    }
    return 1;
}

static void smtp_thread(void *param)
{

    // 添加收件人1
    smtp_add_receiver("1852282973@qq.com");

    while (1)
    {
        // 等待发送信号量
        rt_sem_take(smtp_send_sem, RT_WAITING_FOREVER);

        if (check_image_file())
        {
            // 发送前加锁，保护图片读取
            rt_mutex_take(image_file_mutex, RT_WAITING_FOREVER);
            // 添加图片附件并检查返回值
            if (smtp_add_attachment("/recv_image.jpg", "recv_image.jpg") != 0)
            {
                LOG_E("添加附件失败，邮件未发送！");
                smtp_clear_attachments();
                rt_mutex_release(image_file_mutex);
                continue;
            }
            // 发送邮件
            send_enable = 0;
            LOG_D("start to send mail");
            if (smtp_send_mail(SMTP_SUBJECT, content) == 0)
            {
                LOG_I("send mail success!");
            }
            else
            {
                LOG_E("send mail fail!");
            }
            smtp_clear_attachments();
            // 发送后解锁
            rt_mutex_release(image_file_mutex);
        }
        else
        {
            LOG_E("图片不存在或为空，邮件未发送！");
        }
        // 防止频繁发送
        rt_thread_mdelay(30000);
        send_enable = 1;
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
    smtp_client_tid = rt_thread_create("smtp", smtp_thread, RT_NULL, SMTP_CLIENT_THREAD_STACK_SIZE, 10, 5000);
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
    smtp_client_tid = rt_thread_create("smtp_send", smtp_thread, RT_NULL, SMTP_CLIENT_THREAD_STACK_SIZE, 25, 50);
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
