################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Source/subdir.mk
-include FreeRTOS/Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
stm32f4discovery.hex \

SECONDARY_SIZE += \
stm32f4discovery.siz \


# All Target
all: stm32f4discovery.elf secondary-outputs

# Tool invocations
stm32f4discovery.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T "/home/ruslan/eclipse-workspace/stm32f4discovery/STM32F407VGT6.ld" -Xlinker --gc-sections -L"/home/ruslan/eclipse-workspace/stm32f4discovery" -Wl,-Map,"stm32f4discovery.map" -o "stm32f4discovery.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

stm32f4discovery.hex: stm32f4discovery.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "stm32f4discovery.elf"  "stm32f4discovery.hex"
	@echo 'Finished building: $@'
	@echo ' '

stm32f4discovery.siz: stm32f4discovery.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "stm32f4discovery.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) stm32f4discovery.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
