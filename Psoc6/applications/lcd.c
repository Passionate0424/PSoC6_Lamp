#if 0
/**
 ****************************************************************************************************
 * @file        lcd.c
 * @author      EYA-DISPLAY
 * @version     V2.0
 * @date        2022-04-28
 * @brief       液晶屏驱动Demo
 * @license     Copyright (c) 2022-2032, 亦亞徽科技集团(广东)
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:EYA-ETmcu开发板
 * 公司网址:www.eya-display.com
 *
 ****************************************************************************************************
 **/
#include "lcd.h"
#include "stdlib.h"
#include "font.h"
#include "usart.h"
#include "delay.h"
#include "image2lcd.h"
#include "SPI2.h"

//LCD初始化放在这个函数中
void LCD_INIT_CODE(void)
{
    LCD_WR_REG(0x11);
    delay_ms(120); //Delay 120ms
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


u8 DFT_SCAN_DIR; //扫描方向变量
//////////////////////////////////////////////////////////////////////////////////
extern const u8 gImage_logo[];   //图片数据(包含信息头),存储在image1.c里面.
//LCD的画笔颜色和背景色
u16 POINT_COLOR=0x0000; //画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;


#if(Interface==D4WSPI)
    //写寄存器函数  SPI接口需要用到的函数
    //regval:寄存器值
    void LCD_WR_REG(u8 regval)
    {
        LCD_RS=0;//写地址
        LCD_CS=0;
        SPI2_ReadWriteByte(regval);
        LCD_CS=1;
    }
    //写LCD数据
    //data:要写入的值
    void LCD_WR_DATA(u16 data)
    {
        LCD_RS=1;//写参数
        LCD_CS=0;
        SPI2_ReadWriteByte(data);
        LCD_CS=1;
    }
    //LCD写GRAM
    //RGB_Code:颜色值
    void LCD_WriteRAM(u16 RGB_Code)
    {
        LCD_RS=1;
        LCD_CS=0;
        SPI2_ReadWriteByte(RGB_Code>>8);
        SPI2_ReadWriteByte(RGB_Code);
        LCD_CS=1;
    }
#elif(Interface==I80_16BIT)
    //写寄存器函数  //并口用到的函数
    //regval:寄存器值
    void LCD_WR_REG(u8 regval)
    {
        LCD_RS=0;//写地址
        LCD_CS=0;
        DATAOUT(regval);
        LCD_WR=0;
        LCD_WR=1;
        LCD_CS=1;
    }
    //写LCD数据
    //data:要写入的值
    void LCD_WR_DATA(u16 data)
    {
        LCD_RS=1;
        LCD_CS=0;
        DATAOUT(data);
        LCD_WR=0;
        LCD_WR=1;
        LCD_CS=1;
    }
    //LCD写GRAM
    //RGB_Code:颜色值
    void LCD_WriteRAM(u16 RGB_Code)
    {
        LCD_RS=1;
        LCD_CS=0;
        DATAOUT(RGB_Code);
        LCD_WR=0;
        LCD_WR=1;
        LCD_CS=1;
    }
#else
    //写寄存器函数
    //regval:寄存器值
    void LCD_WR_REG(u8 regval)
    {
        LCD_RS=0;//写地址
        LCD_CS=0;
        DATAOUT(regval);
        LCD_WR=0;
        LCD_WR=1;
        LCD_CS=1;
    }
    //写LCD数据
    //data:要写入的值
    void LCD_WR_DATA(u16 data)
    {
        LCD_RS=1;
        LCD_CS=0;
        DATAOUT(data);
        LCD_WR=0;
        LCD_WR=1;
        LCD_CS=1;
    }
    //LCD写GRAM
    //RGB_Code:颜色值
    void LCD_WriteRAM(u16 RGB_Code)
    {
        LCD_RS=1;
        LCD_CS=0;
        DATAOUT(RGB_Code>>8);
        LCD_WR=0;
        LCD_WR=1;
        DATAOUT(RGB_Code);
        LCD_WR=0;
        LCD_WR=1;
        LCD_CS=1;
    }
#endif

//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}


//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
    while(i--);
}

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(Xpos>>8);
    LCD_WR_DATA(Xpos&0XFF);
    LCD_WR_DATA(Xpos>>8);
    LCD_WR_DATA(Xpos&0XFF);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(Ypos>>8);
    LCD_WR_DATA(Ypos&0XFF);
    LCD_WR_DATA(Ypos>>8);
    LCD_WR_DATA(Ypos&0XFF);
}
//设置LCD的自动扫描方向
//dir:0~7,代表8个方向(具体定义见lcd.h)
void LCD_Scan_Dir(u8 dir)
{
    u16 regval=0;
    u8 dirreg=0;
//  u16 temp;
    switch(dir)//方向转换
    {
        case 0:dir=6;break;
        case 1:dir=7;break;
        case 2:dir=4;break;
        case 3:dir=5;break;
        case 4:dir=1;break;
        case 5:dir=0;break;
        case 6:dir=3;break;
        case 7:dir=2;break;
    }
    switch(dir)
    {
        case L2R_U2D://从左到右,从上到下
            regval|=(0<<7)|(0<<6)|(0<<5);
            break;
        case L2R_D2U://从左到右,从下到上
            regval|=(1<<7)|(0<<6)|(0<<5);
            break;
        case R2L_U2D://从右到左,从上到下
            regval|=(0<<7)|(1<<6)|(0<<5);
            break;
        case R2L_D2U://从右到左,从下到上
            regval|=(1<<7)|(1<<6)|(0<<5);
            break;
        case U2D_L2R://从上到下,从左到右
            regval|=(0<<7)|(0<<6)|(1<<5);
            break;
        case U2D_R2L://从上到下,从右到左
            regval|=(0<<7)|(1<<6)|(1<<5);
            break;
        case D2U_L2R://从下到上,从左到右
            regval|=(1<<7)|(0<<6)|(1<<5);
            break;
        case D2U_R2L://从下到上,从右到左
            regval|=(1<<7)|(1<<6)|(1<<5);
            break;
    }
    dirreg=0X36;
  regval|=0x00; //0x08 0x00  红蓝反色可以通过这里修改
    LCD_WriteReg(dirreg,regval);

    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(0);LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(0);LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);


}
//画点
//x,y:坐标
//color: 点的颜色(32位颜色,方便兼容LTDC)
void LCD_DrawPoint(u16 x,u16 y, uint32_t color)
{
    LCD_SetCursor(x,y);     //设置光标位置
    LCD_WriteRAM_Prepare(); //开始写入GRAM
    LCD_WriteRAM(color);
}

