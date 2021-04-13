################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/nx_azure_iot_cert.c \
../src/nx_azure_iot_ciphersuites.c \
../src/nx_azure_iot_pnp_helpers.c \
../src/nx_driver_renesas_rz.c \
../src/sample_device_identity.c \
../src/sample_pnp_deviceinfo_component.c \
../src/sample_pnp_temperature_controller.c \
../src/sample_pnp_thermostat_component.c 

C_DEPS += \
./src/main.d \
./src/nx_azure_iot_cert.d \
./src/nx_azure_iot_ciphersuites.d \
./src/nx_azure_iot_pnp_helpers.d \
./src/nx_driver_renesas_rz.d \
./src/sample_device_identity.d \
./src/sample_pnp_deviceinfo_component.d \
./src/sample_pnp_temperature_controller.d \
./src/sample_pnp_thermostat_component.d 

OBJS += \
./src/main.o \
./src/nx_azure_iot_cert.o \
./src/nx_azure_iot_ciphersuites.o \
./src/nx_azure_iot_pnp_helpers.o \
./src/nx_driver_renesas_rz.o \
./src/sample_device_identity.o \
./src/sample_pnp_deviceinfo_component.o \
./src/sample_pnp_temperature_controller.o \
./src/sample_pnp_thermostat_component.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=1000 -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\compiler\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\configuration" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\configuration\os_abstraction\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\configuration\sc\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\graphics\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\console\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\system\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\system\iodefines" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\system\iobitmasks" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\adc\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\intc\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\ostm\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_cache\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\middleware\ethernet\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\azure\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\addons\sntp" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\crypto_libraries\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\nx_secure\ports" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\nx_secure\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\addons\dns" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\addons\mqtt" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\addons\cloud" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\addons\azure_iot" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\addons\azure_iot\azure-sdk-for-c\sdk\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\addons\http" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\filex\common\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\threadx\common\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\common\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\addons\dhcp" -I"C:/workspace/RZA1/Azure/rza1h_azure_gcc/sample_pnp_temperature_controller/src" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

