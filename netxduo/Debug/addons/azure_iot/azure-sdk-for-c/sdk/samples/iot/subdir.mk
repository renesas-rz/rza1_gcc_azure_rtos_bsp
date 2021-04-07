################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/iot_sample_common.c \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_c2d_sample.c \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_methods_sample.c \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_pnp_component_sample.c \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_pnp_sample.c \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_sas_telemetry_sample.c \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_telemetry_sample.c \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_twin_sample.c \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_provisioning_sample.c \
../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_provisioning_sas_sample.c 

C_DEPS += \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/iot_sample_common.d \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_c2d_sample.d \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_methods_sample.d \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_pnp_component_sample.d \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_pnp_sample.d \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_sas_telemetry_sample.d \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_telemetry_sample.d \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_twin_sample.d \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_provisioning_sample.d \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_provisioning_sas_sample.d 

OBJS += \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/iot_sample_common.o \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_c2d_sample.o \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_methods_sample.o \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_pnp_component_sample.o \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_pnp_sample.o \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_sas_telemetry_sample.o \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_telemetry_sample.o \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_hub_twin_sample.o \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_provisioning_sample.o \
./addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/paho_iot_provisioning_sas_sample.o 


# Each subdirectory must supply rules for building sources it contributes
addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/%.o: ../addons/azure_iot/azure-sdk-for-c/sdk/samples/iot/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -fdiagnostics-parseable-fixits -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=200 -I"C:/Users/wangwen/e2_studio/workspace/netxduo/src" -I"C:\Users\wangwen\e2_studio\workspace\filex\ports\cortex_a9\gnu\inc" -I"C:\Users\wangwen\e2_studio\workspace\filex\common\inc" -I"C:\Users\wangwen\e2_studio\workspace\threadx\common\inc" -I"C:\Users\wangwen\e2_studio\workspace\threadx\ports\cortex_a9\gnu\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\ports\cortex_a9\gnu\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\common\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\crypto_libraries\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\nx_secure\ports" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\nx_secure\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure-sdk-for-c\sdk\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\iot-security-module-core\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\iot-security-module-core\deps\flatcc\src\runtime" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\iot-security-module-core\deps\flatcc\include" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\cloud" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\mqtt" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\dns" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

