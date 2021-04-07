################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rz_display_driver_565rgb.c \
../rz_rsk_display_driver_24xrgb.c \
../rz_rsk_display_driver_32argb.c \
../rz_rsk_touch.c 

C_DEPS += \
./rz_display_driver_565rgb.d \
./rz_rsk_display_driver_24xrgb.d \
./rz_rsk_display_driver_32argb.d \
./rz_rsk_touch.d 

OBJS += \
./rz_display_driver_565rgb.o \
./rz_rsk_display_driver_24xrgb.o \
./rz_rsk_display_driver_32argb.o \
./rz_rsk_touch.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -fdiagnostics-show-caret -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=1000 -I../../rz_bsp/inc -I../../rz_bsp/inc/iodefines -I../../rz_bsp/src/arm/cmsis/inc -I../../rz_bsp/src/renesas/application/inc -I../../rz_bsp/src/renesas/application/console/inc -I../../rz_bsp/src/renesas/application/graphics/inc -I../../rz_bsp/src/renesas/application/system/inc -I../../rz_bsp/src/renesas/application/system/iodefines -I../../rz_bsp/src/renesas/application/system/iobitmasks -I../../rz_bsp/src/renesas/compiler/inc -I../../rz_bsp/src/renesas/configuration -I../../rz_bsp/src/renesas/configuration/os_abstraction/inc -I../../rz_bsp/src/renesas/configuration/sc/inc -I../../rz_bsp/src/renesas/drivers/r_l2cc -I../../rz_bsp/src/renesas/drivers/adc/inc -I../../rz_bsp/src/renesas/drivers/intc/inc -I../../rz_bsp/src/renesas/drivers/r_cache/inc -I../../rz_bsp/src/renesas/drivers/r_dmac/inc -I../../rz_bsp/src/renesas/drivers/r_switch/inc -I../../rz_bsp/src/renesas/drivers/ostm/inc -I../../rz_bsp/src/renesas/drivers/r_ceu/inc -I../../rz_bsp/src/renesas/drivers/r_i2c/inc -I../../rz_bsp/src/renesas/drivers/r_jcu/inc -I../../rz_bsp/src/renesas/drivers/r_led/inc -I../../rz_bsp/src/renesas/drivers/r_sci/inc -I../../rz_bsp/src/renesas/drivers/r_rspi/inc -I../../rz_bsp/src/renesas/drivers/r_ssif/inc -I../../rz_bsp/src/renesas/drivers/r_rtc/inc -I../../rz_bsp/src/renesas/drivers/r_vdc_vdec/inc -I../../rz_bsp/src/renesas/drivers/r_wdt/inc -I../../rz_bsp/src/renesas/drivers/r_eeprom/inc -I../../rz_bsp/src/renesas/middleware/riic_max9856/inc -I../../rz_bsp/src/renesas/middleware/riic_cat9554/inc -I../../rz_bsp/src/renesas/middleware/sound/inc -I../../rz_bsp/src/renesas/middleware/cmos/inc -I../../rz_bsp/src/renesas/middleware/fatfs/inc -I../../rz_bsp/src/renesas/middleware/pmod_lcd/inc -I../../rz_bsp/src/renesas/middleware/touch/inc -I../../rz_bsp/src/renesas/middleware/timer/inc -I../../rz_bsp/src/renesas/middleware/video/inc -I../../rz_bsp/azure/ports/cortex_a9/gnu/inc -I../../threadx/common/inc -I../../guix/common/inc -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

