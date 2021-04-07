################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/helper_c.c \
../src/sample_usbx_rz_host_storage.c 

C_DEPS += \
./src/helper_c.d \
./src/sample_usbx_rz_host_storage.d 

OBJS += \
./src/helper_c.o \
./src/sample_usbx_rz_host_storage.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mthumb-interwork -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=1000 -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\filex\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\arm\cmsis\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\compiler\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration\os_abstraction\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration\sc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\graphics\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\console\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\iodefines" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\iobitmasks" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\adc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\intc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\ostm\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_cache\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\azure\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\core\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_host_classes\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_host_controllers" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\threadx\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\addons\dhcp" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/sample_usbx_host_mass_storage/src" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

