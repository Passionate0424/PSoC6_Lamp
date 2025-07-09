//目前被copy文件取代

#if 0
/**
 * @file lcd_rtthread.c
 * @brief ST7789 LCD驱动实现 - RT-Thread适配版
 *
 * 主要功能：
 * - LCD初始化配置
 * - 基本绘图功能实现
 * - 文本显示功能
 * - 底层硬件接口封装
 *
 * 移植说明：
 * 1. 基于原厂lcd.c驱动移植
 * 2. 适配RT-Thread SPI设备框架
 * 3. 使用RT-Thread PIN框架控制GPIO
 */
#include "lcd_rtthread.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <drv_common.h>
#include <drv_spi.h> /* 添加SPI驱动头文件 */
#include "font.h"    /* 包含字体数据头文件 */
#include "gt911.h"   /* 包含GT911触摸驱动头文件 */

///* 函数前向声明 */
// static u32 LCD_Pow(u8 m, u8 n);

#define DBG_TAG "lcd.rt"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

u8 DFT_SCAN_DIR;
_lcd_dev lcddev;
static struct rt_spi_device *lcd_dev;

/* 使用lcd_rtthread.h中定义的引脚操作宏 */

/* 移植后的SPI接口函数 */
/**
 * @brief 写LCD寄存器命令
 * @param reg 寄存器地址/命令
 *
 * 功能说明：
 * 1. 拉低DC线表示写命令
 * 2. 通过SPI发送命令字节
 * 3. 拉高DC线恢复数据模式
 */
static void LCD_WR_REG(uint8_t reg)
{
    LCD_DC_CLR;
    rt_spi_send(lcd_dev, &reg, 1);
    LCD_DC_SET;
}

/**
 * @brief 写LCD数据(8位)
 * @param data 要写入的数据
 *
 * 功能说明：
 * 1. 保持DC线为高表示写数据
 * 2. 通过SPI发送单字节数据
 */
static void LCD_WR_DATA(uint8_t data)
{
    LCD_DC_SET;
    rt_spi_send(lcd_dev, &data, 1);
}

/**
 * @brief 写LCD数据(16位)
 * @param data 要写入的16位数据
 *
 * 功能说明：
 * 1. 保持DC线为高表示写数据
 * 2. 将16位数据拆分为高低字节
 * 3. 通过SPI发送两个字节
 */
static void LCD_WR_DATA_16BIT(uint16_t data)
{
    uint8_t buf[2] = {data >> 8, data & 0xFF};
    LCD_DC_SET;
    rt_spi_send(lcd_dev, buf, 2);
}

/* 新增GRAM操作函数 */
/**
 * @brief 写GRAM数据
 * @param RGB_Code RGB565格式颜色值
 *
 * 功能说明：
 * 1. 调用16位数据写入函数
 * 2. 用于连续写入GRAM数据
 */
static void LCD_WriteRAM(uint16_t RGB_Code)
{
    LCD_WR_DATA_16BIT(RGB_Code);
}

/**
 * @brief 准备GRAM写入
 *
 * 功能说明：
 * 1. 发送GRAM写入命令
 * 2. 必须在连续写入GRAM数据前调用
 */
static void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}

/**
 * @brief 写LCD寄存器值
 * @param LCD_Reg 寄存器地址
 * @param LCD_RegValue 要写入的寄存器值
 *
 * 功能说明：
 * 1. 先写寄存器地址
 * 2. 再写寄存器值(16位)
 */
static void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

/* 保留lcd.c中的初始化命令序列 */
static void LCD_INIT_CODE(u8 dir)
{
    // LCD_BLK_CLR;
    LCD_WR_REG(0x11);
    rt_thread_mdelay(120); // Delay 120ms
    //------------------------------display and color format setting--------------------------------//
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x00);

    LCD_WR_REG(0x3a);
    LCD_WR_DATA(0x05);
    //--------------------------------ST7789V Frame rate setting----------------------------------//
    LCD_WR_REG(0xb2);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x0c);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x33);
    LCD_Display_Dir(dir);
    LCD_WR_REG(0xb7);
    LCD_WR_DATA(0x35);
    //---------------------------------ST7789V Power setting--------------------------------------//
    LCD_WR_REG(0xbb);
    LCD_WR_DATA(0x28);
    LCD_WR_REG(0xc0);
    LCD_WR_DATA(0x2c);
    LCD_WR_REG(0xc2);
    LCD_WR_DATA(0x01);
    LCD_WR_REG(0xc3);
    LCD_WR_DATA(0x10);
    LCD_WR_REG(0xc4);
    LCD_WR_DATA(0x20);
    LCD_WR_REG(0xc6);
    LCD_WR_DATA(0x0f);
    LCD_WR_REG(0xd0);
    LCD_WR_DATA(0xa4);
    LCD_WR_DATA(0xa1);
    //--------------------------------ST7789V gamma setting---------------------------------------//
    // 0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23
    LCD_WR_REG(0xe0);
    LCD_WR_DATA(0xd0);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x0a);
    LCD_WR_DATA(0x28);
    LCD_WR_DATA(0x32);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x42);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x0e);
    LCD_WR_DATA(0x12);
    LCD_WR_DATA(0x14);
    LCD_WR_DATA(0x17);
    // 0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23
    LCD_WR_REG(0xe1);
    LCD_WR_DATA(0xd0);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x0a);
    LCD_WR_DATA(0x28);
    LCD_WR_DATA(0x31);
    LCD_WR_DATA(0x54);
    LCD_WR_DATA(0x47);
    LCD_WR_DATA(0x0e);
    LCD_WR_DATA(0x1c);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x1b);
    LCD_WR_DATA(0x1e);
    LCD_WR_REG(0x29);
}

