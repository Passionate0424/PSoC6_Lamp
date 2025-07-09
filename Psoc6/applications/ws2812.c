#include <rtthread.h>
#include "ws2812.h"
#include <math.h>
#include <finsh.h>
#include "cyhal.h"
#include "cybsp.h"

/*
 * 存放PWM输出数据的二维数组，每行24位代表一个LED，最后一行24个0为RESET码
 */
CY_ALIGN(32)
uint16_t Pixel_Buf[Pixel_NUM + 1][24];

// ======= GPIO bitbang WS2812 驱动 =======

static void ws2812_gpio_init(void)
{
    cyhal_gpio_free(WS2812_PIN); // 释放引脚
    rt_pin_mode(WS2812_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(WS2812_PIN, PIN_LOW);
}

static void ws2812_sendarray_bitbang(uint16_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        uint16_t v = data[i];
        for (int b = 15; b >= 0; b--)
        {
            if (v & (1 << b))
            {
                rt_pin_write(WS2812_PIN, PIN_HIGH);
                rt_hw_us_delay(0.8); // T1H
                rt_pin_write(WS2812_PIN, PIN_LOW);
                rt_hw_us_delay(0.45); // T1L
            }
            else
            {
                rt_pin_write(WS2812_PIN, PIN_HIGH);
                rt_hw_us_delay(0.4); // T0H
                rt_pin_write(WS2812_PIN, PIN_LOW);
                rt_hw_us_delay(0.85); // T0L
            }
        }
    }
    // Reset
    rt_pin_write(WS2812_PIN, PIN_LOW);
    rt_hw_us_delay(60); // >50us
}

// 使用DMA批量输出Pixel_Buf到PWM compare buffer
void RGB_SendArray(void)
{
    ws2812_sendarray_bitbang(&Pixel_Buf[0][0], (Pixel_NUM + 1) * 24);
}

/*
 * 静态颜色定义
 */
const RGB_Color_TypeDef RED = {255, 0, 0};        // 红色
const RGB_Color_TypeDef ORANGE = {127, 106, 0};   // 橙色
const RGB_Color_TypeDef YELLOW = {127, 216, 0};   // 黄色
const RGB_Color_TypeDef GREEN = {0, 255, 0};      // 绿色
const RGB_Color_TypeDef CYAN = {0, 255, 255};     // 青色
const RGB_Color_TypeDef BLUE = {0, 0, 255};       // 蓝色
const RGB_Color_TypeDef PURPLE = {238, 130, 238}; // 紫色
const RGB_Color_TypeDef BLACK = {0, 0, 0};        // 黑色
const RGB_Color_TypeDef WHITE = {255, 255, 255};  // 白色
const RGB_Color_TypeDef MAGENTA = {255, 0, 220};  // 品红

RGB_Color_TypeDef load[16];

// 创建 Gamma 修正查找表
uint8_t gamma_correction_table[256];

/* 初始化 Gamma 修正查找表
 * 无参数
 */
void init_gamma_correction_table()
{
    for (int i = 0; i < 256; i++)
    {
        gamma_correction_table[i] = (uint8_t)(pow(i / 255.0, GAMMA) * 255.0);
    }
}

/* Gamma校正函数（查找表实现）
 * @param color 原始颜色分量值
 * @return 校正后的颜色分量值
 */
uint8_t gamma_correction(uint8_t color)
{
    return gamma_correction_table[color];
}

/*
 * 设置单个LED的颜色，将RGB结构体转为24位PWM码
 * @param LedId LED序号（从0开始）
 * @param Color 颜色结构体
 */
void RGB_SetColor(uint8_t LedId, RGB_Color_TypeDef Color)
{
    uint8_t i;
    if (LedId > Pixel_NUM)
        return; // avoid overflow 防止写入ID大于LED总数
    if (1)      // gamma修正
    {
        Color.G = gamma_correction(Color.G);
        Color.B = gamma_correction(Color.B);
        Color.R = gamma_correction(Color.R);
    }
    for (i = 0; i < 8; i++)
        Pixel_Buf[LedId][i] = ((Color.G & (1 << (7 - i))) ? (CODE_1) : CODE_0); // 数组某一行0~7转化存放G
    for (i = 8; i < 16; i++)
        Pixel_Buf[LedId][i] = ((Color.R & (1 << (15 - i))) ? (CODE_1) : CODE_0); // 数组某一行8~15转化存放R
    for (i = 16; i < 24; i++)
        Pixel_Buf[LedId][i] = ((Color.B & (1 << (23 - i))) ? (CODE_1) : CODE_0); // 数组某一行16~23转化存放B
}

/*
 * 设置最后一行为RESET码，输出24个0，满足WS2812复位时序
 * 无参数
 */
void Reset_Load(void)
{
    uint8_t i;
    for (i = 0; i < 24; i++)
    {
        Pixel_Buf[Pixel_NUM][i] = 0;
    }
}

/*
 * 点亮单个LED
 * @param pos LED序号（从0开始）
 * @param rgb 颜色结构体
 */
void single_led(uint8_t pos, RGB_Color_TypeDef rgb)
{
    RGB_SetColor(pos, rgb);
    Reset_Load();
    RGB_SendArray();
}

