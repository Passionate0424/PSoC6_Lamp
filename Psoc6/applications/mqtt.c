// 周期性上传数据
// 环境温度、湿度、CO2、tvoc、光照强度、气压
// 人体存在状态、心率、呼吸率
// 环境评分预测值、温湿度预测值

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <rtthread.h>
#include "uart1.h"
#include "led_brightness_control.h"
#include "events_init.h"
#include "mqtt.h"
#define DBG_ENABLE
#define DBG_SECTION_NAME "mqtt.sample"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#include "paho_mqtt.h"

// ================== 传感器数据接口声明 ==================
// 这些变量在其他模块定义，这里用extern声明以便直接读取实时数据
extern uint8_t human_presence_status; // 人体存在状态（0:无人，1:有人）
extern uint8_t respiration_value;     // 呼吸频率（次/分钟）
extern uint8_t heart_rate_value;      // 心率值（bpm）
extern float current_light_intensity; // 当前光照强度（lux）
extern float Air_Data[3];             // [0]:温度(°C) [1]:气压(hPa) [2]:海拔(m)
extern float T_H_Data[2];             // [0]:温度(°C) [1]:湿度(%)
extern int tvoc;                      // 总挥发性有机物浓度（ppb）
extern int eco2;                      // 二氧化碳浓度（ppm）
extern rt_bool_t led_status;
// 预测值
extern float temp_pre; // 温度预测值(°C)
extern float hum_pre;  // 湿度预测值(%)
extern float env_pre;  // 环境模型评分

// ================== 周期上传线程实现 ==================
static rt_thread_t mqtt_upload_thread = RT_NULL;
static int mqtt_upload_thread_running = 0;
static struct rt_semaphore mqtt_upload_sem; // 信号量用于同步上传线程

int rtmp_status = 0; // RTMP状态，0:推流关 1：关
/**
 * MQTT URI farmat:
 * domain mode
 * tcp://iot.eclipse.org:1883
 *
 * ipv4 mode
 * tcp://192.168.10.1:1883
 * ssl://192.168.10.1:1884
 *
 * ipv6 mode
 * tcp://[fe80::20c:29ff:fe9a:a07e]:1883
 * ssl://[fe80::20c:29ff:fe9a:a07e]:1884
 */
#define MQTT_URI "tcp://49.234.22.239:1883"
#define MQTT_USERNAME "b4df2343-2cb5-99be-b83" // 代理服务器用户名
#define MQTT_PASSWORD "6770a10"                // 代理服务器密码
#define MQTT_SUBTOPIC "devices/telemetry/control/65c4faab-6d8f-bc47-cdde-119ff59c39bc"
#define MQTT_PUBTOPIC "devices/telemetry"
#define MQTT_WILLMSG "Goodbye!"

/* define MQTT client context */
static MQTTClient client;
static int is_started = 0;

static void mqtt_sub_callback(MQTTClient *c, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt sub callback: %.*s %.*s",
          msg_data->topicName->lenstring.len,
          msg_data->topicName->lenstring.data,
          msg_data->message->payloadlen,
          (char *)msg_data->message->payload);
}

static void mqtt_sub_default_callback(MQTTClient *c, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt sub default callback: %.*s %.*s",
          msg_data->topicName->lenstring.len,
          msg_data->topicName->lenstring.data,
          msg_data->message->payloadlen,
          (char *)msg_data->message->payload);
}

static void mqtt_connect_callback(MQTTClient *c)
{
    LOG_D("inter mqtt_connect_callback!");
}

static void mqtt_online_callback(MQTTClient *c)
{
    LOG_D("inter mqtt_online_callback!");
    rt_sem_release(&mqtt_upload_sem);
}

static void mqtt_offline_callback(MQTTClient *c)
{
    LOG_D("inter mqtt_offline_callback!");
}

