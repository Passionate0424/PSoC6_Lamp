/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     RiceChen     the first version
 * 原软件包初始化中断引脚模式配置错误
 */

#define DBG_TAG "gt911"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#include "gt911.h"
#include "lcd_rtthread.h" /* 添加对LCD头文件的引用，以获取Landscape变量 */
#include <rtdevice.h>

/* 添加的全局变量，用于存储当前触摸状态和坐标 */
static struct
{
    uint8_t EN;      // 触摸检测开关, 0_关闭, 1_打开
    uint16_t lcdX;   // 当前按下的LCD坐标值X
    uint16_t lcdY;   // 当前按下的LCD坐标值Y
    uint8_t pressed; // 是否有触摸按下，0_未按下，1_已按下
} xGT911_Info = {1, 0, 0, 0};

struct rt_i2c_client gt911_client;
// 添加全局触摸设备指针变量
static struct rt_touch_device *gt911_touch_device = RT_NULL;

/* hardware section */
static rt_uint8_t GT911_CFG_TBL[] =
    {
        0x41,
        0xf0,
        0x00,
        0x40,
        0x01,
        0x05,
        0x35,
        0x00,
        0x02,
        0x08,
        0x1e,
        0x08,
        0x50,
        0x3c,
        0x0f,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x50,
        0x00,
        0x00,
        0x18,
        0x1a,
        0x1e,
        0x14,
        0x87,
        0x27,
        0x0a,
        0x4b,
        0x4d,
        0xd3,
        0x07,
        0x00,
        0x00,
        0x00,
        0x02,
        0x32,
        0x1c,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x32,
        0x00,
        0x00,
        0x2a,
        0x32,
        0x64,
        0x94,
        0xd5,
        0x02,
        0x07,
        0x00,
        0x00,
        0x04,
        0xa5,
        0x35,
        0x00,
        0x91,
        0x3d,
        0x00,
        0x80,
        0x46,
        0x00,
        0x70,
        0x51,
        0x00,
        0x63,
        0x5d,
        0x00,
        0x63,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x14,
        0x12,
        0x10,
        0x0e,
        0x0c,
        0x0a,
        0x08,
        0x06,
        0x04,
        0x02,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0x00,
        0x02,
        0x04,
        0x06,
        0x08,
        0x0a,
        0x0c,
        0x24,
        0x22,
        0x21,
        0x20,
        0x1f,
        0x1e,
        0x1d,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0x00,
        0x44};

/**
 * 向GT911寄存器写入数据
 * @param dev I2C客户端设备
 * @param data 要写入的数据缓冲区
 * @param len 要写入的数据长度
 * @return 成功返回RT_EOK，失败返回-RT_ERROR
 */
