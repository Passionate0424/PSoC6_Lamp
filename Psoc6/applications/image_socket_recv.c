// image_socket_recv.c
// RT-Thread 下通过 socket 接收图片并保存到文件的示例

#include <rtthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <dfs_posix.h> // RT-Thread 文件系统API
#include "smtp_mail.h" // 确保包含头文件声明
#include "uart1.h"

#define PORT 8080
#define BUF_SIZE 1024
#define IMG_SAVE_PATH "/recv_image.jpg"

extern rt_sem_t smtp_send_sem; // 声明外部信号量
rt_mutex_t image_file_mutex;   // 声明图片互斥锁
extern int rtmp_status; // 声明外部 RTMP 状态变量

static void image_recv_thread(void *parameter)
{
    int sock, conn, len, img_fd;
    struct sockaddr_in server, client;
    char buf[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        rt_kprintf("Socket create failed!\n");
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        rt_kprintf("Bind failed!\n");
        closesocket(sock);
        return;
    }
    listen(sock, 2);
    rt_kprintf("Image socket server listening on port %d...\n", PORT);

    while (1)
    {
        socklen_t clen = sizeof(client);
        conn = accept(sock, (struct sockaddr *)&client, &clen);
        if (conn < 0)
        {
            rt_kprintf("Accept failed!\n");
            continue;
        }
        rt_kprintf("Client connected. Start receiving image...\n");
        // 加锁，保护图片写入
        rt_mutex_take(image_file_mutex, RT_WAITING_FOREVER);
        img_fd = open(IMG_SAVE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0);
        if (img_fd < 0)
        {
            rt_kprintf("Open file failed!\n");
            closesocket(conn);
            rt_mutex_release(image_file_mutex);
            continue;
        }
        while ((len = recv(conn, buf, BUF_SIZE, 0)) > 0)
        {
            write(img_fd, buf, len);
        }
        close(img_fd);
        // 获取图片文件大小并打印
        int fd = open(IMG_SAVE_PATH, O_RDONLY, 0);
        if (fd >= 0)
        {
            int size = lseek(fd, 0, SEEK_END);
            close(fd);
            rt_kprintf("Image file size: %d bytes\n", size);
        }
        else
        {
            rt_kprintf("Failed to open image file for size check!\n");
        }
        closesocket(conn);
        rt_kprintf("Image received and saved to %s\n", IMG_SAVE_PATH);
        // 解锁，允许邮件线程读取
        rt_mutex_release(image_file_mutex);
        // 接收图片后释放信号量，触发邮件发送线程
        rtmp_status = 1; 
        uart_send_protocol("c", 11); // 发送控制字c:11 暂停守卫模式
        rt_thread_mdelay(2000);      // 延时100ms
        uart_send_protocol("c", 6);  // 发送控制字c:16开启推流
        rt_sem_release(smtp_send_sem);
    }
    closesocket(sock);
}

int start_image_recv(void)
{
    image_file_mutex = rt_mutex_create("image_file_mutex", RT_IPC_FLAG_FIFO);
    if (image_file_mutex == RT_NULL)
    {
        rt_kprintf("Failed to create image file mutex!\n");
        return -1;
    }
    rt_thread_t tid = rt_thread_create("img_recv", image_recv_thread, RT_NULL, 20480, 15, 10);
    if (tid)
        rt_thread_startup(tid);
    return 0;
}
INIT_APP_EXPORT(start_image_recv);
