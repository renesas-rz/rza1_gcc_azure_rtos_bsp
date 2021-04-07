################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/smtp/nxd_smtp_client.c 

C_DEPS += \
./addons/smtp/nxd_smtp_client.d 

OBJS += \
./addons/smtp/nxd_smtp_client.o 


# Each subdirectory must supply rules for building sources it contributes
addons/smtp/%.o: ../addons/smtp/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=2000 -I"C:\Users\wangwen\e2_studio\workspace\filex\ports\cortex_a9\gnu\inc" -I"C:\Users\wangwen\e2_studio\workspace\filex\common\inc" -I"C:\Users\wangwen\e2_studio\workspace\threadx\common\inc" -I"C:\Users\wangwen\e2_studio\workspace\threadx\ports\cortex_a9\gnu\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\ports\cortex_a9\gnu\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\common\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\crypto_libraries\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\nx_secure\ports" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\nx_secure\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure-sdk-for-c\sdk\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\iot-security-module-core\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\inc" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\iot-security-module-core\deps\flatcc\src\runtime" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\azure_iot\azure_iot_security_module\iot-security-module-core\deps\flatcc\include" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\cloud" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\mqtt" -I"C:\Users\wangwen\e2_studio\workspace\netxduo\addons\dns" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

