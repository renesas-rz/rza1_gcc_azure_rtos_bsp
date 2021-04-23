################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/event_loop_be.c \
../addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/notifier.c \
../addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/string_utils.c 

C_DEPS += \
./addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/event_loop_be.d \
./addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/notifier.d \
./addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/string_utils.d 

OBJS += \
./addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/event_loop_be.o \
./addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/notifier.o \
./addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/string_utils.o 


# Each subdirectory must supply rules for building sources it contributes
addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/%.o: ../addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=2000 -I../../filex/ports/cortex_a9/gnu/inc -I../../filex/common/inc -I../../threadx/common/inc -I../../rz_bsp/azure/ports/cortex_a9/gnu/inc -I../../netxduo/common/inc -I../../netxduo/crypto_libraries/inc -I../../netxduo/nx_secure/ports -I../../netxduo/nx_secure/inc -I../../netxduo/addons/azure_iot -I../../netxduo/addons/azure_iot/azure-sdk-for-c/sdk/inc -I../../netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/inc -I../../netxduo/addons/azure_iot/azure_iot_security_module/inc -I../../netxduo/addons/azure_iot/azure_iot_security_module -I../../netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/src/runtime -I../../netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/include -I../../netxduo/addons/cloud -I../../netxduo/addons/mqtt -I../../netxduo/addons/dns -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

