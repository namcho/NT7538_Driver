################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/LCDInterface.c \
../src/main.c \
../src/stm32f3xx_it.c \
../src/syscalls.c \
../src/system_stm32f3xx.c 

OBJS += \
./src/LCDInterface.o \
./src/main.o \
./src/stm32f3xx_it.o \
./src/syscalls.o \
./src/system_stm32f3xx.o 

C_DEPS += \
./src/LCDInterface.d \
./src/main.d \
./src/stm32f3xx_it.d \
./src/syscalls.d \
./src/system_stm32f3xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -DSTM32F30 -DSTM32F3 -DSTM32F303VCTx -DSTM32 -DUSE_HAL_DRIVER -DSTM32F303xC -I"E:/workspace_ll/NT7538_Driver/inc" -I"E:/workspace_ll/NT7538_Driver/CMSIS/core" -I"E:/workspace_ll/NT7538_Driver/CMSIS/device" -I"E:/workspace_ll/NT7538_Driver/HAL_Driver/Inc/Legacy" -I"E:/workspace_ll/NT7538_Driver/HAL_Driver/Inc" -I"E:/workspace_ll/NT7538_Driver/LcdController/" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