/*
 * 显示红色
 * @param Pixel_Len 显示LED个数
 */
void RGB_RED(uint16_t Pixel_Len)
{
    uint16_t i;
    for (i = 0; i < Pixel_Len; i++) // 给对应个数LED写入红色
    {
        RGB_SetColor(i, RED);
    }
    Reset_Load();
    RGB_SendArray();
}

/*
 * 显示绿色
 * @param Pixel_Len 显示LED个数
 */
void RGB_GREEN(uint16_t Pixel_Len)
{
    uint16_t i;
    for (i = 0; i < Pixel_Len; i++) // 给对应个数LED写入绿色
    {
        RGB_SetColor(i, GREEN);
    }
    Reset_Load();
    RGB_SendArray();
}

/*
 * 显示蓝色
 * @param Pixel_Len 显示LED个数
 */
void RGB_BLUE(uint16_t Pixel_Len)
{
    uint16_t i;
    for (i = 0; i < Pixel_Len; i++) // 给对应个数LED写入蓝色
    {
        RGB_SetColor(i, BLUE);
    }
    Reset_Load();
    RGB_SendArray();
}

/*
 * 显示白色
 * @param Pixel_Len 显示LED个数
 */
void RGB_WHITE(uint16_t Pixel_Len)
{
    uint16_t i;
    for (i = 0; i < Pixel_Len; i++) // 给对应个数LED写入白色
    {
        RGB_SetColor(i, WHITE);
    }
    Reset_Load();
    RGB_SendArray();
}

/*
 * 显示自定义亮度白色
 * @param Pixel_Len 显示LED个数
 * @param bright 亮度值（0~255）
 */
void RGB_WHITE_Change(uint16_t Pixel_Len, uint8_t bright)
{
    uint16_t i;
    RGB_Color_TypeDef white;
    white.B = bright;
    white.G = bright;
    white.R = bright;
    for (i = 0; i < Pixel_Len; i++) // 给对应个数LED写入自定义亮度白色
    {
        RGB_SetColor(i, white);
    }
    Reset_Load();
    RGB_SendArray();
}

/*
 * 显示黑色
 * @param Pixel_Len 显示LED个数
 */
void RGB_BLACK(uint16_t Pixel_Len)
{
    uint16_t i;
    for (i = 0; i < Pixel_Len; i++) // 给对应个数LED写入白色
    {
        RGB_SetColor(i, BLACK);
    }
    Reset_Load();
    RGB_SendArray();
}

/*
 * 显示加载动画
 * @param Pixel_Len 显示LED个数
 */
void RGB_loading(uint16_t Pixel_Len)
{
    uint16_t i;
    for (i = 0; i <= 15; i++) // 给对应个数LED写入
    {
        RGB_SetColor(i, load[i]);
    }
    Reset_Load();
    RGB_SendArray();
}

/*
 * 启动动画
 * 无参数
 */
void RGB_start(void)
{
    for (int i = 0; i <= 15; i++)
    {
        if (load[i].R >= 16)
            load[i].R -= 16;
        else
            load[i].R = 255;

        if (load[i].G >= 16)
            load[i].G -= 16;
        else
            load[i].G = 255;

        if (load[i].B >= 16)
            load[i].B -= 16;
        else
            load[i].B = 255;
    }
    RGB_loading(Pixel_NUM);
}

// 也可以继续添加其他颜色，和颜色变化函数等

/*******************************************************************************/
/*                                  扩展部分                                   */

/*
 * 显示指定颜色
 * @param Pixel_Len 显示LED个数
 * @param rgb 颜色结构体
 */
static void rgb_show(uint32_t Pixel_Len, RGB_Color_TypeDef rgb)
{
    uint16_t i;
    for (i = 0; i < Pixel_Len; i++)
    {
        RGB_SetColor(i, rgb);
    }
    Reset_Load();
    RGB_SendArray();
}

/*
 * 颜色循环转换
 * @param WheelPos 颜色轮位置（0~255）
 * @return 颜色结构体
 */
static RGB_Color_TypeDef Wheel(uint8_t WheelPos)
{
    RGB_Color_TypeDef rgb;
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        rgb.R = 255 - WheelPos * 3;
        rgb.G = 0;
        rgb.B = WheelPos * 3;
        return rgb;
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        rgb.R = 0;
        rgb.G = WheelPos * 3;
        rgb.B = 255 - WheelPos * 3;
        return rgb;
    }
    WheelPos -= 170;
    rgb.R = WheelPos * 3;
    rgb.G = 255 - WheelPos * 3;
    rgb.B = 0;
    return rgb;
}

/*
 * 彩虹呼吸灯
 * @param wait 每步延时，单位ms
 */
static void rainbow(uint8_t wait)
{
    uint32_t timestamp = rt_tick_get();
    uint16_t i;
    static uint8_t j;
    static uint32_t next_time = 0;

    uint32_t flag = 0;
    if (next_time < wait)
    {
        if ((uint64_t)timestamp + wait - next_time > 0)
            flag = 1;
    }
    else if (timestamp > next_time)
    {
        flag = 1;
    }
    if (flag)
    {
        j++;
        next_time = timestamp + wait;
        for (i = 0; i < Pixel_NUM; i++)
        {
            RGB_SetColor(i, Wheel((i + j) & 255));
        }
    }
    RGB_SendArray();
}