//设置LCD显示方向（6804不支持横屏显示）
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
    if(dir==0)
    {
        lcddev.dir=0;   //竖屏
        lcddev.width=LCD_WIDTH;
        lcddev.height=LCD_HEIGHT;

        lcddev.wramcmd=0X2C;
        lcddev.setxcmd=0X2A;
        lcddev.setycmd=0X2B;
    DFT_SCAN_DIR=U2D_R2L;       //竖显-设定显示方向

    }else                 //横屏
    {
        lcddev.dir=1;
        lcddev.width=LCD_HEIGHT;
        lcddev.height=LCD_WIDTH;

        lcddev.wramcmd=0X2C;
        lcddev.setxcmd=0X2A;
        lcddev.setycmd=0X2B;
    DFT_SCAN_DIR=L2R_U2D;     //横显-设定显示方向

    }
    LCD_Scan_Dir(DFT_SCAN_DIR); //默认扫描方向
}

//送亮度值
void SendPWMval(u16 value)
{
    value = 1000-value*100;

    TIM2->CCR4 = value;
}
//背光控制口初始化
void TIM2_PWM_Init(u16 arr,u16 psc)
{
    //此部分需手动修改IO口设置
    RCC->APB1ENR|=1<<0;      //TIM2时钟使能
    RCC->APB2ENR|=1<<2;        //使能PORTA时钟

    GPIOA->CRL&=0xFFFF0FFF;//PA3
    GPIOA->CRL|=0x0000B000;//
    GPIOA->ODR|=0x08<<0;//

    TIM2->ARR=arr;//
    TIM2->PSC=psc;//

    TIM2->CCER|=1<<12;

    TIM2->CR1=0x8000;   //
    TIM2->CR1|=0x01;    //

    TIM2->CCR4 = 1000;//关闭背光

    TIM2->CCMR2|=7<<12;      //CH4 PWM4模式
    TIM2->CCMR2|=1<<11;      //CH4 预装载使能
}
//初始化lcd
void LCD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA B C
    //只启用SWD，其他JTAG脚用作普通IO  演示板需要用到JTAG引脚
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

