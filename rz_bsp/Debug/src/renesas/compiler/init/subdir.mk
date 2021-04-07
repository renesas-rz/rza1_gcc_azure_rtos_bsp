################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/renesas/compiler/init/libc_init.c \
../src/renesas/compiler/init/peripheral_init_basic.c \
../src/renesas/compiler/init/pl310.c \
../src/renesas/compiler/init/r_compiler_abstraction.c \
../src/renesas/compiler/init/resetprg.c \
../src/renesas/compiler/init/stb_init.c \
../src/renesas/compiler/init/syscalls.c 

C_DEPS += \
./src/renesas/compiler/init/libc_init.d \
./src/renesas/compiler/init/peripheral_init_basic.d \
./src/renesas/compiler/init/pl310.d \
./src/renesas/compiler/init/r_compiler_abstraction.d \
./src/renesas/compiler/init/resetprg.d \
./src/renesas/compiler/init/stb_init.d \
./src/renesas/compiler/init/syscalls.d 

OBJS += \
./src/renesas/compiler/init/libc_init.o \
./src/renesas/compiler/init/peripheral_init_basic.o \
./src/renesas/compiler/init/pl310.o \
./src/renesas/compiler/init/r_compiler_abstraction.o \
./src/renesas/compiler/init/resetprg.o \
./src/renesas/compiler/init/stb_init.o \
./src/renesas/compiler/init/syscalls.o 


# Each subdirectory must supply rules for building sources it contributes
src/renesas/compiler/init/%.o: ../src/renesas/compiler/init/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -DUSE_STREAM_IT_RZ -D__MISC_VISIBLE -DCORTEX_A -DRSK -D_DEBUG -DLWIP_DEBUG -D_WINSOCKAPI_ -D_WINSOCK_H -DCPPAPP -DeC_TARGET_ENV_FREERTOS -DeC_TARGET_ENV_RZA1H -DeC_TARGET_COMPILER_GCC -DeC_USE_TRIG_TABLE -DEC_STRING_FORMAT_UTF16 -DeGML_USE_LITTLE_HELPERS -DSTREAMRUNTIME_APPLICATION -DGUILIANI_STREAM_GUI -DGUILIANI_WRITE_GUI -DPLATFORM_RENESAS_STREAMIT_V2 -DFRAME_BUFFER_WIDTH=800 -DFRAME_BUFFER_HEIGHT=480 -DVSYNC_WAIT_COUNT=1 -DVDC5_INT_PRIORITY=25 -DGUILIANI_TASK_STACK_SIZE=20480 -DFRAME_BUFFER_BITS_PER_PIXEL_16 -D_DEFAULT_SOURCE -DNO_ICONV -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/arm/cmsis/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/sound/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/riic_max9856/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/riic_cat9554/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_l2cc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/console/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/graphics/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/system/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/system/iodefines" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/system/iobitmasks" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/compiler/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/configuration" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/configuration/os_abstraction/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/configuration/sc/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/adc/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/intc/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_cache/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_dmac/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_switch/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/ostm/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_ceu/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_i2c/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_jcu/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_led/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_sci/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_rspi/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_ssif/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_rtc/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_vdc_vdec/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_wdt/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_eeprom/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/cmos/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/fatfs/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/pmod_lcd/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/touch/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/timer/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/video/inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\middleware\ethernet\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\threadx\common\inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/azure/ports/cortex_a9/gnu/inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

