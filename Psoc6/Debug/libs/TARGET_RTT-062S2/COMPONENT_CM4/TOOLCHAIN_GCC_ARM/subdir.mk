################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../libs/TARGET_RTT-062S2/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm4.S 

OBJS += \
./libs/TARGET_RTT-062S2/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm4.o 

S_UPPER_DEPS += \
./libs/TARGET_RTT-062S2/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/startup_psoc6_02_cm4.d 


# Each subdirectory must supply rules for building sources it contributes
libs/TARGET_RTT-062S2/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/%.o: ../libs/TARGET_RTT-062S2/COMPONENT_CM4/TOOLCHAIN_GCC_ARM/%.S
	-@echo compiling $<...
	@arm-none-eabi-gcc -x assembler-with-cpp -Xassembler -mimplicit-it=thumb -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