static void mqtt_control_callback(MQTTClient *c, MessageData *msg_data)
{
    rt_kprintf("==========================mqtt_control_callback called\n");
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt control callback: %.*s %.*s",
          msg_data->topicName->lenstring.len,
          msg_data->topicName->lenstring.data,
          msg_data->message->payloadlen,
          (char *)msg_data->message->payload);
    // 解析JSON，提取led_switch字段
    const char *payload = (char *)msg_data->message->payload;
    int led_switch = -1;
    // 简单查找 led_switch 字段（假设格式为{"led_switch":0/1,...}）
    const char *key = "\"led_switch\":";
    const char *key2 = "\"rtmp_switch\":";
    char *found = strstr(payload, key);
    char *found2 = strstr(payload, key2);
    if (found)
    {
        found += strlen(key);
        led_switch = atoi(found);
        if (led_switch == 0)
        {
            led_request_off(); // 调用LED控制函数关闭LED
            LOG_I("LED turned off");
        }
        else if (led_switch == 1)
        {
            led_request_on(); // 调用LED控制函数打开LED
            LOG_I("LED turned on");
        }
        else
        {
            LOG_E("Invalid led_switch value: %d", led_switch);
            return;
        }
        LOG_I("Set led_status to %d", led_status);
    }
    if (found2)
    {
        found2 += strlen(key2);
        rtmp_status = atoi(found2);
        if (rtmp_status == 0)
        {
            if(guard_status == 1)
            {
                uart_send_protocol("c", 16); // 发送控制字c:16关闭推流
                rt_thread_mdelay(1000); // 延时100ms
                uart_send_protocol("c", 1); // 发送控制字c:11 继续守卫模式
            }
            LOG_I("RTMP stream turned off");
        }
        else if (rtmp_status == 1)
        {
            LOG_I("RTMP stream turned on");
            if(guard_status == 1)
            {
                uart_send_protocol("c", 11); // 发送控制字c:11 暂停守卫模式
                rt_thread_mdelay(1000); // 延时100ms
                uart_send_protocol("c", 6); // 发送控制字c:16开启推流
            }
        }
        else
        {
            LOG_E("Invalid rtmp_switch value: %d", rtmp_status);
            return;
        }
    }
}

static int mqtt_start(int argc, char **argv)
{
    /* init condata param by using MQTTPacket_connectData_initializer */
    MQTTPacket_connectData condata = MQTTPacket_connectData_initializer;
    static char cid[20] = {0};

    if (argc != 1)
    {
        rt_kprintf("mqtt_start    --start a mqtt worker thread.\n");
        return -1;
    }

    if (is_started)
    {
        LOG_E("mqtt client is already connected.");
        return -1;
    }
    /* config MQTT context param */
    {
        client.isconnected = 0;
        client.uri = &MQTT_URI;

        /* generate the random client ID */
        rt_snprintf(cid, sizeof(cid), "rtthread%d", rt_tick_get());
        /* config connect param */
        rt_memcpy(&client.condata, &condata, sizeof(condata));
        client.condata.clientID.cstring = cid;
        client.condata.keepAliveInterval = 30;
        client.condata.cleansession = 1;
        client.condata.username.cstring = MQTT_USERNAME;
        client.condata.password.cstring = MQTT_PASSWORD;

        /* config MQTT will param. */
        client.condata.willFlag = 1;
        client.condata.will.qos = 1;
        client.condata.will.retained = 0;
        client.condata.will.topicName.cstring = MQTT_PUBTOPIC;
        client.condata.will.message.cstring = MQTT_WILLMSG;

        /* rt_malloc buffer. */
        client.buf_size = client.readbuf_size = 1024;
        client.buf = rt_calloc(1, client.buf_size);
        client.readbuf = rt_calloc(1, client.readbuf_size);
        if (!(client.buf && client.readbuf))
        {
            LOG_E("no memory for MQTT client buffer!");
            return -1;
        }

        /* set event callback function */
        client.connect_callback = mqtt_connect_callback;
        client.online_callback = mqtt_online_callback;
        client.offline_callback = mqtt_offline_callback;

        /* set subscribe table and event callback */
        client.messageHandlers[0].topicFilter = rt_strdup(MQTT_SUBTOPIC);
        client.messageHandlers[0].callback = mqtt_control_callback;
        client.messageHandlers[0].qos = QOS1;
        // // 新增订阅设备控制主题
        // client.messageHandlers[1].topicFilter = rt_strdup("devices/telemetry/control/65c4faab-6d8f-bc47-cdde-119ff59c39bc");
        // client.messageHandlers[1].callback = mqtt_control_callback;
        // client.messageHandlers[1].qos = QOS1;

        /* set default subscribe event callback */
        client.defaultMessageHandler = mqtt_sub_default_callback;
    }

    /* run mqtt client */
    paho_mqtt_start(&client);
    is_started = 1;

    return 0;
}

static int mqtt_stop(int argc, char **argv)
{
    if (argc != 1)
    {
        rt_kprintf("mqtt_stop    --stop mqtt worker thread and free mqtt client object.\n");
    }

    is_started = 0;

    // 新增：释放动态分配的内存，防止内存泄漏
    if (client.buf)
    {
        rt_free(client.buf);
        client.buf = NULL;
    }
    if (client.readbuf)
    {
        rt_free(client.readbuf);
        client.readbuf = NULL;
    }
    if (client.messageHandlers[0].topicFilter)
    {
        rt_free(client.messageHandlers[0].topicFilter);
        client.messageHandlers[0].topicFilter = NULL;
    }

    return paho_mqtt_stop(&client);
}

