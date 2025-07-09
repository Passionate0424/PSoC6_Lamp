################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../libraries/IFX_PSOC6_HAL/mtb-pdl-cat1/drivers/source/TOOLCHAIN_GCC_ARM/cy_syslib_ext.S 

OBJS += \
./libraries/IFX_PSOC6_HAL/mtb-pdl-cat1/drivers/source/TOOLCHAIN_GCC_ARM/cy_syslib_ext.o 

S_UPPER_DEPS += \
./libraries/IFX_PSOC6_HAL/mtb-pdl-cat1/drivers/source/TOOLCHAIN_GCC_ARM/cy_syslib_ext.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/IFX_PSOC6_HAL/mtb-pdl-cat1/drivers/source/TOOLCHAIN_GCC_ARM/%.o: ../libraries/IFX_PSOC6_HAL/mtb-pdl-cat1/drivers/source/TOOLCHAIN_GCC_ARM/%.S
	-@echo compiling $<...
	@arm-none-eabi-gcc -x assembler-with-cpp -Xassembler -mimplicit-it=thumb -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

