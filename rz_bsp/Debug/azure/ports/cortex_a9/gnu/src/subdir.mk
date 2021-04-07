################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../azure/ports/cortex_a9/gnu/src/tx_initialize_low_level.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_context_restore.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_context_save.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_fiq_context_restore.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_fiq_context_save.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_fiq_nesting_end.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_fiq_nesting_start.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_interrupt_control.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_interrupt_disable.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_interrupt_restore.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_irq_nesting_end.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_irq_nesting_start.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_schedule.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_stack_build.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_system_return.S \
../azure/ports/cortex_a9/gnu/src/tx_thread_vectored_context_save.S \
../azure/ports/cortex_a9/gnu/src/tx_timer_interrupt.S 

C_SRCS += \
../azure/ports/cortex_a9/gnu/src/tx_timer_config.c 

C_DEPS += \
./azure/ports/cortex_a9/gnu/src/tx_timer_config.d 

OBJS += \
./azure/ports/cortex_a9/gnu/src/tx_initialize_low_level.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_context_restore.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_context_save.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_fiq_context_restore.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_fiq_context_save.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_fiq_nesting_end.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_fiq_nesting_start.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_interrupt_control.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_interrupt_disable.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_interrupt_restore.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_irq_nesting_end.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_irq_nesting_start.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_schedule.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_stack_build.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_system_return.o \
./azure/ports/cortex_a9/gnu/src/tx_thread_vectored_context_save.o \
./azure/ports/cortex_a9/gnu/src/tx_timer_config.o \
./azure/ports/cortex_a9/gnu/src/tx_timer_interrupt.o 

S_UPPER_DEPS += \
./azure/ports/cortex_a9/gnu/src/tx_initialize_low_level.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_context_restore.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_context_save.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_fiq_context_restore.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_fiq_context_save.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_fiq_nesting_end.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_fiq_nesting_start.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_interrupt_control.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_interrupt_disable.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_interrupt_restore.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_irq_nesting_end.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_irq_nesting_start.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_schedule.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_stack_build.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_system_return.d \
./azure/ports/cortex_a9/gnu/src/tx_thread_vectored_context_save.d \
./azure/ports/cortex_a9/gnu/src/tx_timer_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
azure/ports/cortex_a9/gnu/src/%.o: ../azure/ports/cortex_a9/gnu/src/%.S
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -x assembler-with-cpp -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
azure/ports/cortex_a9/gnu/src/%.o: ../azure/ports/cortex_a9/gnu/src/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=100 -DUSE_STREAM_IT_RZ -D__MISC_VISIBLE -DCORTEX_A -DRSK -D_DEBUG -DLWIP_DEBUG -D_WINSOCKAPI_ -D_WINSOCK_H -DCPPAPP -DeC_TARGET_ENV_FREERTOS -DeC_TARGET_ENV_RZA1H -DeC_TARGET_COMPILER_GCC -DeC_USE_TRIG_TABLE -DEC_STRING_FORMAT_UTF16 -DeGML_USE_LITTLE_HELPERS -DSTREAMRUNTIME_APPLICATION -DGUILIANI_STREAM_GUI -DGUILIANI_WRITE_GUI -DPLATFORM_RENESAS_STREAMIT_V2 -DFRAME_BUFFER_WIDTH=800 -DFRAME_BUFFER_HEIGHT=480 -DVSYNC_WAIT_COUNT=1 -DVDC5_INT_PRIORITY=25 -DGUILIANI_TASK_STACK_SIZE=20480 -DFRAME_BUFFER_BITS_PER_PIXEL_16 -D_DEFAULT_SOURCE -DNO_ICONV -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/arm/cmsis/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/sound/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/riic_max9856/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/riic_cat9554/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_l2cc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/console/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/graphics/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/system/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/system/iodefines" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/application/system/iobitmasks" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/compiler/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/configuration" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/configuration/os_abstraction/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/configuration/sc/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/adc/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/intc/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_cache/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_dmac/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_switch/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/ostm/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_ceu/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_i2c/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_jcu/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_led/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_sci/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_rspi/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_ssif/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_rtc/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_vdc_vdec/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_wdt/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/drivers/r_eeprom/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/cmos/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/fatfs/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/pmod_lcd/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/touch/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/timer/inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/src/renesas/middleware/video/inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\middleware\ethernet\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\threadx\common\inc" -I"C:/workspace/RZA1/rza1h_azure_gcc/updated/rz_bsp/azure/ports/cortex_a9/gnu/inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