#if(Interface==D4WSPI)
    //RS使用的IO口初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  //PA11推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_11);//拉高

    //LCD 复位脚使用到的IO口初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                    //PC13 推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //LCD CS脚使用到的IO口初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                    //PB12 推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_12);//拉高

  SPI2_Init();
#else
    //CS RS WR RD BL_EN使用的IO口初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;   //PA8/11/12/15推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);//拉高这些IO


    //LCD-RESET 复位脚使用到的IO口初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                    //PC13 推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //16个数据口定义
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;                   //PB推挽输出  用作并行数据口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
  //LCD复位
    LCD_RST=1;
    delay_ms(10);
    LCD_RST=0;
    delay_ms(20);
    LCD_RST=1;
    delay_ms(20);
    LCD_RD=1;//不读，拉高RD即可

    LCD_INIT_CODE();

    LCD_Display_Dir(Landscape);      //1:竖屏；0:横屏   横竖屏从这里切换
    LCD_Clear(WHITE);//刷白
    image_display((lcddev.width-80)/2,(lcddev.height-63)/2,(u8*)gImage_logo);//居中显示logo

}
//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
  u32 index=0;
    u32 totalpoint=lcddev.width;
    totalpoint*=lcddev.height;  //得到总点数
  LCD_Set_Window(0,0,lcddev.width,lcddev.height);
    LCD_WriteRAM_Prepare();     //开始写入GRAM

    LCD_RS=1;
    LCD_CS=0;

    for(index=0; index<totalpoint; index++)
    {
#if(FAST==0)
             LCD_WriteRAM(color);
#else
            //使用以下代码可以提速
#if(Interface==I80_16BIT)
      DATAOUT(color);
        LCD_WR=0;
        LCD_WR=1;
#elif(Interface==D4WSPI)
        SPI2_ReadWriteByte(color>>8);
        SPI2_ReadWriteByte(color);
#else
      DATAOUT(color>>8);
        LCD_WR=0;
        LCD_WR=1;
        DATAOUT(color);
        LCD_WR=0;
        LCD_WR=1;
#endif
#endif
    }
    LCD_CS=1;

}
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
    u16 i,j;
    u16 xlen=0;
    xlen=ex-sx+1;
    for(i=sy;i<=ey;i++)
    {
        LCD_SetCursor(sx,i);                    //设置光标位置
        LCD_WriteRAM_Prepare();                 //开始写入GRAM
        for(j=0;j<xlen;j++)LCD_WriteRAM(color);  //设置光标位置
    }
}
//在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
    u16 height,width;
    u16 i,j;
    width=ex-sx+1;      //得到填充的宽度
    height=ey-sy+1;     //高度
    for(i=0;i<height;i++)
    {
        LCD_SetCursor(sx,sy+i);     //设置光标位置
        LCD_WriteRAM_Prepare();     //开始写入GRAM
        for(j=0;j<width;j++)LCD_WriteRAM(color[i*height+j]);//写入数据
    }
}
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
//color:颜色
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
    u16 t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if(delta_x==0)incx=0;//垂直线
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;//水平线
    else{incy=-1;delta_y=-delta_y;}
    if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(t=0;t<=distance+1;t++ )//画线输出
    {
        LCD_DrawPoint(uRow,uCol,color);//画点
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
    LCD_DrawLine(x1,y1,x2,y1,color);
    LCD_DrawLine(x1,y1,x1,y2,color);
    LCD_DrawLine(x1,y2,x2,y2,color);
    LCD_DrawLine(x2,y1,x2,y2,color);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
    int a,b;
    int di;
    a=0;b=r;
    di=3-(r<<1);             //判断下个点位置的标志
    while(a<=b)
    {
        LCD_DrawPoint(x0+a,y0-b,color);             //5
        LCD_DrawPoint(x0+b,y0-a,color);             //0
        LCD_DrawPoint(x0+b,y0+a,color);             //4
        LCD_DrawPoint(x0+a,y0+b,color);             //6
        LCD_DrawPoint(x0-a,y0+b,color);             //1
        LCD_DrawPoint(x0-b,y0+a,color);
        LCD_DrawPoint(x0-a,y0-b,color);             //2
        LCD_DrawPoint(x0-b,y0-a,color);             //7
        a++;
        //使用Bresenham算法画圆
        if(di<0)di +=4*a+6;
        else
        {
            di+=10+4*(a-b);
            b--;
        }
    }
}
//在指定位置显示一个字符
//x,y:起始坐标
//chr:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
//color : 字符的颜色;
void LCD_ShowChar(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t temp, t1, t;
    uint16_t y0 = y;
    uint8_t csize = 0;
    uint8_t *pfont = 0;

    csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* 得到字体一个字符对应点阵集所占的字节数 */
    chr = chr - ' ';    /* 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库） */

    switch (size)
    {
        case 12:
            pfont = (uint8_t *)asc2_1206[chr];  /* 调用1206字体 */
            break;

        case 16:
            pfont = (uint8_t *)asc2_1608[chr];  /* 调用1608字体 */
            break;

        default:
            return ;
    }

    for (t = 0; t < csize; t++)
    {
        temp = pfont[t];    /* 获取字符的点阵数据 */

        for (t1 = 0; t1 < 8; t1++)   /* 一个字节8个点 */
        {
            if (temp & 0x80)        /* 有效点,需要显示 */
            {
                LCD_DrawPoint(x, y, color);        /* 画点出来,要显示这个点 */
            }
            else if (mode == 0)     /* 无效点,不显示 */
            {
                LCD_DrawPoint(x, y, BACK_COLOR); /* 画背景色,相当于这个点不显示(注意背景色由全局变量控制) */
            }

            temp <<= 1; /* 移位, 以便获取下一个位的状态 */
            y++;

            if (y >= lcddev.height)return;  /* 超区域了 */

            if ((y - y0) == size)   /* 显示完一列了? */
            {
                y = y0; /* y坐标复位 */
                x++;    /* x坐标递增 */

                if (x >= lcddev.width)return;   /* x坐标超区域了 */

                break;
            }
        }
    }
}
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}
//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,u16 color)
{
    u8 t,temp;
    u8 enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+(size/2)*t,y,' ',size,0,color);
                continue;
            }else enshow=1;

        }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0,color);
    }
}
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode,u16 color)
{
    u8 t,temp;
    u8 enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01,color);
                else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01,color);
                continue;
            }else enshow=1;

        }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01,color);
    }
}
//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 color)
{
    u8 x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0,color);
        x+=size/2;
        p++;
    }
}

//设置窗口
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{
    width=sx+width-1;
    height=sy+height-1;

    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(sx>>8);
    LCD_WR_DATA(sx&0XFF);
    LCD_WR_DATA(width>>8);
    LCD_WR_DATA(width&0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(sy>>8);
    LCD_WR_DATA(sy&0XFF);
    LCD_WR_DATA(height>>8);
    LCD_WR_DATA(height&0XFF);
}
#endif
