################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/snake.c \
../src/syscalls.c \
../src/system_stm32f0xx.c 

OBJS += \
./src/snake.o \
./src/syscalls.o \
./src/system_stm32f0xx.o 

C_DEPS += \
./src/snake.d \
./src/syscalls.d \
./src/system_stm32f0xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -DSTM32F0DISCOVERY -DDEBUG -DSTM32F051 -DUSE_STDPERIPH_DRIVER -I"D:/OneDrive - purdue.edu/school PU1_junior/ECE 36200/miniproj/miniproj_LTHW/Utilities" -I"D:/OneDrive - purdue.edu/school PU1_junior/ECE 36200/miniproj/miniproj_LTHW/StdPeriph_Driver/inc" -I"D:/OneDrive - purdue.edu/school PU1_junior/ECE 36200/miniproj/miniproj_LTHW/inc" -I"D:/OneDrive - purdue.edu/school PU1_junior/ECE 36200/miniproj/miniproj_LTHW/CMSIS/device" -I"D:/OneDrive - purdue.edu/school PU1_junior/ECE 36200/miniproj/miniproj_LTHW/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


