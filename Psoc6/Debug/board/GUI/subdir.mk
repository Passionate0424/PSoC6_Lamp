################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/GUI/custom.c \
../board/GUI/events_init.c \
../board/GUI/gui_guider.c \
../board/GUI/lv_analogclock.c \
../board/GUI/lv_dclock.c \
../board/GUI/lv_font_Alatsi_Regular_18.c \
../board/GUI/lv_font_Alatsi_Regular_35.c \
../board/GUI/lv_font_SourceHanSerifSC_Regular_11.c \
../board/GUI/lv_font_SourceHanSerifSC_Regular_12.c \
../board/GUI/lv_font_SourceHanSerifSC_Regular_13.c \
../board/GUI/lv_font_SourceHanSerifSC_Regular_14.c \
../board/GUI/lv_font_SourceHanSerifSC_Regular_15.c \
../board/GUI/lv_font_SourceHanSerifSC_Regular_16.c \
../board/GUI/lv_font_SourceHanSerifSC_Regular_18.c \
../board/GUI/lv_font_iconfont_14.c \
../board/GUI/lv_font_iconfont_15.c \
../board/GUI/lv_font_iconfont_16.c \
../board/GUI/lv_font_iconfont_18.c \
../board/GUI/lv_font_iconfont_20.c \
../board/GUI/lv_font_iconfont_25.c \
../board/GUI/lv_font_iconfont_40.c \
../board/GUI/lv_font_montserratMedium_12.c \
../board/GUI/lv_font_montserratMedium_14.c \
../board/GUI/lv_font_montserratMedium_15.c \
../board/GUI/lv_font_montserratMedium_16.c \
../board/GUI/lv_font_montserratMedium_18.c \
../board/GUI/setup_scr_font.c \
../board/GUI/setup_scr_guader.c \
../board/GUI/setup_scr_home.c \
../board/GUI/setup_scr_monitor.c \
../board/GUI/setup_scr_settings.c \
../board/GUI/widgets_init.c 

OBJS += \
./board/GUI/custom.o \
./board/GUI/events_init.o \
./board/GUI/gui_guider.o \
./board/GUI/lv_analogclock.o \
./board/GUI/lv_dclock.o \
./board/GUI/lv_font_Alatsi_Regular_18.o \
./board/GUI/lv_font_Alatsi_Regular_35.o \
./board/GUI/lv_font_SourceHanSerifSC_Regular_11.o \
./board/GUI/lv_font_SourceHanSerifSC_Regular_12.o \
./board/GUI/lv_font_SourceHanSerifSC_Regular_13.o \
./board/GUI/lv_font_SourceHanSerifSC_Regular_14.o \
./board/GUI/lv_font_SourceHanSerifSC_Regular_15.o \
./board/GUI/lv_font_SourceHanSerifSC_Regular_16.o \
./board/GUI/lv_font_SourceHanSerifSC_Regular_18.o \
./board/GUI/lv_font_iconfont_14.o \
./board/GUI/lv_font_iconfont_15.o \
./board/GUI/lv_font_iconfont_16.o \
./board/GUI/lv_font_iconfont_18.o \
./board/GUI/lv_font_iconfont_20.o \
./board/GUI/lv_font_iconfont_25.o \
./board/GUI/lv_font_iconfont_40.o \
./board/GUI/lv_font_montserratMedium_12.o \
./board/GUI/lv_font_montserratMedium_14.o \
./board/GUI/lv_font_montserratMedium_15.o \
./board/GUI/lv_font_montserratMedium_16.o \
./board/GUI/lv_font_montserratMedium_18.o \
./board/GUI/setup_scr_font.o \
./board/GUI/setup_scr_guader.o \
./board/GUI/setup_scr_home.o \
./board/GUI/setup_scr_monitor.o \
./board/GUI/setup_scr_settings.o \
./board/GUI/widgets_init.o 

