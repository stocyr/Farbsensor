################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/appXynthDemo.c 

OBJS += \
./app/appXynthDemo.o 

C_DEPS += \
./app/appXynthDemo.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o: ../app/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux GCC C Compiler'
	arm-linux-gcc -I/home/student/workspace/xynthLeds/include -I/opt/embedded/sitara/rootfs/usr/include -I/opt/embedded/sitara/rootfs/usr/local/include -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-a8 -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


