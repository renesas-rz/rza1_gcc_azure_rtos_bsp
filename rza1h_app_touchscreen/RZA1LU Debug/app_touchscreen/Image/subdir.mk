################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../app_touchscreen/Image/JCU_ExampleImage.S 

OBJS += \
./app_touchscreen/Image/JCU_ExampleImage.o 

S_UPPER_DEPS += \
./app_touchscreen/Image/JCU_ExampleImage.d 


# Each subdirectory must supply rules for building sources it contributes
app_touchscreen/Image/%.o: ../app_touchscreen/Image/%.S
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

