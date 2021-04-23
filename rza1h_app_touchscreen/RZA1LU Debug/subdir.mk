################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LINKER_SCRIPT += \
../linker_settings_rza1h.ld \
../linker_settings_rza1lu.ld 

C_SRCS += \
../main.c 

C_DEPS += \
./main.d 

OBJS += \
./main.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\arm\cmsis\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_dmac\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\middleware\sound\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\middleware\riic_max9856\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_l2cc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\application\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\application\console\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\application\system\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\application\system\iodefines" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\application\system\iobitmasks" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\compiler\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\configuration" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\configuration\os_abstraction\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\configuration\sc\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\adc\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\intc\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_switch\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\ostm\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_ceu\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_i2c\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_jcu\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_led\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_sci\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_rspi\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_ssif\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_rtc\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_vdc_vdec\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_wdt\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_eeprom\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\middleware\cmos\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\middleware\pmod_lcd\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\middleware\touch\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\middleware\timer\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\middleware\video\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\drivers\r_cache\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\threadx\common\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\azure\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rz_bsp\src\renesas\application\graphics\inc" -I"C:\workspace\RZA1\Azure\rza1_azure_gcc\rza1h_app_touchscreen\app_touchscreen\inc" -std=gnu11 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

