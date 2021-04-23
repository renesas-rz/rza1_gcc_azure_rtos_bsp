################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/main.c \
../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_context.c \
../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_http.c \
../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_json.c \
../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_logging.c \
../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_pipeline.c \
../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_policy.c \
../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_span.c \
../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_url_encode.c 

C_DEPS += \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/main.d \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_context.d \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_http.d \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_json.d \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_logging.d \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_pipeline.d \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_policy.d \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_span.d \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_url_encode.d 

OBJS += \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/main.o \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_context.o \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_http.o \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_json.o \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_logging.o \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_pipeline.o \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_policy.o \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_span.o \
./addons/azure_iot/azure-sdk-for-c/sdk/tests/core/test_az_url_encode.o 


# Each subdirectory must supply rules for building sources it contributes
addons/azure_iot/azure-sdk-for-c/sdk/tests/core/%.o: ../addons/azure_iot/azure-sdk-for-c/sdk/tests/core/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -fdiagnostics-parseable-fixits -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=200 -I"C:/Users/wangwen/e2_studio/workspace/netxduo/src" -I"C:\Users\wangwen\e2_studio\workspace\filex\ports\cortex_a9\gnu\inc" -I"C:\Users\wangwen\e2_studio\workspace\filex\common\inc" -I"C:\Users\wangwen\e2_studio\workspace\threadx\common\inc" -I"C:\Users\wangwen\e2_studio\workspace\threadx\ports\cortex_a9\gnu\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\ports\cortex_a9\gnu\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\common\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\crypto_libraries\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\nx_secure\ports" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\nx_secure\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure-sdk-for-c\sdk\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\iot-security-module-core\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\iot-security-module-core\deps\flatcc\src\runtime" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\iot-security-module-core\deps\flatcc\include" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\cloud" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\mqtt" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\dns" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