/* 移植lcd.c中的功能函数 */
/**
 * @brief 设置光标位置
 * @param Xpos X坐标
 * @param Ypos Y坐标
 *
 * 功能说明：
 * 1. 设置X方向光标位置
 * 2. 设置Y方向光标位置
 * 3. 用于后续数据写入定位
 */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(Xpos >> 8);
    LCD_WR_DATA(Xpos & 0X00FF);
    LCD_WR_DATA(Xpos >> 8);
    LCD_WR_DATA(Xpos & 0X00FF);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(Ypos >> 8);
    LCD_WR_DATA(Ypos & 0X000FF);
    LCD_WR_DATA(Ypos >> 8);
    LCD_WR_DATA(Ypos & 0X00FF);
}

/* 设置窗口函数 - 优化为原厂风格 */
/**
 * @brief 设置LCD显示窗口
 * @param sx 起始X坐标
 * @param sy 起始Y坐标
 * @param width 窗口宽度
 * @param height 窗口高度
 *
 * 功能说明：
 * 1. 设置X方向起始和结束地址
 * 2. 设置Y方向起始和结束地址
 * 3. 准备GRAM写入
 */
static void LCD_SetWindows(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    width = sx + width - 1;
    height = sy + height - 1;

    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(sx >> 8);
    LCD_WR_DATA(sx & 0X00FF);
    LCD_WR_DATA(width >> 8);
    LCD_WR_DATA(width & 0X00FF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(sy >> 8);
    LCD_WR_DATA(sy & 0X00FF);
    LCD_WR_DATA(height >> 8);
    LCD_WR_DATA(height & 0X00FF);
}

/* 兼容旧接口 */
/**
 * @brief 设置显示窗口(兼容旧接口)
 * @param sx 起始X坐标
 * @param sy 起始Y坐标
 * @param ex 结束X坐标
 * @param ey 结束Y坐标
 *
 * 功能说明：
 * 1. 转换为宽度/高度格式
 * 2. 调用新式窗口设置函数
 */
static void LCD_SetWindow(u16 sx, u16 sy, u16 ex, u16 ey)
{
    LCD_SetWindows(sx, sy, ex - sx + 1, ey - sy + 1);
}

/**
 * @brief 清屏函数
 * @param Color 填充颜色(RGB565)
 *
 * 功能说明：
 * 1. 设置全屏窗口
 * 2. 循环写入颜色数据
 * 3. 用于快速清屏
 */
void LCD_Clear(uint16_t Color)
{
    uint32_t index;
    uint32_t total = lcddev.width * lcddev.height;

    LCD_SetWindows(0, 0, lcddev.width, lcddev.height);
    LCD_WriteRAM_Prepare(); // 开始写入GRAM

    // for (index = 0; index < total; index++)
    // {
    //     LCD_WR_DATA_16BIT(Color);
    // }
    for (index = 0; index < total; index++)
    {
#if (FAST == 0)
        LCD_WriteRAM(color);
#else
        // 使用以下代码可以提速

        uint8_t buf[2] = {Color >> 8, Color & 0xFF};
        rt_spi_send(lcd_dev, buf, 2);
#endif
    }
}

/* 保留其他图形绘制函数... */

/* RT-Thread设备初始化 */
static void lcd_pin_init(void)
{
    rt_pin_mode(LCD_DC_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_RES_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_BLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_CS_PIN, PIN_MODE_OUTPUT);
}

int spi_lcd_init(void)
{
    rt_err_t res = RT_EOK;

    /* 初始化引脚 */
    lcd_pin_init();

    /* 附加SPI设备 */
    rt_hw_spi_device_attach(LCD_SPI_BUS, "spi_lcd", LCD_CS_PIN);

    /* 查找SPI设备 */
    lcd_dev = (struct rt_spi_device *)rt_device_find("spi_lcd");
    if (lcd_dev != RT_NULL)
    {
        /* 配置SPI参数 */
        struct rt_spi_configuration spi_config;
        spi_config.data_width = 8;
        spi_config.max_hz = 25 * 1000 * 1000; /* 25MHz */
        spi_config.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
        rt_spi_configure(lcd_dev, &spi_config);
    }
    else
    {
        res = -RT_ERROR;
        LOG_E("SPI device not found!");
    }

    // /* 初始化LCD设备参数 */
    // lcddev.width = LCD_W;
    // lcddev.height = LCD_H;
    // lcddev.wramcmd = 0x2C;
    // lcddev.setxcmd = 0x2A;
    // lcddev.setycmd = 0x2B;
    // lcddev.dir = 1; // 默认横屏(与头文件定义一致)

    /* 初始化LCD */
    /* RT-Thread适配的复位时序 */
    // LCD_RES_SET;
    rt_thread_mdelay(25);
    LCD_RES_CLR;
    rt_thread_mdelay(25);
    LCD_RES_SET;
    rt_thread_mdelay(50);

    /* 执行LCD初始化序列 */
    LCD_INIT_CODE(Landscape);

    /* 设置显示方向(使用头文件定义的默认方向) */
    LOG_D("Setting default display direction: %s", Landscape == LANDSCAPE ? "LANDSCAPE" : "PORTRAIT");
    // LCD_Display_Dir(Landscape);
    rt_thread_mdelay(10); // 确保方向设置完成

    // /* 背光控制 - 完全按照lcd.h定义 */
    // #if (BACKLIGHT_ACTIVE_LEVEL == 1)
    //     LCD_BLK_SET; // 高电平点亮
    // #else
    //     LCD_BLK_CLR; // 低电平点亮
    // #endif

    //     /* 详细初始化流程 */
    //     LOG_D("--- Starting LCD Initialization ---");

    //     // 1. 复位序列
    //     LOG_D("1. Reset sequence");
    //     LCD_RES_SET;
    //     rt_thread_mdelay(10);
    //     LCD_RES_CLR;
    //     rt_thread_mdelay(20);
    //     LCD_RES_SET;
    //     rt_thread_mdelay(120); // 延长复位后等待时间

    //     // 2. 发送初始化命令
    //     LOG_D("2. Sending init commands");
    //     LCD_INIT_CODE();

    //     // 3. 设置颜色接口(0x3A) - 完全按照原厂设置
    //     LOG_D("3. Set color format");
    //     LCD_WR_REG(0x3A);
    //     LCD_WR_DATA(0x05); // RGB565格式
    //     rt_thread_mdelay(10);

    //     // 4. 设置扫描方向(0x36)
    //     LOG_D("4. Set scan direction");
    //     LCD_WR_REG(0x36);
    //     LCD_WR_DATA(0x00); // 竖屏模式
    //     rt_thread_mdelay(10);

    //     // 5. 开启显示(0x29)
    //     LOG_D("5. Display ON");
    //     LCD_WR_REG(0x29);
    //     rt_thread_mdelay(100); // 确保显示完全开启

    //     LOG_D("--- LCD Initialization Complete ---");

    //     LOG_I("LCD init success");
    //     return RT_EOK;
}

INIT_COMPONENT_EXPORT(spi_lcd_init);

/* 移植图形绘制函数 */
/**
 * @brief 在指定位置画点
 * @param x X坐标
 * @param y Y坐标
 * @param color 点的颜色(RGB565)
 *
 * 功能说明：
 * 1. 设置光标位置
 * 2. 准备GRAM写入
 * 3. 写入颜色数据
 */
void LCD_DrawPoint(u16 x, u16 y, u16 color)
{
    LCD_SetCursor(x, y);
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(color);
}

void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
    u16 i, j;
    u16 width = ex - sx + 1;
    u16 height = ey - sy + 1;

    LCD_SetWindows(sx, sy, width, height);
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            LCD_WR_DATA_16BIT(color);
        }
    }
}