/*
 * 彩虹灯旋转
 * @param wait 每步延时，单位ms
 */
static void rainbowCycle(uint8_t wait)
{
    uint32_t timestamp = rt_tick_get();
    uint16_t i;
    static uint8_t j;
    static uint32_t next_time = 0;

    static uint8_t loop = 0;
    if (loop == 0)
        next_time = timestamp;
    loop = 1;

    if ((timestamp > next_time))
    {
        j++;
        next_time = timestamp + wait;
        for (i = 0; i < Pixel_NUM; i++)
        {
            RGB_SetColor(i, Wheel(((i * 256 / (Pixel_NUM)) + j) & 255));
        }
    }
    RGB_SendArray();
}

static uint8_t rainbow_change_flag = 0;
void led_loop(void)
{
    int i;
    rgb_show(16, BLACK);
    rt_thread_mdelay(300);
    for (i = 1; i <= 16; i++)
    {
        rgb_show(i, RED);
        rt_thread_mdelay(50);
    }
    for (i = 1; i <= 16; i++)
    {
        rgb_show(i, ORANGE);
        rt_thread_mdelay(50);
    }
    for (i = 1; i <= 16; i++)
    {
        rgb_show(i, YELLOW);
        rt_thread_mdelay(50);
    }
    for (i = 1; i <= 16; i++)
    {
        rgb_show(i, GREEN);
        rt_thread_mdelay(50);
    }
    for (i = 1; i <= 16; i++)
    {
        rgb_show(i, CYAN);
        rt_thread_mdelay(50);
    }
    for (i = 1; i <= 16; i++)
    {
        rgb_show(i, BLUE);
        rt_thread_mdelay(50);
    }
    for (i = 1; i <= 16; i++)
    {
        rgb_show(i, PURPLE);
        rt_thread_mdelay(50);
    }
    /*
    // 若需定时器中断可用rt_timer，暂不实现
    */
}

/*
 * 加载动画初始化
 * 无参数
 */
void loading_init(void)
{
    int i = 0;
    init_gamma_correction_table();
    for (int i = 0; i <= 15; i++)
    {
        load[i].R = i * 16 + 15;
        load[i].G = i * 16 + 15;
        load[i].B = i * 16 * 15;
    }
}

/*
 * LED加载动画
 * 无参数
 */
void led_loading(void)
{
    int i;
    rgb_show(16, BLACK);
    rt_thread_mdelay(300);
    loading_init();
    for (i = 0; i <= 15; i++)
    {
        rgb_show(1, load[(i + 1) % 16]);
        rgb_show(2, load[(i + 2) % 16]);
        rgb_show(3, load[(i + 3) % 16]);
        rgb_show(4, load[(i + 4) % 16]);
        rgb_show(5, load[(i + 5) % 16]);
        rgb_show(6, load[(i + 6) % 16]);
        rgb_show(7, load[(i + 7) % 16]);
        rgb_show(8, load[(i + 8) % 16]);
        rgb_show(9, load[(i + 9) % 16]);
        rgb_show(10, load[(i + 10) % 16]);
        rgb_show(11, load[(i + 11) % 16]);
        rgb_show(12, load[(i + 12) % 16]);
        rgb_show(13, load[(i + 13) % 16]);
        rgb_show(14, load[(i + 14) % 16]);
        rgb_show(15, load[(i + 15) % 16]);
        rgb_show(16, load[(i + 16) % 16]);
        if (i == 15)
            i = 0;
        rt_thread_mdelay(300);
    }
}

// MSH command: turn all LEDs red
static void msh_ws2812_red(int argc, char **argv)
{
    RGB_RED(Pixel_NUM);
}
MSH_CMD_EXPORT(msh_ws2812_red, turn all LEDs red);

// MSH command: turn all LEDs green
static void msh_ws2812_green(int argc, char **argv)
{
    RGB_GREEN(Pixel_NUM);
}
MSH_CMD_EXPORT(msh_ws2812_green, turn all LEDs green);

// MSH command: turn all LEDs blue
static void msh_ws2812_blue(int argc, char **argv)
{
    RGB_BLUE(Pixel_NUM);
}
MSH_CMD_EXPORT(msh_ws2812_blue, turn all LEDs blue);

// MSH command: turn off all LEDs
static void msh_ws2812_off(int argc, char **argv)
{
    RGB_BLACK(Pixel_NUM);
}
MSH_CMD_EXPORT(msh_ws2812_off, turn off all LEDs);

// MSH command: rainbow cycle demo
static void msh_ws2812_rainbow(int argc, char **argv)
{
    for (int i = 0; i < 100; i++)
    {
        rainbowCycle(10);
        rt_thread_mdelay(10);
    }
}
MSH_CMD_EXPORT(msh_ws2812_rainbow, rainbow cycle demo);

MSH_CMD_EXPORT(ws2812_gpio_init, ws2812 hardware init);