C_DEPS += \
./board/GUI/custom.d \
./board/GUI/events_init.d \
./board/GUI/gui_guider.d \
./board/GUI/lv_analogclock.d \
./board/GUI/lv_dclock.d \
./board/GUI/lv_font_Alatsi_Regular_18.d \
./board/GUI/lv_font_Alatsi_Regular_35.d \
./board/GUI/lv_font_SourceHanSerifSC_Regular_11.d \
./board/GUI/lv_font_SourceHanSerifSC_Regular_12.d \
./board/GUI/lv_font_SourceHanSerifSC_Regular_13.d \
./board/GUI/lv_font_SourceHanSerifSC_Regular_14.d \
./board/GUI/lv_font_SourceHanSerifSC_Regular_15.d \
./board/GUI/lv_font_SourceHanSerifSC_Regular_16.d \
./board/GUI/lv_font_SourceHanSerifSC_Regular_18.d \
./board/GUI/lv_font_iconfont_14.d \
./board/GUI/lv_font_iconfont_15.d \
./board/GUI/lv_font_iconfont_16.d \
./board/GUI/lv_font_iconfont_18.d \
./board/GUI/lv_font_iconfont_20.d \
./board/GUI/lv_font_iconfont_25.d \
./board/GUI/lv_font_iconfont_40.d \
./board/GUI/lv_font_montserratMedium_12.d \
./board/GUI/lv_font_montserratMedium_14.d \
./board/GUI/lv_font_montserratMedium_15.d \
./board/GUI/lv_font_montserratMedium_16.d \
./board/GUI/lv_font_montserratMedium_18.d \
./board/GUI/setup_scr_font.d \
./board/GUI/setup_scr_guader.d \
./board/GUI/setup_scr_home.d \
./board/GUI/setup_scr_monitor.d \
./board/GUI/setup_scr_settings.d \
./board/GUI/widgets_init.d 