/* 字体显示函数 */
/**
 * @brief 显示单个字符
 * @param x X坐标
 * @param y Y坐标
 * @param chr 要显示的字符
 * @param size 字体大小(12/16)
 * @param mode 显示模式(0-覆盖模式,1-叠加模式)
 * @param color 字符颜色
 *
 * 功能说明：
 * 1. 计算字符在字库中的偏移
 * 2. 获取字模数据
 * 3. 按位绘制字符点阵
 */
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color, uint16_t bg_color)
{
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
    uint8_t *pfont = 0;

    // 字符偏移量计算
    chr = chr - ' ';
    LOG_D("Drawing char '%c'(%d) at (%d,%d) size:%d", chr + ' ', chr + ' ', x, y, size);

    // 获取字体数据指针
    switch (size)
    {
    case 12:
        pfont = (uint8_t *)asc2_1206[chr];
        break;
    case 16:
        pfont = (uint8_t *)asc2_1608[chr];
        break;
    default:
        LOG_E("Unsupported font size: %d", size);
        return;
    }

    for (t = 0; t < csize; t++)
    {
        temp = pfont[t]; /* 获取字符的点阵数据 */

        for (t1 = 0; t1 < 8; t1++) /* 一个字节8个点 */
        {
            if (temp & 0x80) /* 有效点,需要显示 */
            {
                LCD_DrawPoint(x, y, color); /* 画点出来,要显示这个点 */
            }
            else if (mode == 0) /* 无效点,不显示 */
            {
                LCD_DrawPoint(x, y, bg_color); /* 画指定背景色 */
            }

            temp <<= 1; /* 移位, 以便获取下一个位的状态 */
            y++;

            if (y >= lcddev.height)
                return; /* 超区域了 */

            if ((y - y0) == size) /* 显示完一列了? */
            {
                y = y0; /* y坐标复位 */
                x++;    /* x坐标递增 */

                if (x >= lcddev.width)
                    return; /* x坐标超区域了 */

                break;
            }
        }
    }
}

