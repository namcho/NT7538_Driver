################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -I"E:/workspace_ll/NT7538_Test/inc" -I"E:/workspace_ll/NT7538_Test/CMSIS/core" -I"E:/workspace_ll/NT7538_Test/CMSIS/device" -I"E:/workspace_ll/NT7538_Test/HAL_Driver/Inc/Legacy" -I"E:/workspace_ll/NT7538_Test/HAL_Driver/Inc" -I"E:/workspace_ll/NT7538_Test/LcdController/" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

