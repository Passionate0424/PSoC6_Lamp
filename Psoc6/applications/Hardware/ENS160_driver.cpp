#include <Wire.h>
#include "ENS160.h"                        // ENS160 library
ScioSense_ENS160 ens160(ENS160_I2CADDR_0); // 0x52
// ScioSense_ENS160      ens160(ENS160_I2CADDR_1);

#include "RTduino.h"
#include "Config.h"

// C 语言头文件，包含 RT-Thread 相关接口和 LCD 显示接口
extern "C"
{
#include <rtthread.h>
#include "lcd_rtthread.h"
#include "D:\RT-ThreadStudio\workspace\Psoc6_spi\board\GUI\gui_guider.h"
}

extern float T_H_Data[2];
int tvoc = 0;
int eco2 = 0;
char ens_display_buffer[15];

/*--------------------------------------------------------------------------
  SETUP function
  initiate sensor
 --------------------------------------------------------------------------*/
void ENS_setup()
{

    Serial.begin();

    while (!Serial)
    {
    }
    // Serial.println("------------------------------------------------------------");
    // Serial.println("ENS160 - Digital air quality sensor");
    // Serial.println();
    // Serial.println("Sensor readout in standard mode");
    // Serial.println();
    // Serial.println("------------------------------------------------------------");
    delay(1000);

    Serial.print("ENS160...");
    ens160.begin();
    Serial.println(ens160.available() ? "done." : "failed!");
    if (ens160.available())
    {
        // Print ENS160 versions
        Serial.print("\tRev: ");
        Serial.print(ens160.getMajorRev());
        Serial.print(".");
        Serial.print(ens160.getMinorRev());
        Serial.print(".");
        Serial.println(ens160.getBuild());

        Serial.print("\tStandard mode ");
        Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
    }
}

/*--------------------------------------------------------------------------
  MAIN LOOP FUNCTION
  Cylce every 1000ms and perform measurement
 --------------------------------------------------------------------------*/

void ENS_loop()
{

    if (ens160.available())
    {
        ens160.measure(true);
        ens160.measureRaw(true);
        tvoc = ens160.getTVOC();
        eco2 = ens160.geteCO2();

        // Serial.print("AQI: ");
        // Serial.print(ens160.getAQI());
        // Serial.print("\t");
        // Serial.print("TVOC: ");
        // Serial.print(ens160.getTVOC());
        // Serial.print("ppb\t");
        // Serial.print("eCO2: ");
        // Serial.print(ens160.geteCO2());
        // Serial.print("ppm\t");
        // Serial.print("R HP0: ");
        // Serial.print(ens160.getHP0());
        // Serial.print("Ohm\t");
        // Serial.print("R HP1: ");
        // Serial.print(ens160.getHP1());
        // Serial.print("Ohm\t");
        // Serial.print("R HP2: ");
        // Serial.print(ens160.getHP2());
        // Serial.print("Ohm\t");
        // Serial.print("R HP3: ");
        // Serial.print(ens160.getHP3());
        // Serial.println("Ohm");
    }
    // lvgl 显示数据
    rt_snprintf(ens_display_buffer, sizeof(ens_display_buffer), "%dppm", eco2);
    if (guider_ui.monitor_label_co2_now != RT_NULL)
    {
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
        lv_label_set_text(guider_ui.monitor_label_co2_now, ens_display_buffer);
        rt_mutex_release(lv_mutex); // 释放互斥锁
    }

    rt_snprintf(ens_display_buffer, sizeof(ens_display_buffer), "%dppb", tvoc);
    if (guider_ui.monitor_label_tvoc_now != RT_NULL)
    {
        rt_mutex_take(lv_mutex, RT_WAITING_FOREVER); // 确保线程安全
        lv_label_set_text(guider_ui.monitor_label_tvoc_now, ens_display_buffer);
        rt_mutex_release(lv_mutex); // 释放互斥锁
    }
    delay(1000);
}

// 初始化RTduino线程，注册 BMP_setup/BMP_loop
static int rtduino_init(void)
{
    rtduino_sketch_loader_create("RT_ENS", ENS_setup, ENS_loop);
    return 0;
}
INIT_APP_EXPORT(rtduino_init);