/**
 * @brief 在LCD上显示字符串
 * @param x 起始X坐标(像素)
 * @param y 起始Y坐标(像素)
 * @param width 显示区域宽度(像素)
 * @param height 显示区域高度(像素)
 * @param size 字体大小(12/16/24等)
 * @param p 要显示的字符串指针
 * @param color 文字颜色(RGB565格式)
 * @param bg_color 背景颜色(RGB565格式)
 *
 * 功能说明：
 * 1. 在指定区域内显示字符串
 * 2. 自动换行处理(当超出width时换到下一行)
 * 3. 支持ASCII字符(32~126)
 * 4. 超出显示区域的内容会被裁剪
 * 5. 可自定义文字和背景颜色
 *
 * 注意：
 * - 使用前需先初始化LCD
 * - 确保坐标和区域大小在屏幕范围内
 * - 背景色参数为新增功能，旧代码需要更新
 */
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p, u16 color, uint16_t bg_color)
{
    u8 x0 = x;
    width += x;
    height += y;
    while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }
        if (y >= height)
            break; // 退出
        LCD_ShowChar(x, y, *p, size, 0, color, bg_color);
        x += size / 2;
        p++;
    }
}

/**
 * @brief 显示数字
 * @param x X坐标
 * @param y Y坐标
 * @param num 要显示的数字
 * @param len 数字长度
 * @param size 字体大小
 * @param color 颜色
 *
 * 功能说明：
 * 1. 支持固定长度显示
 * 2. 自动处理前导零
 * 3. 调用字符显示函数实现
 */
/**
 * @brief 显示数字
 * @param x X坐标
 * @param y Y坐标
 * @param num 要显示的数字
 * @param len 数字长度
 * @param size 字体大小
 * @param color 文字颜色
 * @param bg_color 背景颜色
 *
 * 功能说明：
 * 1. 支持固定长度显示
 * 2. 自动处理前导零
 * 3. 可自定义文字和背景颜色
 */
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size, u16 color, uint16_t bg_color)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + (size / 2) * t, y, ' ', size, 0, color, bg_color);
                continue;
            }
            else
                enshow = 1;
        }
        LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, 0, color, bg_color);
    }
}

/**
 * @brief 显示数字(增强版)
 * @param x X坐标
 * @param y Y坐标
 * @param num 要显示的数字
 * @param len 数字长度
 * @param size 字体大小
 * @param mode 显示模式
 * @param color 颜色
 *
 * 功能说明：
 * 1. 支持多种显示模式
 * 2. 可控制前导零显示
 * 3. 调用字符显示函数实现
 */
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode, u16 color, uint16_t bg_color)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                if (mode & 0X80)
                    LCD_ShowChar(x + (size / 2) * t, y, '0', size, mode & 0X01, color, bg_color);
                else
                    LCD_ShowChar(x + (size / 2) * t, y, ' ', size, mode & 0X01, color, BACK_COLOR);
                continue;
            }
            else
                enshow = 1;
        }
        LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, mode & 0X01, color, BACK_COLOR);
    }
}

// m^n函数
// 返回值:m^n次方.
/**
 * @brief 计算m的n次方
 * @param m 底数
 * @param n 指数
 * @return m^n结果
 *
 * 功能说明：
 * 1. 用于数字显示计算
 * 2. 简单循环实现
 */
u32 LCD_Pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}

/* 图片显示函数 */
/**
 * @brief 显示图片
 * @param x X坐标
 * @param y Y坐标
 * @param width 图片宽度
 * @param height 图片高度
 * @param p 图片数据指针(RGB565格式)
 *
 * 功能说明：
 * 1. 设置显示窗口
 * 2. 连续写入像素数据
 * 3. 支持RGB565格式图片
 */
