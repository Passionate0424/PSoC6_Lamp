#if 0
#ifndef __LCD_ST7789_H__
#define __LCD_ST7789_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "drivers/spi.h"
#include "drv_spi.h"
typedef struct
{
    uint16_t width;   /* LCD width */
    uint16_t height;  /* LCD high */
    uint32_t id;      /* LCD ID */
    uint8_t dir;      /* 0:Vertical | 1:Vertical */
    uint16_t wramcmd; /* gram cmd */
    uint16_t setxcmd; /* set x cmd */
    uint16_t setycmd; /* set y cmd */
} _lcd_dev;

/* LCD param */
extern _lcd_dev lcddev;

/* 0-0 angle|1-90 angle|2-180 angle|-270 angle */
#define USE_DIRECTION   0

/* lcd size */
#define LCD_W 240
#define LCD_H 320

#define PKG_ST_7789_SPI_BUS_NAME    "spi0"
#define PKG_ST_7789_SPI_DEVICE_NAME "spi01"
#define PKG_ST_7789_DC_PIN  GET_PIN(5, 6)
#define PKG_ST_7789_RES_PIN  GET_PIN(5, 7)
#define PKG_ST_7789_BLK_PIN  GET_PIN(11, 5)
#define PKG_ST_7789_CS_PIN   GET_PIN(0, 5)

#define LCD_DC_CLR  rt_pin_write(PKG_ST_7789_DC_PIN, PIN_LOW)
#define LCD_DC_SET  rt_pin_write(PKG_ST_7789_DC_PIN, PIN_HIGH)
#define LCD_RES_CLR rt_pin_write(PKG_ST_7789_RES_PIN, PIN_LOW)
#define LCD_RES_SET rt_pin_write(PKG_ST_7789_RES_PIN, PIN_HIGH)
#define LCD_BLK_CLR rt_pin_write(PKG_ST_7789_BLK_PIN, PIN_HIGH)
#define DELAY       rt_thread_mdelay



/*
 * Comment one to use another.
 * 3 parameters can be choosed
 * 135x240(0.96 inch) & 240x240(1.3inch) & 170x320(1.9inch)
 * X_SHIFT & Y_SHIFT are used to adapt different display's resolution
 */

/* Choose a type you are using */
//#define USING_135X240
#define USING_240X240
//#define USING_170X320

/* Choose a display rotation you want to use: (0-3) */
//#define ST7789_ROTATION 0
//#define ST7789_ROTATION 1
#define ST7789_ROTATION 0              //  use Normally on 240x240
//#define ST7789_ROTATION 3

#ifdef USING_135X240

    #if ST7789_ROTATION == 0
        #define ST7789_WIDTH 135
        #define ST7789_HEIGHT 240
        #define X_SHIFT 53
        #define Y_SHIFT 40
    #endif

    #if ST7789_ROTATION == 1
        #define ST7789_WIDTH 240
        #define ST7789_HEIGHT 135
        #define X_SHIFT 40
        #define Y_SHIFT 52
    #endif

    #if ST7789_ROTATION == 2
        #define ST7789_WIDTH 135
        #define ST7789_HEIGHT 240
        #define X_SHIFT 52
        #define Y_SHIFT 40
    #endif

    #if ST7789_ROTATION == 3
        #define ST7789_WIDTH 240
        #define ST7789_HEIGHT 135
        #define X_SHIFT 40
        #define Y_SHIFT 53
    #endif

#endif

#ifdef USING_240X240

    #define ST7789_WIDTH 240
    #define ST7789_HEIGHT 240

        #if ST7789_ROTATION == 0
            #define X_SHIFT 0
            #define Y_SHIFT 80
        #elif ST7789_ROTATION == 1
            #define X_SHIFT 80
            #define Y_SHIFT 0
        #elif ST7789_ROTATION == 2
            #define X_SHIFT 0
            #define Y_SHIFT 0
        #elif ST7789_ROTATION == 3
            #define X_SHIFT 0
            #define Y_SHIFT 0
        #endif

#endif

#ifdef USING_170X320

    #if ST7789_ROTATION == 0
        #define ST7789_WIDTH 170
        #define ST7789_HEIGHT 320
        #define X_SHIFT 35
        #define Y_SHIFT 0
    #endif

    #if ST7789_ROTATION == 1
        #define ST7789_WIDTH 320
        #define ST7789_HEIGHT 170
        #define X_SHIFT 0
        #define Y_SHIFT 35
    #endif

    #if ST7789_ROTATION == 2
        #define ST7789_WIDTH 170
        #define ST7789_HEIGHT 320
        #define X_SHIFT 35
        #define Y_SHIFT 0
    #endif

    #if ST7789_ROTATION == 3
        #define ST7789_WIDTH 320
        #define ST7789_HEIGHT 170
        #define X_SHIFT 0
        #define Y_SHIFT 35
    #endif

#endif

/**
 *Color of pen
 *If you want to use another color, you can choose one in RGB565 format.
 */

#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define GRAY        0X8430
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define BROWN       0XBC40
#define BRRED       0XFC07
#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458

#define LIGHTGREEN  0X841F
#define LGRAY       0XC618
#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12

/* Control Registers and constant codes */
#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

/**
 * Memory Data Access Control Register (0x36H)
 * MAP:     D7  D6  D5  D4  D3  D2  D1  D0
 * param:   MY  MX  MV  ML  RGB MH  -   -
 *
 */

/* Page Address Order ('0': Top to Bottom, '1': the opposite) */
#define ST7789_MADCTL_MY  0x80
/* Column Address Order ('0': Left to Right, '1': the opposite) */
#define ST7789_MADCTL_MX  0x40
/* Page/Column Order ('0' = Normal Mode, '1' = Reverse Mode) */
#define ST7789_MADCTL_MV  0x20
/* Line Address Order ('0' = LCD Refresh Top to Bottom, '1' = the opposite) */
#define ST7789_MADCTL_ML  0x10
/* RGB/BGR Order ('0' = RGB, '1' = BGR) */
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1   0xDA
#define ST7789_RDID2   0xDB
#define ST7789_RDID3   0xDC
#define ST7789_RDID4   0xDD

/* Advanced options */
#define ST7789_COLOR_MODE_16bit 0x05    //  RGB565 (16bit)
#define ST7789_COLOR_MODE_18bit 0x66    //  RGB666 (18bit)

void LCD_Clear(uint16_t Color);
void LCD_direction(uint8_t direction);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void lcd_fill_array_spi(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, void *pcolor);

void spi_lcd_init(void);

#ifdef __cplusplus
}
#endif

#endif
#endif
