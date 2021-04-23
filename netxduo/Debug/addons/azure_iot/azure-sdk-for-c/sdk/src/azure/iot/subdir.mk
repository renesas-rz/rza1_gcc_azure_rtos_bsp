################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_common.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_c2d.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_methods.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_sas.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_telemetry.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_twin.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_provisioning_client.c \
../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_provisioning_client_sas.c 

C_DEPS += \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_common.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_c2d.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_methods.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_sas.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_telemetry.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_twin.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_provisioning_client.d \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_provisioning_client_sas.d 

OBJS += \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_common.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_c2d.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_methods.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_sas.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_telemetry.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_hub_client_twin.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_provisioning_client.o \
./addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/az_iot_provisioning_client_sas.o 


# Each subdirectory must supply rules for building sources it contributes
addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/%.o: ../addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=2000 -I../../filex/ports/cortex_a9/gnu/inc -I../../filex/common/inc -I../../threadx/common/inc -I../../rz_bsp/azure/ports/cortex_a9/gnu/inc -I../../netxduo/common/inc -I../../netxduo/crypto_libraries/inc -I../../netxduo/nx_secure/ports -I../../netxduo/nx_secure/inc -I../../netxduo/addons/azure_iot -I../../netxduo/addons/azure_iot/azure-sdk-for-c/sdk/inc -I../../netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/inc -I../../netxduo/addons/azure_iot/azure_iot_security_module/inc -I../../netxduo/addons/azure_iot/azure_iot_security_module -I../../netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/src/runtime -I../../netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/include -I../../netxduo/addons/cloud -I../../netxduo/addons/mqtt -I../../netxduo/addons/dns -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

