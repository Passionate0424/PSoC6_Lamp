#if 0
#include "lcd_st7789.h"

#define DBG_TAG "st7789"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define PKG_USING_st7789

#ifdef PKG_USING_st7789

_lcd_dev lcddev;
static struct rt_spi_device *lcd_dev;

static void LCD_RESET(void)
{
    DELAY(25);
    LCD_RES_CLR;
    DELAY(25);
    LCD_RES_SET;
    DELAY(50);
}

static void LCD_WR_REG(uint8_t reg)
{
    LCD_DC_CLR;
    rt_spi_send(lcd_dev, &reg, 1);
    LCD_DC_SET;
}

static void LCD_WR_DATA(uint8_t data)
{
    LCD_DC_SET;
    rt_spi_send(lcd_dev, &data, 1);
}
static void LCD_WR_Continus_DATA(uint8_t *buff, uint8_t buff_size)
{
    LCD_DC_SET;
    rt_spi_send(lcd_dev, buff, buff_size);
}
static void LCD_ReadData(uint8_t *data, uint16_t length)
{
    LCD_DC_SET;
    rt_spi_transfer(lcd_dev, RT_NULL, &data, length);
}

static void LCD_WriteReg(uint8_t reg, uint16_t regdata)
{
    LCD_WR_REG(reg);
    LCD_WR_DATA(regdata);
}

static void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}

static void LCD_WriteData_16Bit(uint16_t Data)
{
    uint8_t buf[2];
    LCD_DC_SET;
    buf[0] = Data >> 8;
    buf[1] = Data & 0xff;
    rt_spi_send(lcd_dev, buf, 2);
}

void LCD_direction(uint8_t direction)
{
    lcddev.setxcmd = ST7789_CASET;
    lcddev.setycmd = ST7789_RASET;
    lcddev.wramcmd = ST7789_RAMWR;
    switch (direction)
    {
    case 0:
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        LCD_WriteReg(0x36, ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB); /* BGR==1,MY==0,MX==0,MV==0 */
        break;
    case 1:
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        LCD_WriteReg(0x36, ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB); /* BGR==1,MY==1,MX==0,MV==1 */
        break;
    case 2:
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        LCD_WriteReg(0x36, ST7789_MADCTL_RGB); /* BGR==1,MY==0,MX==0,MV==0 */
        break;
    case 3:
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        LCD_WriteReg(0x36, ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB); /* BGR==1,MY==1,MX==0,MV==1 */
        break;
    default:
        break;
    }
}

void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(xStar >> 8);
    LCD_WR_DATA(0x00FF & xStar);
    LCD_WR_DATA(xEnd >> 8);
    LCD_WR_DATA(0x00FF & xEnd);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(yStar >> 8);
    LCD_WR_DATA(0x00FF & yStar);
    LCD_WR_DATA(yEnd >> 8);
    LCD_WR_DATA(0x00FF & yEnd);

    LCD_WriteRAM_Prepare();
}

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD_SetWindows(Xpos, Ypos, Xpos, Ypos);
}

void LCD_Clear(uint16_t Color)
{
    unsigned int i, m;
    uint8_t buf[80];

    for (i = 0; i < 40; i++)
    {
        buf[2 * i] = Color >> 8;
        buf[2 * i + 1] = Color & 0xff;
    }

    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);

    LCD_DC_SET;
    for (i = 0; i < lcddev.height; i++)
    {
        for (m = 0; m < lcddev.width;)
        {
            m += 40;
            rt_spi_send(lcd_dev, buf, 80);
        }
    }



}

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    LCD_SetWindows(xsta, ysta, xend - 1, yend - 1);
    for (i = ysta; i < yend; i++)
    {
        for (j = xsta; j < xend; j++)
        {
            LCD_WriteData_16Bit(color);
        }
    }
}

void lcd_fill_array_spi(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, void *Image)
{
    rt_uint32_t size = 0;

    size = (Xend - Xstart + 1) * (Yend - Ystart + 1) * 2;/*16bit*/
    LCD_SetWindows(Xstart, Ystart, Xend, Yend);
    LCD_DC_SET;

    rt_spi_send(lcd_dev, Image, size);
}

