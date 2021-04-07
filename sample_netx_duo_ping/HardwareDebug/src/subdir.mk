################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/nx_driver_renesas_rz.c \
../src/sample_netx_duo_ping.c 

C_DEPS += \
./src/nx_driver_renesas_rz.d \
./src/sample_netx_duo_ping.d 

OBJS += \
./src/nx_driver_renesas_rz.o \
./src/sample_netx_duo_ping.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\arm\cmsis\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\compiler\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration\os_abstraction\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration\sc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\graphics\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\console\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\iodefines" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\iobitmasks" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\adc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\intc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\ostm\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_cache\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\middleware\ethernet\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\azure\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\threadx\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\addons\dhcp" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/sample_netx_duo_ping/src" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

