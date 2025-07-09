#if 1
/**
 * @file lcd_rtthread.h
 * @brief ST7789 LCD驱动 - RT-Thread适配版
 *
 * 详细接口修改说明：
 *
 * 一、完全重写的底层接口：
 * 1. LCD_WR_REG() - 改用rt_spi_send实现SPI写寄存器
 * 2. LCD_WR_DATA() - 改用rt_spi_send实现SPI写数据
 * 3. LCD_WriteRAM() - 基于RT-Thread SPI接口重写
 * 4. lcd_pin_init() - 改用rt_pin_mode配置GPIO
 *
 * 二、部分修改的函数：
 * 1. LCD_Init() - 保留初始化流程，但实现改为：
 *    - 使用rt_spi_transfer代替直接寄存器操作
 *    - 延时改用rt_thread_mdelay
 *    - 背光控制改用rt_pin_write
 *
 * 三、新增RT-Thread特有接口：
 * 1. spi_lcd_init() - 设备注册和初始化入口
 *
 * 四、完全保留的API：
 * 1. 所有绘图函数(LCD_DrawPoint等)
 * 2. 文本显示函数(LCD_ShowChar等)
 * 3. 颜色和方向定义
 *
 * 五、删除的内容：
 * 1. 原硬件相关的寄存器操作宏
 * 2. 原裸机延时函数
 */
#ifndef __LCD_RTTHREAD_H__
#define __LCD_RTTHREAD_H__

#include <rtthread.h>
//#include "font.h" /* 包含字体数据头文件 */
//#include <rtdevice.h>

/* 背景色定义 */
#define BACK_COLOR BLACK

/* [移植修改点1] LCD尺寸定义 - 根据屏幕规格修改 */
#define LCD_W 240  /* 屏幕宽度(像素) */
#define LCD_H 320  /* 屏幕高度(像素) */

#define FAST     1  //快速刷图  1:快速刷图  0：慢刷   快刷比慢刷快24%


/* [移植修改点2] 硬件引脚定义 - 根据实际硬件修改 */
#define LCD_DC_PIN GET_PIN(10, 3)    /* 数据/命令选择引脚 */
#define LCD_RES_PIN GET_PIN(5, 7)   /* 复位引脚 */
#define LCD_BLK_PIN GET_PIN(11, 5)   /* 背光控制引脚 */
#define LCD_CS_PIN GET_PIN(0, 5)     /* SPI片选引脚 */
#define LCD_SPI_BUS "spi0"          /* SPI总线名称 */

/* 引脚控制宏 */
#define LCD_RES_CLR rt_pin_write(LCD_RES_PIN, PIN_LOW)
#define LCD_RES_SET rt_pin_write(LCD_RES_PIN, PIN_HIGH)
#define LCD_DC_CLR rt_pin_write(LCD_DC_PIN, PIN_LOW)
#define LCD_DC_SET rt_pin_write(LCD_DC_PIN, PIN_HIGH)
#define LCD_BLK_CLR rt_pin_write(LCD_BLK_PIN, PIN_HIGH) // 背光低电平点亮
#define LCD_BLK_SET rt_pin_write(LCD_BLK_PIN, PIN_LOW)
#define DELAY(ms) rt_thread_mdelay(ms)

/* 兼容类型定义 */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

// 扫描方向定义
#define L2R_U2D 0 // 从左到右,从上到下
#define L2R_D2U 1 // 从左到右,从下到上
#define R2L_U2D 2 // 从右到左,从上到下
#define R2L_D2U 3 // 从右到左,从下到上

#define U2D_L2R 4 // 从上到下,从左到右
#define U2D_R2L 5 // 从上到下,从右到左
#define D2U_L2R 6 // 从下到上,从左到右
#define D2U_R2L 7 // 从下到上,从右到左

// 常用方向预设
#define PORTRAIT U2D_R2L  // 竖屏方向
#define LANDSCAPE L2R_U2D // 横屏方向
//横竖屏切换

#define Landscape 1 //1:横显示  0：竖显示  PS:横竖按照FPC的出线方向来看
extern u8 DFT_SCAN_DIR;

// 快速方向切换函数
void LCD_SetPortrait(void);  // 设置为竖屏
void LCD_SetLandscape(void); // 设置为横屏

/* LCD设备结构体 */
typedef struct
{
    u16 width;  // LCD宽度
    u16 height; // LCD高度
    u8 dir;     // 横屏/竖屏
    u8 wramcmd; // 写GRAM指令
    u8 setxcmd; // 设置X坐标指令
    u8 setycmd; // 设置Y坐标指令
} _lcd_dev;

/* 全局变量声明 */
extern _lcd_dev lcddev;

/* 基本颜色定义 */
#define WHITE 0xFFFF
#define BLACK 0x0000
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define YELLOW 0xFFE0

/* 函数声明 */
int spi_lcd_init(void); /* LCD驱动初始化 */

/* 基本绘图函数 */
void LCD_Init(void);
void LCD_Clear(u16 Color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x, u16 y, u16 color);
static void LCD_WR_REG(uint8_t reg);
static void LCD_WR_DATA(uint8_t data);
static void LCD_WR_DATA_16BIT(uint16_t data);
static void LCD_WriteRAM(uint16_t RGB_Code);
static void LCD_WriteRAM_Prepare(void);
static void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
static void LCD_SetWindows(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height);
static void LCD_SetWindow(u16 sx, u16 sy, u16 ex, u16 ey);

/* 高级绘图函数 */
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color);
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);
void LCD_ShowImage(u16 x, u16 y, u16 width, u16 height, const u16 *p);
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color);

/* 文本显示函数 */
void LCD_ShowChar(u16 x, u16 y, char chr, u8 size, u8 mode, u16 color, u16 bg_color);
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p, u16 color, u16 bg_color);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size, u16 color, uint16_t bg_color);
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode, u16 color, uint16_t bg_color);

/* 辅助功能函数 */
u32 LCD_Pow(u8 m, u8 n);
void LCD_Scan_Dir(u8 dir);
void LCD_Display_Dir(u8 dir);

int lcd(void);

#endif /* __LCD_RTTHREAD_H__ */
#endif