# Each subdirectory must supply rules for building sources it contributes
board/GUI/%.o: ../board/GUI/%.c
	-@echo compiling $<...
	@arm-none-eabi-gcc -I"../rt-thread/components/legacy/dfs" -I"../packages/llmchat-latest/demo" -I"../applications/Hardware/ENS160" -I"../board/GUI" -I"../board/GUI" -I"../board/GUI" -I".." -I"../applications/Hardware" -I"../applications/arduino_pinout" -I"../applications/lvgl/demo" -I"../applications/lvgl" -I"../applications/model_int" -I"../applications" -I"../board/GUI" -I"../board/ports" -I"../board" -I"../libraries/HAL_Drivers" -I"../libraries/IFX_PSOC6_HAL/capsense" -I"../libraries/IFX_PSOC6_HAL/core-lib/include" -I"../libraries/IFX_PSOC6_HAL/mtb-hal-cat1/COMPONENT_CAT1A/include" -I"../libraries/IFX_PSOC6_HAL/mtb-hal-cat1/include_pvt" -I"../libraries/IFX_PSOC6_HAL/mtb-hal-cat1/include" -I"../libraries/IFX_PSOC6_HAL/mtb-pdl-cat1/cmsis/include" -I"../libraries/IFX_PSOC6_HAL/mtb-pdl-cat1/devices/COMPONENT_CAT1A/include" -I"../libraries/IFX_PSOC6_HAL/mtb-pdl-cat1/drivers/include" -I"../libraries/IFX_PSOC6_HAL/mtb_shared/csdidac" -I"../libraries/IFX_PSOC6_HAL/mtb_shared/serial-flash" -I"../libraries/IFX_PSOC6_HAL/mtb_shared/usbdev" -I"../libraries/IFX_PSOC6_HAL/psoc6cm0p" -I"../libraries/IFX_PSOC6_HAL/retarget-io" -I"../libs/TARGET_RTT-062S2/config/GeneratedSource" -I"../libs/TARGET_RTT-062S2" -I"../packages/Adafruit-BMP280-latest" -I"../packages/Adafruit-BusIO-latest" -I"../packages/Adafruit-SHT31-latest" -I"../packages/Adafruit-Sensor-latest" -I"../packages/LVGL-v8.3.10/env_support/rt-thread" -I"../packages/LVGL-v8.3.10/src/core" -I"../packages/LVGL-v8.3.10/src/draw/arm2d" -I"../packages/LVGL-v8.3.10/src/draw/nxp/pxp" -I"../packages/LVGL-v8.3.10/src/draw/nxp/vglite" -I"../packages/LVGL-v8.3.10/src/draw/renesas" -I"../packages/LVGL-v8.3.10/src/draw/sdl" -I"../packages/LVGL-v8.3.10/src/draw/stm32_dma2d" -I"../packages/LVGL-v8.3.10/src/draw/swm341_dma2d" -I"../packages/LVGL-v8.3.10/src/draw/sw" -I"../packages/LVGL-v8.3.10/src/draw" -I"../packages/LVGL-v8.3.10/src/extra/layouts/flex" -I"../packages/LVGL-v8.3.10/src/extra/layouts/grid" -I"../packages/LVGL-v8.3.10/src/extra/layouts" -I"../packages/LVGL-v8.3.10/src/extra/libs/bmp" -I"../packages/LVGL-v8.3.10/src/extra/libs/ffmpeg" -I"../packages/LVGL-v8.3.10/src/extra/libs/freetype" -I"../packages/LVGL-v8.3.10/src/extra/libs/fsdrv" -I"../packages/LVGL-v8.3.10/src/extra/libs/gif" -I"../packages/LVGL-v8.3.10/src/extra/libs/png" -I"../packages/LVGL-v8.3.10/src/extra/libs/qrcode" -I"../packages/LVGL-v8.3.10/src/extra/libs/rlottie" -I"../packages/LVGL-v8.3.10/src/extra/libs/sjpg" -I"../packages/LVGL-v8.3.10/src/extra/libs" -I"../packages/LVGL-v8.3.10/src/extra/others/fragment" -I"../packages/LVGL-v8.3.10/src/extra/others/gridnav" -I"../packages/LVGL-v8.3.10/src/extra/others/ime" -I"../packages/LVGL-v8.3.10/src/extra/others/imgfont" -I"../packages/LVGL-v8.3.10/src/extra/others/monkey" -I"../packages/LVGL-v8.3.10/src/extra/others/msg" -I"../packages/LVGL-v8.3.10/src/extra/others/snapshot" -I"../packages/LVGL-v8.3.10/src/extra/others" -I"../packages/LVGL-v8.3.10/src/extra/themes/basic" -I"../packages/LVGL-v8.3.10/src/extra/themes/default" -I"../packages/LVGL-v8.3.10/src/extra/themes/mono" -I"../packages/LVGL-v8.3.10/src/extra/themes" -I"../packages/LVGL-v8.3.10/src/extra/widgets/animimg" -I"../packages/LVGL-v8.3.10/src/extra/widgets/calendar" -I"../packages/LVGL-v8.3.10/src/extra/widgets/chart" -I"../packages/LVGL-v8.3.10/src/extra/widgets/colorwheel" -I"../packages/LVGL-v8.3.10/src/extra/widgets/imgbtn" -I"../packages/LVGL-v8.3.10/src/extra/widgets/keyboard" -I"../packages/LVGL-v8.3.10/src/extra/widgets/led" -I"../packages/LVGL-v8.3.10/src/extra/widgets/list" -I"../packages/LVGL-v8.3.10/src/extra/widgets/menu" -I"../packages/LVGL-v8.3.10/src/extra/widgets/meter" -I"../packages/LVGL-v8.3.10/src/extra/widgets/msgbox" -I"../packages/LVGL-v8.3.10/src/extra/widgets/span" -I"../packages/LVGL-v8.3.10/src/extra/widgets/spinbox" -I"../packages/LVGL-v8.3.10/src/extra/widgets/spinner" -I"../packages/LVGL-v8.3.10/src/extra/widgets/tabview" -I"../packages/LVGL-v8.3.10/src/extra/widgets/tileview" -I"../packages/LVGL-v8.3.10/src/extra/widgets/win" -I"../packages/LVGL-v8.3.10/src/extra/widgets" -I"../packages/LVGL-v8.3.10/src/extra" -I"../packages/LVGL-v8.3.10/src/font" -I"../packages/LVGL-v8.3.10/src/hal" -I"../packages/LVGL-v8.3.10/src/misc" -I"../packages/LVGL-v8.3.10/src/widgets" -I"../packages/LVGL-v8.3.10/src" -I"../packages/RTduino-latest/core" -I"../packages/RTduino-latest/libraries/buildin/SPI" -I"../packages/RTduino-latest/libraries/buildin/Servo" -I"../packages/RTduino-latest/libraries/buildin/Wire" -I"../packages/RTduino-latest/libraries/user" -I"../packages/Servo_sg90-v1.0.0" -I"../packages/bh1750-latest" -I"../packages/cJSON-v1.7.17" -I"../packages/gt911-latest/inc" -I"../packages/i2c-tools-latest/src" -I"../packages/i2c-tools-latest" -I"../packages/llmchat-latest/ports" -I"../packages/llmchat-latest" -I"../packages/mbedtls-v2.28.1/mbedtls/include" -I"../packages/mbedtls-v2.28.1/mbedtls/library" -I"../packages/mbedtls-v2.28.1/ports/inc" -I"../packages/netutils-latest/ntp" -I"../packages/nnom-v0.4.3/inc" -I"../packages/nnom-v0.4.3/port" -I"../packages/onnx-backend-latest/examples" -I"../packages/onnx-backend-latest/src" -I"../packages/onnx-parser-latest/examples" -I"../packages/onnx-parser-latest/src" -I"../packages/pahomqtt-latest/MQTTClient-RT" -I"../packages/pahomqtt-latest/MQTTPacket/src" -I"../packages/perf_counter-v2.2.4.1" -I"../packages/protobuf-c-latest/examples" -I"../packages/protobuf-c-latest/src" -I"../packages/rw007-v2.1.0/inc" -I"../packages/rw007-v2.1.0" -I"../packages/smtp_client-latest/inc" -I"../packages/webclient-v2.2.0/inc" -I"../rt-thread/components/dfs/dfs_v1/filesystems/devfs" -I"../rt-thread/components/dfs/dfs_v1/filesystems/elmfat" -I"../rt-thread/components/dfs/dfs_v1/include" -I"../rt-thread/components/drivers/include" -I"../rt-thread/components/drivers/sensor/v1" -I"../rt-thread/components/drivers/spi" -I"../rt-thread/components/drivers/touch" -I"../rt-thread/components/drivers/wlan" -I"../rt-thread/components/finsh" -I"../rt-thread/components/libc/compilers/common/include" -I"../rt-thread/components/libc/compilers/newlib" -I"../rt-thread/components/libc/cplusplus" -I"../rt-thread/components/libc/posix/io/poll" -I"../rt-thread/components/libc/posix/io/stdio" -I"../rt-thread/components/libc/posix/ipc" -I"../rt-thread/components/net/lwip/lwip-2.0.3/src/include/netif" -I"../rt-thread/components/net/lwip/lwip-2.0.3/src/include" -I"../rt-thread/components/net/lwip/port" -I"../rt-thread/components/net/netdev/include" -I"../rt-thread/components/net/sal/impl" -I"../rt-thread/components/net/sal/include/dfs_net" -I"../rt-thread/components/net/sal/include/socket/sys_socket" -I"../rt-thread/components/net/sal/include/socket" -I"../rt-thread/components/net/sal/include" -I"../rt-thread/components/utilities/resource" -I"../rt-thread/include" -I"../rt-thread/libcpu/arm/common" -I"../rt-thread/libcpu/arm/cortex-m4" -include"../rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

