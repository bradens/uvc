################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../diskinfo/diskinfo.c \
../diskinfo/helpers.c 

OBJS += \
./diskinfo/diskinfo.o \
./diskinfo/helpers.o 

C_DEPS += \
./diskinfo/diskinfo.d \
./diskinfo/helpers.d 


# Each subdirectory must supply rules for building sources it contributes
diskinfo/%.o: ../diskinfo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


