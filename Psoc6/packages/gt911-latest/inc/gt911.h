/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     RiceChen     the first version
 */

#ifndef __GT911_H__
#define __GT911_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <drv_gpio.h>

extern struct rt_i2c_client gt911_client;

//定义引脚
#define GT911_RST_PIN GET_PIN(11,4)
#define GT911_IRQ_PIN GET_PIN(11,3)

#define GT911_ADDR_LEN          2
#define GT911_REGITER_LEN       2
#define GT911_MAX_TOUCH         5
#define GT911_POINT_INFO_NUM    5

#define GT911_ADDRESS_HIGH      0x5D
#define GT911_ADDRESS_LOW       0x14

#define GT911_COMMAND_REG       0x8040
#define GT911_CONFIG_REG        0x8047

#define GT911_PRODUCT_ID        0x8140
#define GT911_VENDOR_ID         0x814A
#define GT911_READ_STATUS       0x814E

#define GT911_POINT1_REG        0x814F
#define GT911_POINT2_REG        0x8157
#define GT911_POINT3_REG        0x815F
#define GT911_POINT4_REG        0x8167
#define GT911_POINT5_REG        0x816F

#define GT911_CHECK_SUM         0x80FF

/* Function declarations */
static rt_size_t gt911_read_point(struct rt_touch_device *touch, void *buf, rt_size_t read_num);
rt_err_t gt911_write_reg(struct rt_i2c_client *dev, rt_uint8_t *data, rt_uint8_t len);
rt_err_t gt911_read_regs(struct rt_i2c_client *dev, rt_uint8_t *reg, rt_uint8_t *data, rt_uint8_t len);
static rt_err_t gt911_get_product_id(struct rt_i2c_client *dev, rt_uint8_t *data, rt_uint8_t len);
static rt_err_t gt911_get_info(struct rt_i2c_client *dev, struct rt_touch_info *info);
static rt_err_t gt911_soft_reset(struct rt_i2c_client *dev);
static rt_err_t gt911_control(struct rt_touch_device *touch, int cmd, void *arg);

int rt_hw_gt911_init(const char *name, struct rt_touch_config *cfg);

#endif /* gt911.h */