void LCD_ShowImage(u16 x, u16 y, u16 width, u16 height, const u16 *p)
{
    u32 i;
    u16 color;

    LCD_SetWindows(x, y, width, height);
    for (i = 0; i < width * height; i++)
    {
        color = *p++;
        LCD_WR_DATA_16BIT(color);
    }
}
// 画线
// x1,y1:起点坐标
// x2,y2:终点坐标
// color:颜色
/**
 * @brief 画线函数
 * @param x1 起点X坐标
 * @param y1 起点Y坐标
 * @param x2 终点X坐标
 * @param y2 终点Y坐标
 * @param color 线条颜色
 *
 * 功能说明：
 * 1. 使用Bresenham算法
 * 2. 支持任意方向直线
 * 3. 调用画点函数实现
 */
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t <= distance + 1; t++) // 画线输出
    {
        LCD_DrawPoint(uRow, uCol, color); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
// 画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
/**
 * @brief 画矩形
 * @param x1 左上角X坐标
 * @param y1 左上角Y坐标
 * @param x2 右下角X坐标
 * @param y2 右下角Y坐标
 * @param color 边框颜色
 *
 * 功能说明：
 * 1. 调用画线函数实现
 * 2. 绘制四条边组成矩形
 */
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}
// 在指定位置画一个指定大小的圆
//(x,y):中心点
// r    :半径
/**
 * @brief 画圆
 * @param x0 圆心X坐标
 * @param y0 圆心Y坐标
 * @param r 半径
 * @param color 圆颜色
 *
 * 功能说明：
 * 1. 使用Bresenham算法
 * 2. 对称绘制8个点
 * 3. 调用画点函数实现
 */
void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1); // 判断下个点位置的标志
    while (a <= b)
    {
        LCD_DrawPoint(x0 + a, y0 - b, color); // 5
        LCD_DrawPoint(x0 + b, y0 - a, color); // 0
        LCD_DrawPoint(x0 + b, y0 + a, color); // 4
        LCD_DrawPoint(x0 + a, y0 + b, color); // 6
        LCD_DrawPoint(x0 - a, y0 + b, color); // 1
        LCD_DrawPoint(x0 - b, y0 + a, color);
        LCD_DrawPoint(x0 - a, y0 - b, color); // 2
        LCD_DrawPoint(x0 - b, y0 - a, color); // 7
        a++;
        // 使用Bresenham算法画圆
        if (di < 0)
            di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}
// 设置LCD的自动扫描方向
// dir:0~7,代表8个方向(具体定义见lcd.h)
/**
 * @brief 设置LCD扫描方向
 * @param dir 方向参数(0~7)
 *
 * 功能说明：
 * 1. 配置扫描方向寄存器
 * 2. 影响显示内容方向
 * 3. 支持8种方向设置
 */
void LCD_Scan_Dir(u8 dir)
{
    u16 regval = 0;
    u8 dirreg = 0;
    //  u16 temp;
    /* 方向参数转换 - 与原版lcd.c保持一致 */
    switch (dir)
    {
    case 0:
        dir = 6;
        break;
    case 1:
        dir = 7;
        break;
    case 2:
        dir = 4;
        break;
    case 3:
        dir = 5;
        break;
    case 4:
        dir = 1;
        break;
    case 5:
        dir = 0;
        break;
    case 6:
        dir = 3;
        break;
    case 7:
        dir = 2;
        break;
    }
    rt_kprintf("dir:%d\n", dir);
    switch (dir)
    {
    case L2R_U2D: // 从左到右,从上到下
        regval = (0 << 7) | (0 << 6) | (0 << 5);
        break;
    case L2R_D2U: // 从左到右,从下到上
        regval = (1 << 7) | (0 << 6) | (0 << 5);
        break;
    case R2L_U2D: // 从右到左,从上到下
        regval = (0 << 7) | (1 << 6) | (0 << 5);
        break;
    case R2L_D2U: // 从右到左,从下到上
        regval = (1 << 7) | (1 << 6) | (0 << 5);
        break;
    case U2D_L2R: // 从上到下,从左到右
        regval = (0 << 7) | (0 << 6) | (1 << 5);
        break;
    case U2D_R2L: // 从上到下,从右到左
        regval = (0 << 7) | (1 << 6) | (1 << 5);
        break;
    case D2U_L2R: // 从下到上,从左到右
        regval = (1 << 7) | (0 << 6) | (1 << 5);
        break;
    case D2U_R2L: // 从下到上,从右到左
        regval = (1 << 7) | (1 << 6) | (1 << 5);
        break;
    }
    dirreg = 0X36;
    regval |= 0x00; // 0x08 0x00  红蓝反色可以通过这里修改
    rt_kprintf("regval:%x\n", regval);
    LCD_WriteReg(dirreg, regval);

    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.width - 1) >> 8);
    LCD_WR_DATA((lcddev.width - 1) & 0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.height - 1) >> 8);
    LCD_WR_DATA((lcddev.height - 1) & 0XFF);
}