static void _st7789_init(void)
{
    /* Internal LCD Voltage generator settings */
    LCD_WR_REG(0XB7);               //  Gate Control
    LCD_WR_DATA(0x35);          //  Default value
    LCD_WR_REG(0xBB);               //  VCOM setting
    LCD_WR_DATA(0x19);          //  0.725v (default 0.75v for 0x20)
    LCD_WR_REG(0xC0);               //  LCMCTRL
    LCD_WR_DATA (0x2C);         //  Default value
    LCD_WR_REG (0xC2);              //  VDV and VRH command Enable
    LCD_WR_DATA (0x01);         //  Default value
    LCD_WR_REG (0xC3);              //  VRH set
    LCD_WR_DATA (0x12);         //  +-4.45v (defalut +-4.1v for 0x0B)
    LCD_WR_REG (0xC4);              //  VDV set
    LCD_WR_DATA (0x20);         //  Default value
    LCD_WR_REG (0xC6);              //  Frame rate control in normal mode
    LCD_WR_DATA (0x0F);         //  Default value (60HZ)
    LCD_WR_REG (0xD0);              //  Power control
    LCD_WR_DATA (0xA4);         //  Default value
    LCD_WR_DATA (0xA1);         //  Default value
    /**************** Division line ****************/

    LCD_WR_REG(0xE0);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
        LCD_WR_Continus_DATA(data, sizeof(data));
    }

    LCD_WR_REG(0xE1);
    {
        uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
        LCD_WR_Continus_DATA(data, sizeof(data));
    }
    LCD_WR_REG (ST7789_INVON);      //  Inversion ON
    LCD_WR_REG (ST7789_SLPOUT); //  Out of sleep mode
    LCD_WR_REG (ST7789_NORON);      //  Normal Display on
    LCD_WR_REG (ST7789_DISPON); //  Main screen turned on
}

static void Lcd_pin_init(void)
{
    rt_pin_mode(PKG_ST_7789_DC_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(PKG_ST_7789_RES_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(PKG_ST_7789_BLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(PKG_ST_7789_CS_PIN, PIN_MODE_OUTPUT);
}

static void LCD_Init(void)
{
//    Lcd_pin_init();

    LCD_RESET();        /* LCD Hardware Reset */
//    LCD_WR_REG(0x11);   /* Sleep out */
//    DELAY(50);         /* Delay 120ms */

    //LCD_BLK_CLR;        /* Open Backlight */
    LCD_WR_REG(ST7789_COLMOD);      //  Set color mode
    LCD_WR_DATA(ST7789_COLOR_MODE_16bit);
    LCD_WR_REG(0xB2);               //  Porch control
    {
        uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
        LCD_WR_Continus_DATA(data, sizeof(data));
    }

    LCD_direction(ST7789_ROTATION);//   MADCTL (Display Rotation)

     _st7789_init();    /* st7789 init */
//     LCD_Clear(RED);
}

rt_err_t spi_device_attach(const char *bus_name, const char *device_name, rt_base_t cs_pin)
{
    RT_ASSERT(bus_name != RT_NULL);
    RT_ASSERT(device_name != RT_NULL);

    rt_err_t result = RT_EOK;
    struct rt_spi_device *spi_device;

    /* attach the device to spi bus*/
    spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
    RT_ASSERT(spi_device != RT_NULL);

    result=rt_hw_spi_device_attach(bus_name, device_name, cs_pin);
    if (RT_EOK != result)
    {
        LOG_E("%s attach to %s faild, %d\n", device_name, bus_name, result);
    }
    else
    {
        LOG_I("%s attach to %s done", device_name, bus_name);
    }

    return result;
}

void  spi_lcd_init(void)
{
    rt_err_t res = RT_EOK;
    Lcd_pin_init();
    spi_device_attach(PKG_ST_7789_SPI_BUS_NAME, PKG_ST_7789_SPI_DEVICE_NAME, PKG_ST_7789_CS_PIN);
    lcd_dev = (struct rt_spi_device *)rt_device_find(PKG_ST_7789_SPI_DEVICE_NAME);
    if (lcd_dev != RT_NULL)
    {
        struct rt_spi_configuration spi_config;
        spi_config.data_width = 8;
        spi_config.max_hz = 20 * 1000 * 1000;
        spi_config.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
        rt_spi_configure(lcd_dev, &spi_config);
    }
    else
    {
        res = -RT_ERROR;
    }

    LCD_Init();

    //return res;
}

INIT_COMPONENT_EXPORT(spi_lcd_init);
//MSH_CMD_EXPORT(_spi_lcd_init, st7789 init);
static uint16_t color_array[] =
{
    WHITE, BLACK, BLUE, BRED,
    GRED, GBLUE, RED, YELLOW
};

static int lcd_spi_test(void)
{
    uint8_t index = 0;
    uint16_t time_tick0=0,time_tick1=0;

    for (index = 0; index < sizeof(color_array) / sizeof(color_array[0]); index++)
    {
        time_tick0=rt_tick_get();
        LCD_Clear(color_array[index]);
        time_tick1=rt_tick_get();
        LOG_I("lcd clear color: %#x", color_array[index]);
        LOG_I("spend time:%d ms\n", time_tick1-time_tick0);
        DELAY(200);
    }

    return RT_EOK;
}
MSH_CMD_EXPORT(lcd_spi_test, lcd will fill color => you need init lcd first);
#endif
#endif
