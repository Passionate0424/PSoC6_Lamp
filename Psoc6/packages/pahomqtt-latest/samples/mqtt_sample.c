//#include <stdlib.h>
//#include <string.h>
//#include <stdint.h>
//
//#include <rtthread.h>
//
//#define DBG_ENABLE
//#define DBG_SECTION_NAME    "mqtt.sample"
//#define DBG_LEVEL           DBG_LOG
//#define DBG_COLOR
//#include <rtdbg.h>
//
//#include "paho_mqtt.h"
//
///**
// * MQTT URI farmat:
// * domain mode
// * tcp://iot.eclipse.org:1883
// *
// * ipv4 mode
// * tcp://192.168.10.1:1883
// * ssl://192.168.10.1:1884
// *
// * ipv6 mode
// * tcp://[fe80::20c:29ff:fe9a:a07e]:1883
// * ssl://[fe80::20c:29ff:fe9a:a07e]:1884
// */
//#define MQTT_URI                "tcp://47.104.235.67:1883"
//#define MQTT_USERNAME           "29dfcc5b-2f70-8318-7dd"                        //代理服务器用户名
//#define MQTT_PASSWORD           "14763e3"                        //代理服务器密码
//#define MQTT_SUBTOPIC           "devices/telemetry/control/12692f4f-20d6-d2d4-3655-bc29bbffe9"
//#define MQTT_PUBTOPIC           "devices/telemetry"
//#define MQTT_WILLMSG            "Goodbye!"
//
///* define MQTT client context */
//static MQTTClient client;
//static int is_started = 0;
//
//static void mqtt_sub_callback(MQTTClient *c, MessageData *msg_data)
//{
//    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
//    LOG_D("mqtt sub callback: %.*s %.*s",
//               msg_data->topicName->lenstring.len,
//               msg_data->topicName->lenstring.data,
//               msg_data->message->payloadlen,
//               (char *)msg_data->message->payload);
//}
//
//static void mqtt_sub_default_callback(MQTTClient *c, MessageData *msg_data)
//{
//    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
//    LOG_D("mqtt sub default callback: %.*s %.*s",
//               msg_data->topicName->lenstring.len,
//               msg_data->topicName->lenstring.data,
//               msg_data->message->payloadlen,
//               (char *)msg_data->message->payload);
//}
//
//static void mqtt_connect_callback(MQTTClient *c)
//{
//    LOG_D("inter mqtt_connect_callback!");
//}
//
//static void mqtt_online_callback(MQTTClient *c)
//{
//    LOG_D("inter mqtt_online_callback!");
//}
//
//static void mqtt_offline_callback(MQTTClient *c)
//{
//    LOG_D("inter mqtt_offline_callback!");
//}
//
//static int mqtt_start(int argc, char **argv)
//{
//    /* init condata param by using MQTTPacket_connectData_initializer */
//    MQTTPacket_connectData condata = MQTTPacket_connectData_initializer;
//    static char cid[20] = { 0 };
//
//    if (argc != 1)
//    {
//        rt_kprintf("mqtt_start    --start a mqtt worker thread.\n");
//        return -1;
//    }
//
//    if (is_started)
//    {
//        LOG_E("mqtt client is already connected.");
//        return -1;
//    }
//    /* config MQTT context param */
//    {
//        client.isconnected = 0;
//        client.uri = &MQTT_URI;
//
//        /* generate the random client ID */
//        rt_snprintf(cid, sizeof(cid), "rtthread%d", rt_tick_get());
//        /* config connect param */
//        rt_memcpy(&client.condata, &condata, sizeof(condata));
//        client.condata.clientID.cstring = cid;
//        client.condata.keepAliveInterval = 30;
//        client.condata.cleansession = 1;
//        client.condata.username.cstring=MQTT_USERNAME;
//        client.condata.password.cstring=MQTT_PASSWORD;
//
//        /* config MQTT will param. */
//        client.condata.willFlag = 1;
//        client.condata.will.qos = 1;
//        client.condata.will.retained = 0;
//        client.condata.will.topicName.cstring = MQTT_PUBTOPIC;
//        client.condata.will.message.cstring = MQTT_WILLMSG;
//
//        /* rt_malloc buffer. */
//        client.buf_size = client.readbuf_size = 1024;
//        client.buf = rt_calloc(1, client.buf_size);
//        client.readbuf = rt_calloc(1, client.readbuf_size);
//        if (!(client.buf && client.readbuf))
//        {
//            LOG_E("no memory for MQTT client buffer!");
//            return -1;
//        }
//
//        /* set event callback function */
//        client.connect_callback = mqtt_connect_callback;
//        client.online_callback = mqtt_online_callback;
//        client.offline_callback = mqtt_offline_callback;
//
//        /* set subscribe table and event callback */
//        client.messageHandlers[0].topicFilter = rt_strdup(MQTT_SUBTOPIC);
//        client.messageHandlers[0].callback = mqtt_sub_callback;
//        client.messageHandlers[0].qos = QOS1;
//
//        /* set default subscribe event callback */
//        client.defaultMessageHandler = mqtt_sub_default_callback;
//    }
//
//    /* run mqtt client */
//    paho_mqtt_start(&client);
//    is_started = 1;
//
//    return 0;
//}
//
//static int mqtt_stop(int argc, char **argv)
//{
//    if (argc != 1)
//    {
//        rt_kprintf("mqtt_stop    --stop mqtt worker thread and free mqtt client object.\n");
//    }
//
//    is_started = 0;
//
//    return paho_mqtt_stop(&client);
//}
//
//static int mqtt_publish(int argc, char **argv)
//{
//    if (is_started == 0)
//    {
//        LOG_E("mqtt client is not connected.");
//        return -1;
//    }
//
//    if (argc == 2)
//    {
//        paho_mqtt_publish(&client, QOS1, MQTT_PUBTOPIC, argv[1]);
//    }
//    else if (argc == 3)
//    {
//        paho_mqtt_publish(&client, QOS1, argv[1], argv[2]);
//    }
//    else
//    {
//        rt_kprintf("mqtt_publish <topic> [message]  --mqtt publish message to specified topic.\n");
//        return -1;
//    }
//
//    return 0;
//}
//
//static void mqtt_new_sub_callback(MQTTClient *client, MessageData *msg_data)
//{
//    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
//    LOG_D("mqtt new subscribe callback: %.*s %.*s",
//               msg_data->topicName->lenstring.len,
//               msg_data->topicName->lenstring.data,
//               msg_data->message->payloadlen,
//               (char *)msg_data->message->payload);
//}
//
//static int mqtt_subscribe(int argc, char **argv)
//{
//    if (argc != 2)
//    {
//        rt_kprintf("mqtt_subscribe [topic]  --send an mqtt subscribe packet and wait for suback before returning.\n");
//        return -1;
//    }
//
//    if (is_started == 0)
//    {
//        LOG_E("mqtt client is not connected.");
//        return -1;
//    }
//
//    return paho_mqtt_subscribe(&client, QOS1, argv[1], mqtt_new_sub_callback);
//}
//
//static int mqtt_unsubscribe(int argc, char **argv)
//{
//    if (argc != 2)
//    {
//        rt_kprintf("mqtt_unsubscribe [topic]  --send an mqtt unsubscribe packet and wait for suback before returning.\n");
//        return -1;
//    }
//
//    if (is_started == 0)
//    {
//        LOG_E("mqtt client is not connected.");
//        return -1;
//    }
//
//    return paho_mqtt_unsubscribe(&client, argv[1]);
//}
//
//#ifdef FINSH_USING_MSH
//MSH_CMD_EXPORT(mqtt_start, startup mqtt client);
//MSH_CMD_EXPORT(mqtt_stop, stop mqtt client);
//MSH_CMD_EXPORT(mqtt_publish, mqtt publish message to specified topic);
//MSH_CMD_EXPORT(mqtt_subscribe,  mqtt subscribe topic);
//MSH_CMD_EXPORT(mqtt_unsubscribe, mqtt unsubscribe topic);
//#endif /* FINSH_USING_MSH */
//
