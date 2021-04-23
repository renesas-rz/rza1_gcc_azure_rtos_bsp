################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_context.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_pipeline.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy_logging.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy_retry.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_request.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_response.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_reader.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_token.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_writer.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_log.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_precondition.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_span.c 

C_DEPS += \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_context.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_pipeline.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy_logging.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy_retry.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_request.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_response.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_reader.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_token.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_writer.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_log.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_precondition.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_span.d 

OBJS += \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_context.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_pipeline.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy_logging.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_policy_retry.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_request.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_http_response.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_reader.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_token.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_writer.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_log.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_precondition.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_span.o 


# Each subdirectory must supply rules for building sources it contributes
addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/%.o: ../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=2000 -I../../filex/ports/cortex_a9/gnu/inc -I../../filex/common/inc -I../../threadx/common/inc -I../../rz_bsp/azure/ports/cortex_a9/gnu/inc -I../../netxduo/common/inc -I../../netxduo/crypto_libraries/inc -I../../netxduo/nx_secure/ports -I../../netxduo/nx_secure/inc -I../../netxduo/addons/azure_iot -I../../netxduo/addons/azure_iot/azure-sdk-for-c/sdk/inc -I../../netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/inc -I../../netxduo/addons/azure_iot/azure_iot_security_module/inc -I../../netxduo/addons/azure_iot/azure_iot_security_module -I../../netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/src/runtime -I../../netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/include -I../../netxduo/addons/cloud -I../../netxduo/addons/mqtt -I../../netxduo/addons/dns -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