// 设置LCD显示方向（6804不支持横屏显示）
// dir:0,竖屏；1,横屏
/**
 * @brief 设置LCD显示方向
 * @param dir 方向(0-竖屏,1-横屏)
 *
 * 功能说明：
 * 1. 配置显示参数
 * 2. 更新宽高值
 * 3. 设置默认扫描方向
 */
void LCD_Display_Dir(u8 dir)
{
    if (dir == 0)
    {
        lcddev.dir = 0; // 竖屏
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;

        lcddev.wramcmd = 0X2C;
        lcddev.setxcmd = 0X2A;
        lcddev.setycmd = 0X2B;
        DFT_SCAN_DIR = U2D_R2L; // 竖显-设定显示方向
    }
    else // 横屏
    {
        lcddev.dir = 1;
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;

        lcddev.wramcmd = 0X2C;
        lcddev.setxcmd = 0X2A;
        lcddev.setycmd = 0X2B;
        DFT_SCAN_DIR = L2R_U2D; // 横显-设定显示方向
    }
    LCD_Scan_Dir(DFT_SCAN_DIR); // 默认扫描方向
}

// 在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
// color:要填充的颜色
/**
 * @brief 颜色块填充
 * @param sx 起始X坐标
 * @param sy 起始Y坐标
 * @param ex 结束X坐标
 * @param ey 结束Y坐标
 * @param color 颜色数据指针
 *
 * 功能说明：
 * 1. 设置显示窗口
 * 2. 按行填充颜色数据
 * 3. 支持任意矩形区域
 */
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color)
{
    u16 height, width;
    u16 i, j;
    width = ex - sx + 1;  // 得到填充的宽度
    height = ey - sy + 1; // 高度
    for (i = 0; i < height; i++)
    {
        LCD_SetCursor(sx, sy + i); // 设置光标位置
        LCD_WriteRAM_Prepare();    // 开始写入GRAM
        for (j = 0; j < width; j++)
            LCD_WriteRAM(color[i * height + j]); // 写入数据
    }
}

/* 快速方向切换函数实现 */
void LCD_SetPortrait(void)
{
    LCD_Scan_Dir(PORTRAIT);
    lcddev.dir = 0; // 竖屏
    lcddev.width = LCD_W;
    lcddev.height = LCD_H;
}

void LCD_SetLandscape(void)
{
    LCD_Scan_Dir(LANDSCAPE);
    lcddev.dir = 1; // 横屏
    lcddev.width = LCD_H;
    lcddev.height = LCD_W;
}

/* 测试命令 */
static uint16_t color_array[] = {
    WHITE, BLACK, BLUE, RED,
    GREEN, YELLOW};

/**
 * @brief 绘制测试图案
 *
 * 功能说明：
 * 1. 绘制9宫格色块
 * 2. 每种颜色不同亮度
 * 3. 用于LCD功能测试
 */
static void LCD_DrawTestPattern(void)
{
    LOG_D("Starting optimized test pattern...");

/* 确保背光开启 */
#if (BACKLIGHT_ACTIVE_LEVEL == 1)
    LCD_BLK_SET;
#else
    LCD_BLK_CLR;
#endif

    /* 先清屏为白色 */
    LCD_Clear(WHITE);
    rt_thread_mdelay(100);

    /* 计算区域尺寸 */
    uint16_t w = lcddev.width / 3;
    uint16_t h = lcddev.height / 3;

    /* 绘制9个色块 */
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            uint16_t color;
            /* 确定基础颜色 */
            switch (col)
            {
            case 0:
                color = RED;
                break;
            case 1:
                color = GREEN;
                break;
            case 2:
                color = BLUE;
                break;
            }

            /* 调整亮度 */
            switch (row)
            {
            case 0:
                color |= 0xFFFF;
                break; // 全亮
            case 1:
                color &= 0xFFE0;
                break; // 半亮
            case 2:
                color &= 0xF800;
                break; // 暗
            }

            /* 计算区域坐标 */
            uint16_t x1 = col * w;
            uint16_t y1 = row * h;
            uint16_t x2 = (col == 2) ? lcddev.width - 1 : (col + 1) * w - 1;
            uint16_t y2 = (row == 2) ? lcddev.height - 1 : (row + 1) * h - 1;

            LOG_D("Drawing block %d-%d: (%d,%d)-(%d,%d) color=0x%04X",
                  row, col, x1, y1, x2, y2, color);

            /* 填充区域 */
            LCD_Fill(x1, y1, x2, y2, color);

            /* 添加短暂延时防止SPI过载 */
            rt_thread_mdelay(10);
        }
    }
    LOG_D("Test pattern complete");
}

/**
 * @brief LCD测试命令
 * @return 0-成功 -1-失败
 *
 * 功能说明：
 * 1. 测试字符显示功能
 * 2. 显示不同大小和颜色的文本
 * 3. 可通过RT-Thread MSH调用
 */
