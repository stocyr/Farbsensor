################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include app/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_LIST += \
xynthLeds.lst \

SECONDARY_SIZE += \
xynthLeds.siz \


# All Target
all: xynthLeds secondary-outputs

# Tool invocations
xynthLeds: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linux GCC C Linker'
	arm-linux-gcc -L/opt/embedded/sitara/rootfs/usr/lib -L/opt/embedded/sitara/rootfs/usr/local/lib -Wl,-Map,xynthLeds.map -mcpu=cortex-a8 -g3 -gdwarf-2 -o "xynthLeds" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

xynthLeds.lst: xynthLeds
	@echo 'Invoking: ARM Linux GNU Create Listing'
	arm-linux-objdump -h -S xynthLeds > "xynthLeds.lst"
	@echo 'Finished building: $@'
	@echo ' '

xynthLeds.siz: xynthLeds
	@echo 'Invoking: ARM Linux GNU Print Size'
	arm-linux-size  --format=berkeley xynthLeds
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) xynthLeds
	-@echo ' '

post-build:
	-cp xynthLeds /opt/embedded/sitara/rootfs/usr/local/bin/xynthLeds
	-@echo ' '

secondary-outputs: $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
