################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LcdController/NT7538_Driver.c 

OBJS += \
./LcdController/NT7538_Driver.o 

C_DEPS += \
./LcdController/NT7538_Driver.d 


# Each subdirectory must supply rules for building sources it contributes
LcdController/%.o: ../LcdController/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -DSTM32F30 -DSTM32F3 -DSTM32F303VCTx -DSTM32 -DUSE_HAL_DRIVER -DSTM32F303xC -I"E:/workspace_ll/NT7538_Test/inc" -I"E:/workspace_ll/NT7538_Test/CMSIS/core" -I"E:/workspace_ll/NT7538_Test/CMSIS/device" -I"E:/workspace_ll/NT7538_Test/HAL_Driver/Inc/Legacy" -I"E:/workspace_ll/NT7538_Test/HAL_Driver/Inc" -I"E:/workspace_ll/NT7538_Test/LcdController/" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