rt_err_t gt911_write_reg(struct rt_i2c_client *dev, rt_uint8_t *data, rt_uint8_t len)
{
    struct rt_i2c_msg msgs;

    msgs.addr = dev->client_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = data;
    msgs.len = len;

    if (rt_i2c_transfer(dev->bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

/**
 * 从GT911寄存器读取数据
 * @param dev I2C客户端设备
 * @param reg 要读取的寄存器地址
 * @param data 存储读取数据的缓冲区
 * @param len 要读取的数据长度
 * @return 成功返回RT_EOK，失败返回-RT_ERROR
 */
rt_err_t gt911_read_regs(struct rt_i2c_client *dev, rt_uint8_t *reg, rt_uint8_t *data, rt_uint8_t len)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr = dev->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = reg;
    msgs[0].len = GT911_REGITER_LEN;

    msgs[1].addr = dev->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf = data;
    msgs[1].len = len;

    if (rt_i2c_transfer(dev->bus, msgs, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

/**
 * 获取GT911产品ID
 * @param dev I2C客户端设备
 * @param data 存储产品ID的缓冲区
 * @param len 缓冲区长度
 * @return 成功返回RT_EOK，失败返回-RT_ERROR
 */
static rt_err_t gt911_get_product_id(struct rt_i2c_client *dev, rt_uint8_t *data, rt_uint8_t len)
{
    rt_uint8_t reg[2];

    reg[0] = (rt_uint8_t)(GT911_PRODUCT_ID >> 8);
    reg[1] = (rt_uint8_t)(GT911_PRODUCT_ID & 0xff);

    if (gt911_read_regs(dev, reg, data, len) != RT_EOK)
    {
        LOG_E("read id failed");
        return -RT_ERROR;
    }
    return RT_EOK;
}

/**
 * 获取GT911触摸面板信息
 * @param dev I2C客户端设备
 * @param info 存储触摸信息的指针(x/y范围，点数)
 * @return 成功返回RT_EOK，失败返回-RT_ERROR
 * 缺少类型和厂商
 */
static rt_err_t gt911_get_info(struct rt_i2c_client *dev, struct rt_touch_info *info)
{
    rt_uint8_t reg[2];
    rt_uint8_t out_info[7];
    rt_uint8_t out_len = 7;

    reg[0] = (rt_uint8_t)(GT911_CONFIG_REG >> 8);
    reg[1] = (rt_uint8_t)(GT911_CONFIG_REG & 0xFF);
    if (gt911_read_regs(dev, reg, out_info, out_len) != RT_EOK)
    {
        LOG_E("read info failed");
        return -RT_ERROR;
    }

    info->range_x = (out_info[2] << 8) | out_info[1];
    info->range_y = (out_info[4] << 8) | out_info[3];
    info->point_num = out_info[5] & 0x0f;

    return RT_EOK;
}

/**
 * 对GT911进行软件复位
 * @param dev I2C客户端设备
 * @return 成功返回RT_EOK，失败返回-RT_ERROR
 */
static rt_err_t gt911_soft_reset(struct rt_i2c_client *dev)
{
    rt_uint8_t buf[3];

    buf[0] = (rt_uint8_t)(GT911_COMMAND_REG >> 8);
    buf[1] = (rt_uint8_t)(GT911_COMMAND_REG & 0xFF);
    buf[2] = 0x02;

    if (gt911_write_reg(dev, buf, 3) != RT_EOK)
    {
        LOG_E("soft reset failed");
        return -RT_ERROR;
    }
    return RT_EOK;
}

static int16_t pre_x[GT911_MAX_TOUCH] = {-1, -1, -1, -1, -1};
static int16_t pre_y[GT911_MAX_TOUCH] = {-1, -1, -1, -1, -1};
static int16_t pre_w[GT911_MAX_TOUCH] = {-1, -1, -1, -1, -1};
static rt_uint8_t s_tp_dowm[GT911_MAX_TOUCH];
static struct rt_touch_data *read_data;

/**
 * 处理触摸抬起事件
 * @param buf 触摸数据缓冲区
 * @param id 触摸点ID
 */
static void gt911_touch_up(void *buf, int8_t id)
{
    read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1)
    {
        s_tp_dowm[id] = 0;
        read_data[id].event = RT_TOUCH_EVENT_UP;
    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_NONE;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = pre_w[id];
    read_data[id].x_coordinate = pre_x[id];
    read_data[id].y_coordinate = pre_y[id];
    read_data[id].track_id = id;

    pre_x[id] = -1; /* last point is none */
    pre_y[id] = -1;
    pre_w[id] = -1;
}

/**
 * 处理触摸按下/移动事件
 * @param buf 触摸数据缓冲区
 * @param id 触摸点ID
 * @param x X坐标
 * @param y Y坐标
 * @param w 触摸大小/宽度
 */
static void gt911_touch_down(void *buf, int8_t id, int16_t x, int16_t y, int16_t w)
{
    read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1)
    {
        read_data[id].event = RT_TOUCH_EVENT_MOVE;
    }
    else
    {
        read_data[id].event = RT_TOUCH_EVENT_DOWN;
        s_tp_dowm[id] = 1;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;

    pre_x[id] = x; /* save last point */
    pre_y[id] = y;
    pre_w[id] = w;
}

/**
 * 从GT911读取触摸点
 * @param touch 触摸设备结构体
 * @param buf 存储触摸点的缓冲区
 * @param read_num 要读取的点数
 * @return 实际读取的点数
 */
static rt_size_t gt911_read_point(struct rt_touch_device *touch, void *buf, rt_size_t read_num)
{
    rt_uint8_t point_status = 0;
    rt_uint8_t touch_num = 0;
    rt_uint8_t write_buf[3];
    rt_uint8_t cmd[2];
    rt_uint8_t read_buf[8 * GT911_MAX_TOUCH] = {0};
    rt_uint8_t read_index;
    int8_t read_id = 0;
    int16_t input_x = 0;
    int16_t input_y = 0;
    int16_t input_w = 0;
    int16_t temp = 0; // 用于交换X和Y坐标

    static rt_uint8_t pre_touch = 0;
    static int8_t pre_id[GT911_MAX_TOUCH] = {0};

    /* point status register */
    cmd[0] = (rt_uint8_t)((GT911_READ_STATUS >> 8) & 0xFF);
    cmd[1] = (rt_uint8_t)(GT911_READ_STATUS & 0xFF);

    if (gt911_read_regs(&gt911_client, cmd, &point_status, 1) != RT_EOK)
    {
        LOG_D("read point failed\n");
        read_num = 0;
        goto exit_;
    }

    if (point_status == 0) /* no data */
    {
        read_num = 0;
        goto exit_;
    }

    if ((point_status & 0x80) == 0) /* data is not ready */
    {
        read_num = 0;
        goto exit_;
    }

    touch_num = point_status & 0x0f; /* get point num */

    if (touch_num > GT911_MAX_TOUCH) /* point num is not correct */
    {
        read_num = 0;
        goto exit_;
    }

    cmd[0] = (rt_uint8_t)((GT911_POINT1_REG >> 8) & 0xFF);
    cmd[1] = (rt_uint8_t)(GT911_POINT1_REG & 0xFF);

    /* read point num is touch_num */
    if (gt911_read_regs(&gt911_client, cmd, read_buf, read_num * GT911_POINT_INFO_NUM) != RT_EOK)
    {
        LOG_D("read point failed\n");
        read_num = 0;
        goto exit_;
    }

    if (pre_touch > touch_num) /* point up */
    {
        for (read_index = 0; read_index < pre_touch; read_index++)
        {
            rt_uint8_t j;

            for (j = 0; j < touch_num; j++) /* this time touch num */
            {
                read_id = read_buf[j * 8] & 0x0F;

                if (pre_id[read_index] == read_id) /* this id is not free */
                    break;

                if (j >= touch_num - 1)
                {
                    rt_uint8_t up_id;
                    up_id = pre_id[read_index];
                    gt911_touch_up(buf, up_id);
                }
            }
        }
    }

    if (touch_num) /* point down */
    {
        rt_uint8_t off_set;

        for (read_index = 0; read_index < touch_num; read_index++)
        {
            off_set = read_index * 8;
            read_id = read_buf[off_set] & 0x0f;
            pre_id[read_index] = read_id;
            input_x = read_buf[off_set + 1] | (read_buf[off_set + 2] << 8); /* x */
            input_y = read_buf[off_set + 3] | (read_buf[off_set + 4] << 8); /* y */
            input_w = read_buf[off_set + 5] | (read_buf[off_set + 6] << 8); /* size */

            // 横屏模式下交换X和Y坐标
            if (Landscape)
            {
                // 在横屏模式下，交换X和Y坐标
                temp = input_x;
                input_x = LCD_H - input_y;
                input_y = temp;
            }

            gt911_touch_down(buf, read_id, input_x, input_y, input_w);
        }
    }
    else if (pre_touch)
    {
        for (read_index = 0; read_index < pre_touch; read_index++)
        {
            gt911_touch_up(buf, pre_id[read_index]);
        }
    }

    pre_touch = touch_num;

exit_:
    write_buf[0] = (rt_uint8_t)((GT911_READ_STATUS >> 8) & 0xFF);
    write_buf[1] = (rt_uint8_t)(GT911_READ_STATUS & 0xFF);
    write_buf[2] = 0x00;
    gt911_write_reg(&gt911_client, write_buf, 3);

    return read_num;
}

/**
 * 控制GT911触摸设备
 * @param touch 触摸设备结构体
 * @param cmd 控制命令(RT_TOUCH_CTRL_*)
 * @param arg 命令参数
 * @return 成功返回RT_EOK，失败返回-RT_ERROR
 */
static rt_err_t gt911_control(struct rt_touch_device *touch, int cmd, void *arg)
{
    if (cmd == RT_TOUCH_CTRL_GET_ID)
    {
        return gt911_get_product_id(&gt911_client, arg, 6);
    }

    if (cmd == RT_TOUCH_CTRL_GET_INFO)
    {
        return gt911_get_info(&gt911_client, arg);
    }

    rt_uint8_t buf[4];
    rt_uint8_t i = 0;
    // rt_uint8_t config[190];
    rt_uint8_t *config;

    config = (rt_uint8_t *)rt_calloc(1, sizeof(GT911_CFG_TBL) + GT911_REGITER_LEN);
    if (config == RT_NULL)
    {
        LOG_D("malloc config memory failed\n");
        return -RT_ERROR;
    }

    config[0] = (rt_uint8_t)((GT911_CONFIG_REG >> 8) & 0xFF);
    config[1] = (rt_uint8_t)(GT911_CONFIG_REG & 0xFF);

    memcpy(&config[2], GT911_CFG_TBL, sizeof(GT911_CFG_TBL));

    switch (cmd)
    {
    case RT_TOUCH_CTRL_SET_X_RANGE:
    {
        rt_uint16_t x_range;

        x_range = *(rt_uint16_t *)arg;
        config[4] = (rt_uint8_t)(x_range >> 8);
        config[3] = (rt_uint8_t)(x_range & 0xff);

        GT911_CFG_TBL[2] = config[4];
        GT911_CFG_TBL[1] = config[3];
        break;
    }
    case RT_TOUCH_CTRL_SET_Y_RANGE:
    {
        rt_uint16_t y_range;

        y_range = *(rt_uint16_t *)arg;
        config[6] = (rt_uint8_t)(y_range >> 8);
        config[5] = (rt_uint8_t)(y_range & 0xff);

        GT911_CFG_TBL[4] = config[6];
        GT911_CFG_TBL[3] = config[5];
        break;
    }
    case RT_TOUCH_CTRL_SET_X_TO_Y:
    {
        config[8] = config[8] ^= (1 << 3);
        break;
    }
    case RT_TOUCH_CTRL_SET_MODE:
    {
        rt_uint16_t trig_type;
        // 原有的*(rt_uint16_t*)值不正确
        trig_type = (rt_uint16_t)arg;

        switch (trig_type)
        {
        case RT_DEVICE_FLAG_INT_RX:
            config[8] &= 0xFC;
            config[9] = (rt_uint8_t)((GT911_READ_STATUS >> 8) & 0xFF);
            config[10] = (rt_uint8_t)(GT911_READ_STATUS & 0xFF);
            config[11] = 0x00;
            break;
        case RT_DEVICE_FLAG_RDONLY:
            config[8] &= 0xFC;
            config[8] |= 0x02;
            config[9] = (rt_uint8_t)((GT911_READ_STATUS >> 8) & 0xFF);
            config[10] = (rt_uint8_t)(GT911_READ_STATUS & 0xFF);
            config[11] = 0x00;
            break;
        default:
            break;
        }
        break;
    }
    default:
    {
        break;
    }
    }

    if (gt911_write_reg(&gt911_client, config, sizeof(GT911_CFG_TBL) + GT911_ADDR_LEN) != RT_EOK)
    {
        LOG_D("send config failed");
        return -1;
    }

    rt_uint8_t refresh_cmd[3] = {
        (rt_uint8_t)(GT911_COMMAND_REG >> 8),
        (rt_uint8_t)(GT911_COMMAND_REG & 0xFF),
        0x00 // Configuration refresh command
    };

    LOG_I("Refreshing GT911 configuration...");
    if (gt911_write_reg(&gt911_client, refresh_cmd, 3) != RT_EOK)
    {
        LOG_E("Failed to refresh configuration");
        return -RT_ERROR;
    }
    rt_thread_mdelay(50); // Required delay for configuration to take effect
    LOG_I("Configuration refresh complete");

    rt_uint8_t read_back[sizeof(GT911_CFG_TBL)] = {0};
    rt_uint8_t reg[2] = {(rt_uint8_t)(GT911_CONFIG_REG >> 8), (rt_uint8_t)(GT911_CONFIG_REG & 0xFF)};
    if (gt911_read_regs(&gt911_client, reg, read_back, sizeof(GT911_CFG_TBL)) == RT_EOK)
    {
        // 打印寄存器
        // for (rt_uint8_t i = 0; i < sizeof(GT911_CFG_TBL); i++)
        // {
        //     rt_kprintf("0x%02x,", read_back[i]);
        // }
        // rt_kprintf("\n");
        //        if (memcmp(read_back, &config[2], sizeof(config) - 2) == 0)
        //        {
        //            LOG_I("Config verify OK");
        //        }
        //        else
        //        {
        //            LOG_E("Config verify FAILED");
        //            rt_free(config);
        //            return -RT_ERROR;
        //        }
    }

    buf[0] = (rt_uint8_t)((GT911_CHECK_SUM >> 8) & 0xFF);
    buf[1] = (rt_uint8_t)(GT911_CHECK_SUM & 0xFF);
    buf[2] = 0;

    for (i = GT911_ADDR_LEN; i < sizeof(GT911_CFG_TBL) + GT911_ADDR_LEN; i++)
    {
        buf[GT911_ADDR_LEN] += config[i];
    }

    buf[2] = (~buf[2]) + 1;
    buf[3] = 1;

    gt911_write_reg(&gt911_client, buf, 4);
    rt_free(config);

    return RT_EOK;
}

static struct rt_touch_ops gt911_touch_ops =
    {
        .touch_readpoint = gt911_read_point,
        .touch_control = gt911_control,
};

/**
 * 函数名： GT911_Cmd
 * 功  能： 触摸检测开关
 *          由于检测比较耗时, 在不使用触屏的状态下, 可以关闭检测以节省芯片资源
 * 参  数： status  0_关闭触摸屏的检测,以节省资源; 1_打开触摸检测
 * 返  回： 无
 */
void GT911_Cmd(uint8_t status)
{
    if (status != 0)
    {
        xGT911_Info.EN = 1;
    }
    else
    {
        xGT911_Info.EN = 0;
    }
}

/**
 * 函数名： GT911_IsPressed
 * 功  能： 判断触摸屏是否有按下
 * 参  数： 无
 * 返  回： 0-未按下、1-按下中
 */
/**
 * 函数名： GT911_IsPressed
 * 功  能： 判断触摸屏是否有按下
 * 参  数： 无
 * 返  回： 0-未按下、1-按下中
 */
uint8_t GT911_IsPressed(void)
{
    if (xGT911_Info.EN == 0) // 触摸检测开关关闭时直接返回0
        return 0;

    // 查找GT911触摸设备
    static rt_device_t touch_dev = RT_NULL;
    if (touch_dev == RT_NULL)
    {
        touch_dev = rt_device_find("gt911");
        if (touch_dev == RT_NULL)
        {
            LOG_E("Cannot find gt911 device!");
            return 0;
        }
    }

    // 准备接收触摸数据的缓冲区
    struct rt_touch_data touch_data[GT911_MAX_TOUCH];
    rt_memset(touch_data, 0, sizeof(touch_data));

    // 读取触摸数据
    rt_size_t read_size = rt_device_read(touch_dev, 0, touch_data, GT911_MAX_TOUCH);

    // 如果有读取到触摸点数据
    if (read_size > 0)
    {
        // 检查是否有按下或移动事件
        for (rt_uint8_t i = 0; i < read_size; i++)
        {
            if (touch_data[i].event == RT_TOUCH_EVENT_DOWN ||
                touch_data[i].event == RT_TOUCH_EVENT_MOVE)
            {
                // 保存第一个有效触摸点的坐标
                xGT911_Info.lcdX = touch_data[i].x_coordinate;
                xGT911_Info.lcdY = touch_data[i].y_coordinate;
                xGT911_Info.pressed = 1;
                return 1;
            }
        }
    }

    xGT911_Info.pressed = 0;
    return 0;
}

/**
 * 函数名： GT911_GetX
 * 功  能： 获取按下位置的坐标值 (X)
 * 参  数： 无
 * 返  回： 坐标值 (X)
 */
uint16_t GT911_GetX(void)
{
    return xGT911_Info.lcdX;
}

/**
 * 函数名： GT911_GetY
 * 功  能： 获取按下位置的坐标值 (Y)
 * 参  数： 无
 * 返  回： 坐标值 (Y)
 */
uint16_t GT911_GetY(void)
{
    return xGT911_Info.lcdY;
}

/**
 * 初始化GT911触摸设备
 * @param name 设备名称
 * @param cfg 触摸配置
 * @return 成功返回RT_EOK，失败返回-RT_ERROR
 */
int rt_hw_gt911_init(const char *name, struct rt_touch_config *cfg)
{
    struct rt_touch_device *touch_device = RT_NULL;

    touch_device = (struct rt_touch_device *)rt_malloc(sizeof(struct rt_touch_device));
    if (touch_device == RT_NULL)
    {
        LOG_E("touch device malloc fail");
        return -RT_ERROR;
    }
    rt_memset((void *)touch_device, 0, sizeof(struct rt_touch_device));

    /* hw init*/
    cyhal_gpio_free(cfg->irq_pin.pin);
    rt_pin_mode(*(rt_uint8_t *)cfg->user_data, PIN_MODE_OUTPUT);
    rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_write(cfg->irq_pin.pin, PIN_LOW);
    rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_LOW);
    rt_thread_mdelay(10);

#ifdef PKG_GT911_USING_LOW_ADDR
    rt_pin_write(cfg->irq_pin.pin, PIN_HIGH);
    rt_thread_mdelay(10);
    rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_HIGH);
    rt_thread_mdelay(10);
    rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_INPUT);
    gt911_client.client_addr = GT911_ADDRESS_LOW;
#else
    rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_HIGH);
    rt_thread_mdelay(10);
    cyhal_gpio_free(cfg->irq_pin.pin);
    // rt_pin_write(cfg->irq_pin.pin, PIN_HIGH);
    rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_INPUT);
    rt_thread_mdelay(100);
    gt911_client.client_addr = GT911_ADDRESS_HIGH;
#endif

    gt911_client.bus = (struct rt_i2c_bus_device *)rt_device_find(cfg->dev_name);

    if (gt911_client.bus == RT_NULL)
    {
        LOG_E("Can't find %s device", cfg->dev_name);
        return -RT_ERROR;
    }

    if (rt_device_open((rt_device_t)gt911_client.bus, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        LOG_E("open %s device failed", cfg->dev_name);
        return -RT_ERROR;
    }

    gt911_soft_reset(&gt911_client);

    /* write config table */

    rt_uint8_t *config;
    config = (rt_uint8_t *)rt_calloc(1, sizeof(GT911_CFG_TBL) + 2);
    if (config == RT_NULL)
    {
        LOG_E("malloc config memory failed");
        return -RT_ERROR;
    }

    config[0] = (rt_uint8_t)((GT911_CONFIG_REG >> 8) & 0xFF);
    config[1] = (rt_uint8_t)(GT911_CONFIG_REG & 0xFF);
    memcpy(&config[2], GT911_CFG_TBL, sizeof(GT911_CFG_TBL));

    LOG_I("Writing config table to GT911...");
    rt_err_t ret = gt911_write_reg(&gt911_client, config, sizeof(GT911_CFG_TBL) + 2);
    if (ret != RT_EOK)
    {
        LOG_E("write config failed, ret=%d", ret);
        rt_free(config);
        return ret;
    }

    /* Verify config write */
    rt_uint8_t read_back[sizeof(GT911_CFG_TBL)] = {0};
    rt_uint8_t reg[2] = {(rt_uint8_t)(GT911_CONFIG_REG >> 8), (rt_uint8_t)(GT911_CONFIG_REG & 0xFF)};

    if (gt911_read_regs(&gt911_client, reg, read_back, sizeof(GT911_CFG_TBL)) == RT_EOK)
    {
        // 打印寄存器
        // for (rt_uint8_t i = 0; i < sizeof(GT911_CFG_TBL); i++)
        // {
        //     rt_kprintf("0x%02x,", read_back[i]);
        // }
        // rt_kprintf("\n");
        if (memcmp(read_back, GT911_CFG_TBL, sizeof(GT911_CFG_TBL)) == 0)
        {
            LOG_I("Config verify OK");
        }
        else
        {
            LOG_E("Config verify FAILED");
            rt_free(config);
            return -RT_ERROR;
        }
    }

    rt_free(config);

    /* Refresh configuration */

    rt_uint8_t refresh_cmd[3] = {
        (rt_uint8_t)(GT911_COMMAND_REG >> 8),
        (rt_uint8_t)(GT911_COMMAND_REG & 0xFF),
        0x00 // Configuration refresh command
    };

    LOG_I("Refreshing GT911 configuration...");
    if (gt911_write_reg(&gt911_client, refresh_cmd, 3) != RT_EOK)
    {
        LOG_E("Failed to refresh configuration");
        return -RT_ERROR;
    }
    rt_thread_mdelay(50); // Required delay for configuration to take effect
    LOG_I("Configuration refresh complete");

    /* register touch device */
    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = RT_TOUCH_VENDOR_GT;
    rt_memcpy(&touch_device->config, cfg, sizeof(struct rt_touch_config));
    touch_device->ops = &gt911_touch_ops;

    rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_INT_RX, RT_NULL);
    // rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_RDWR, RT_NULL);

    gt911_touch_device = touch_device;
    LOG_I("touch device gt911 init success");

    return RT_EOK;
}

// 初始化//
/**
 * GT911端口初始化(由RT-Thread自动调用)
 * @return 总是返回0
 */
int rt_hw_gt911_port(void)
{
    xGT911_Info.EN = 1; // 触摸检测开关打开
    struct rt_touch_config cfg;
    rt_uint8_t rst_pin;

    rst_pin = GT911_RST_PIN;
    cfg.dev_name = "i2c4";
    cfg.irq_pin.pin = GT911_IRQ_PIN;
    cfg.irq_pin.mode = PIN_MODE_INPUT;
    cfg.user_data = &rst_pin;

    rt_hw_gt911_init("gt911", &cfg);

    return 0;
}
INIT_ENV_EXPORT(rt_hw_gt911_port);
