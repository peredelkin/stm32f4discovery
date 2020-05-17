################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/crc16_ccitt.c \
../Source/gpio.c \
../Source/main.c \
../Source/rcc.c \
../Source/startup_stm32f407vgt6.c \
../Source/system_stm32f4xx.c \
../Source/uart.c 

OBJS += \
./Source/crc16_ccitt.o \
./Source/gpio.o \
./Source/main.o \
./Source/rcc.o \
./Source/startup_stm32f407vgt6.o \
./Source/system_stm32f4xx.o \
./Source/uart.o 

C_DEPS += \
./Source/crc16_ccitt.d \
./Source/gpio.d \
./Source/main.d \
./Source/rcc.d \
./Source/startup_stm32f407vgt6.d \
./Source/system_stm32f4xx.d \
./Source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F40_41xxx -I"/home/ruslan/eclipse-workspace/stm32f4discovery/Inclide" -I"/home/ruslan/eclipse-workspace/stm32f4discovery/FreeRTOS/Source/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