static int mqtt_publish(int argc, char **argv)
{
    if (is_started == 0)
    {
        LOG_E("mqtt client is not connected.");
        return -1;
    }

    if (argc == 2)
    {
        paho_mqtt_publish(&client, QOS1, MQTT_PUBTOPIC, argv[1]);
    }
    else if (argc == 3)
    {
        paho_mqtt_publish(&client, QOS1, argv[1], argv[2]);
    }
    else
    {
        rt_kprintf("mqtt_publish <topic> [message]  --mqtt publish message to specified topic.\n");
        return -1;
    }

    return 0;
}

static void mqtt_new_sub_callback(MQTTClient *client, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt new subscribe callback: %.*s %.*s",
          msg_data->topicName->lenstring.len,
          msg_data->topicName->lenstring.data,
          msg_data->message->payloadlen,
          (char *)msg_data->message->payload);
}

static int mqtt_subscribe(int argc, char **argv)
{
    if (argc != 2)
    {
        rt_kprintf("mqtt_subscribe [topic]  --send an mqtt subscribe packet and wait for suback before returning.\n");
        return -1;
    }

    if (is_started == 0)
    {
        LOG_E("mqtt client is not connected.");
        return -1;
    }

    return paho_mqtt_subscribe(&client, QOS1, argv[1], mqtt_new_sub_callback);
}

static int mqtt_unsubscribe(int argc, char **argv)
{
    if (argc != 2)
    {
        rt_kprintf("mqtt_unsubscribe [topic]  --send an mqtt unsubscribe packet and wait for suback before returning.\n");
        return -1;
    }

    if (is_started == 0)
    {
        LOG_E("mqtt client is not connected.");
        return -1;
    }

    return paho_mqtt_unsubscribe(&client, argv[1]);
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(mqtt_start, startup mqtt client);
MSH_CMD_EXPORT(mqtt_stop, stop mqtt client);
MSH_CMD_EXPORT(mqtt_publish, mqtt publish message to specified topic);
MSH_CMD_EXPORT(mqtt_subscribe, mqtt subscribe topic);
MSH_CMD_EXPORT(mqtt_unsubscribe, mqtt unsubscribe topic);
#endif /* FINSH_USING_MSH */

// 周期性上传线程函数，每5秒采集一次数据并通过MQTT上传
static void mqtt_upload_entry(void *parameter)
{
    rt_thread_mdelay(20000); // 等待20秒确保MQTT连接已建立
    while (mqtt_upload_thread_running)
    {
        // 只有在MQTT连接且mqtt_start后才允许上传，否则等待信号量
        if (!(is_started && client.isconnected))
        {
            // 等待信号量，超时1秒后继续判断（防止假死）
            rt_sem_take(&mqtt_upload_sem, RT_TICK_PER_SECOND);
            continue;
        }
        char payload[256];
        // 组装JSON格式数据包，便于云端解析
        rt_snprintf(payload, sizeof(payload),
                    "{\"temperature\":%.2f,\"humidity\":%.2f,\"co2\":%d,\"tvoc\":%d,\"light\":%.2f,\"pressure\":%d,"
                    "\"human_state\":%d,\"heart_rate\":%d,\"resp_rate\":%d,\"env_score\":%.2f,\"pred_temp\":%.2f,\"pred_humi\":%.2f,\"led_switch\":%d,\"rtmp_switch\":%d}",
                    T_H_Data[0], T_H_Data[1], eco2, tvoc, current_light_intensity, (rt_uint16_t)Air_Data[1],
                    human_presence_status, heart_rate_value, respiration_value, env_pre, temp_pre, hum_pre, led_status, rtmp_status);
        // 通过MQTT发布数据
        paho_mqtt_publish(&client, QOS1, MQTT_PUBTOPIC, payload);
        rt_thread_mdelay(60000); // 60秒上传一次
    }
}

static int mqtt_upload_start(int argc, char **argv)
{
    if (mqtt_upload_thread_running)
    {
        LOG_E("MQTT upload thread is already running.");
        return -1;
    }
    // 初始化信号量，初始值为0
    rt_sem_init(&mqtt_upload_sem, "mqtt_up", 0, RT_IPC_FLAG_FIFO);
    mqtt_upload_thread = rt_thread_create("mqtt_upload", mqtt_upload_entry, RT_NULL, 1520, 20, 10);
    if (mqtt_upload_thread == RT_NULL)
    {
        LOG_E("Failed to create MQTT upload thread.");
        return -1;
    }
    mqtt_start(1, NULL); // 启动MQTT客户端
    mqtt_upload_thread_running = 1;
    rt_thread_startup(mqtt_upload_thread);
    return 0;
}
INIT_APP_EXPORT(mqtt_upload_start);