int lcd(void)
{
    // if (argc > 1)
    // {
    //     rt_kprintf("Usage: lcd_test\n");
    //     return -1;
    // }

    // if (!lcd_dev)
    // {
    //     rt_kprintf("LCD not initialized, run 'rt_hw_lcd_init' first\n");
    //     return -1;
    // }

    //    /* 显式设置为横屏模式 */
    //    rt_kprintf("Setting display to landscape mode...\n");
    //    LCD_SetLandscape();
    //    rt_thread_mdelay(100); // 确保方向设置完成

    //        /* 测试1: 纯色填充 */
    //        for (int i = 0; i < sizeof(color_array) / sizeof(color_array[0]); i++)
    //        {
    //            LCD_Clear(color_array[i]);
    //            rt_kprintf("Test1: Fill color 0x%04X\n", color_array[i]);
    //            rt_thread_mdelay(500);
    //        }
    //
    //        /* 测试2: 绘制测试图案 */
    //        LCD_Clear(BLACK);
    //        rt_kprintf("Test2: Drawing test pattern...\n");
    //        LCD_DrawTestPattern();
    //        rt_thread_mdelay(1000);

    //        /* 测试3: 绘制渐变 - 优化为分块绘制 */
              //LCD_Clear(BLACK);
    //        rt_kprintf("Test3: Drawing optimized gradient...\n");
    //
    //        /* 强制设置扫描方向为从左到右，从上到下 */
    //        rt_thread_mdelay(10); // 确保方向设置完成
    //
    //     #define GRADIENT_BLOCK_SIZE 16 // 16x16像素块
    //
    //        for (uint16_t block_y = 0; block_y < lcddev.height; block_y += GRADIENT_BLOCK_SIZE)
    //        {
    //            uint16_t end_y = (block_y + GRADIENT_BLOCK_SIZE > lcddev.height) ? lcddev.height : block_y + GRADIENT_BLOCK_SIZE;
    //
    //            for (uint16_t block_x = 0; block_x < lcddev.width; block_x += GRADIENT_BLOCK_SIZE)
    //            {
    //                uint16_t end_x = (block_x + GRADIENT_BLOCK_SIZE > lcddev.width) ? lcddev.width : block_x + GRADIENT_BLOCK_SIZE;
    //
    //                // 计算块四角颜色(双线性插值)
    //                uint16_t x1 = block_x;
    //                uint16_t y1 = block_y;
    //                uint16_t x2 = (block_x + GRADIENT_BLOCK_SIZE >= lcddev.width) ? lcddev.width - 1 : block_x + GRADIENT_BLOCK_SIZE;
    //                uint16_t y2 = (block_y + GRADIENT_BLOCK_SIZE >= lcddev.height) ? lcddev.height - 1 : block_y + GRADIENT_BLOCK_SIZE;
    //
    //                // 计算四角颜色
    //                uint16_t c00_r = (x1 * 31) / (lcddev.width - 1);
    //                uint16_t c00_g = (y1 * 63) / (lcddev.height - 1);
    //                uint16_t c00_b = 31 - (x1 * 31) / (lcddev.width - 1);
    //                uint16_t c00 = (c00_r << 11) | (c00_g << 5) | c00_b;
    //
    //                uint16_t c01_r = (x1 * 31) / (lcddev.width - 1);
    //                uint16_t c01_g = (y2 * 63) / (lcddev.height - 1);
    //                uint16_t c01_b = 31 - (x1 * 31) / (lcddev.width - 1);
    //                uint16_t c01 = (c01_r << 11) | (c01_g << 5) | c01_b;
    //
    //                uint16_t c10_r = (x2 * 31) / (lcddev.width - 1);
    //                uint16_t c10_g = (y1 * 63) / (lcddev.height - 1);
    //                uint16_t c10_b = 31 - (x2 * 31) / (lcddev.width - 1);
    //                uint16_t c10 = (c10_r << 11) | (c10_g << 5) | c10_b;
    //
    //                uint16_t c11_r = (x2 * 31) / (lcddev.width - 1);
    //                uint16_t c11_g = (y2 * 63) / (lcddev.height - 1);
    //                uint16_t c11_b = 31 - (x2 * 31) / (lcddev.width - 1);
    //                uint16_t c11 = (c11_r << 11) | (c11_g << 5) | c11_b;
    //
    //                // 填充块(逐行绘制渐变)
    //                for (uint16_t y = y1; y <= y2; y++)
    //                {
    //                    for (uint16_t x = x1; x <= x2; x++)
    //                    {
    //                        // 双线性插值计算颜色
    //                        float tx = (float)(x - x1) / (x2 - x1);
    //                        float ty = (float)(y - y1) / (y2 - y1);
    //
    //                        uint16_t r = (1 - tx) * (1 - ty) * c00_r + tx * (1 - ty) * c10_r +
    //                                     (1 - tx) * ty * c01_r + tx * ty * c11_r;
    //                        uint16_t g = (1 - tx) * (1 - ty) * c00_g + tx * (1 - ty) * c10_g +
    //                                     (1 - tx) * ty * c01_g + tx * ty * c11_g;
    //                        uint16_t b = (1 - tx) * (1 - ty) * c00_b + tx * (1 - ty) * c10_b +
    //                                     (1 - tx) * ty * c01_b + tx * ty * c11_b;
    //                        uint16_t color = (r << 11) | (g << 5) | b;
    //
    //                        LCD_DrawPoint(x, y, color);
    //                    }
    //                }
    //
    //                // 每5个块打印一次进度
    //                if ((block_x / GRADIENT_BLOCK_SIZE) % 5 == 0)
    //                {
    //                    rt_kprintf("Drawing block (%d,%d) color=0x%04X\n",
    //                               block_x, block_y);
    //                }
    //            }
    //
    //            // 每行块之间添加延时
    //            rt_thread_mdelay(5);
    //        }
    //        rt_thread_mdelay(500);

    /* 测试4: 显示字符 */
    // LCD_Display_Dir(0);
     //LCD_Clear(BLACK);
//    rt_kprintf("Test4: Drawing characters...\n");
//
//    // 显示不同大小和颜色的字符(仅测试支持的字体)
//    LCD_ShowString(10, 10, 200, 16, 16, (u8 *)"Moutain", WHITE, BACK_COLOR);
//    LCD_ShowString(10, 40, 200, 16, 16, (u8 *)"is a", RED, BACK_COLOR);
//    LCD_ShowString(10, 80, 200, 16, 16, (u8 *)"bias", GREEN, BACK_COLOR);
//    LCD_ShowString(10, 130, 200, 16, 16, (u8 *)"I realy burned it out.", BLUE, BACK_COLOR);
//
//    // 显示ASCII字符表
//    for (int i = 0; i < 5; i++)
//    {
//        char buf[20];
//        rt_snprintf(buf, sizeof(buf), "ASCII %d-%d", 32 + i * 16, 47 + i * 16);
//        LCD_ShowString(10, 160 + i * 20, 200, 16, 16, (u8 *)buf, YELLOW, BACK_COLOR);
//    }
//    rt_thread_mdelay(2000);
//
//    /* 测试5: 图形绘制测试 */
//    // LCD_Clear(BLACK);
//    rt_kprintf("\n=== Test5: Graphics drawing ===\n");
    //
    //        // 绘制线条
    //        for (int i = 0; i < 5; i++) {
    //            LCD_DrawLine(10, 30 + i*20, 200, 30 + i*20, color_array[i]);
    //        }
    //        rt_kprintf("Lines drawn\n");
    //        rt_thread_mdelay(1000);
    //
    //        // 绘制矩形
    //        LCD_DrawRectangle(50, 50, 150, 150, RED);
    //        LCD_DrawRectangle(60, 60, 140, 140, GREEN);
    //        rt_kprintf("Rectangles drawn\n");
    //        rt_thread_mdelay(1000);
    //
    //        // 绘制圆形
    //        for (int i = 0; i < 3; i++) {
    //            Draw_Circle(100, 100, 30 + i*10, BLUE);
    //        }
    //        rt_kprintf("Circles drawn\n");
    //        rt_thread_mdelay(1000);
    //
    //    /* 测试6: 扫描方向测试 */
    //    LCD_Clear(BLACK);
    //    rt_kprintf("\n=== Test6: Scan direction ===\n");
    //
    //    for (int dir = 0; dir < 8; dir++)
    //    {
    //        LCD_Scan_Dir(dir);
    //        LCD_Clear(BLACK);
    //        LCD_ShowString(10, 10, 200, 16, 16, (u8 *)"Direction Test", WHITE, BACK_COLOR);
    //        rt_kprintf("Direction %d tested\n", dir);
    //        rt_thread_mdelay(500);
    //    }
    //    // 恢复默认方向
    //    LCD_Scan_Dir(DFT_SCAN_DIR);

    //    /* 测试7: 性能测试 */
    //    LCD_Clear(BLACK);
    //    rt_kprintf("\n=== Test7: Performance test ===\n");
    //
    //    uint32_t start = rt_tick_get();
    //    for (int i = 0; i < 1000; i++)
    //    {
    //        LCD_DrawPoint(rand() % lcddev.width, rand() % lcddev.height, WHITE);
    //    }
    //    uint32_t end = rt_tick_get();
    //    rt_kprintf("1000 points drawn in %d ms\n", end - start);
    //    rt_thread_mdelay(1000);
    //
    //    return 0;
}


MSH_CMD_EXPORT(lcd, LCD test command : fill screen with different colors);
#endif
