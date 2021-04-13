################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/usbx_device_controllers/ux_dcd_rz_address_set.c \
../common/usbx_device_controllers/ux_dcd_rz_buffer_empty_interrupt.c \
../common/usbx_device_controllers/ux_dcd_rz_buffer_notready_interrupt.c \
../common/usbx_device_controllers/ux_dcd_rz_buffer_read.c \
../common/usbx_device_controllers/ux_dcd_rz_buffer_ready_interrupt.c \
../common/usbx_device_controllers/ux_dcd_rz_buffer_write.c \
../common/usbx_device_controllers/ux_dcd_rz_current_endpoint_change.c \
../common/usbx_device_controllers/ux_dcd_rz_data_buffersize.c \
../common/usbx_device_controllers/ux_dcd_rz_dma_register_clear.c \
../common/usbx_device_controllers/ux_dcd_rz_dma_register_read.c \
../common/usbx_device_controllers/ux_dcd_rz_dma_register_set.c \
../common/usbx_device_controllers/ux_dcd_rz_dma_register_write.c \
../common/usbx_device_controllers/ux_dcd_rz_dma_rx_interrupt_handler.c \
../common/usbx_device_controllers/ux_dcd_rz_dma_tx_interrupt_handler.c \
../common/usbx_device_controllers/ux_dcd_rz_endpoint_create.c \
../common/usbx_device_controllers/ux_dcd_rz_endpoint_destroy.c \
../common/usbx_device_controllers/ux_dcd_rz_endpoint_nak_set.c \
../common/usbx_device_controllers/ux_dcd_rz_endpoint_reset.c \
../common/usbx_device_controllers/ux_dcd_rz_endpoint_stall.c \
../common/usbx_device_controllers/ux_dcd_rz_endpoint_status.c \
../common/usbx_device_controllers/ux_dcd_rz_fifo_port_change.c \
../common/usbx_device_controllers/ux_dcd_rz_fifo_read.c \
../common/usbx_device_controllers/ux_dcd_rz_fifoc_write.c \
../common/usbx_device_controllers/ux_dcd_rz_fifod_write.c \
../common/usbx_device_controllers/ux_dcd_rz_frame_number_get.c \
../common/usbx_device_controllers/ux_dcd_rz_function.c \
../common/usbx_device_controllers/ux_dcd_rz_initialize.c \
../common/usbx_device_controllers/ux_dcd_rz_initialize_complete.c \
../common/usbx_device_controllers/ux_dcd_rz_interrupt_handler.c \
../common/usbx_device_controllers/ux_dcd_rz_low_level_setup.c \
../common/usbx_device_controllers/ux_dcd_rz_pipe_select.c \
../common/usbx_device_controllers/ux_dcd_rz_register_clear.c \
../common/usbx_device_controllers/ux_dcd_rz_register_read.c \
../common/usbx_device_controllers/ux_dcd_rz_register_set.c \
../common/usbx_device_controllers/ux_dcd_rz_register_write.c \
../common/usbx_device_controllers/ux_dcd_rz_state_change.c \
../common/usbx_device_controllers/ux_dcd_rz_transfer_callback.c \
../common/usbx_device_controllers/ux_dcd_rz_transfer_request.c 

C_DEPS += \
./common/usbx_device_controllers/ux_dcd_rz_address_set.d \
./common/usbx_device_controllers/ux_dcd_rz_buffer_empty_interrupt.d \
./common/usbx_device_controllers/ux_dcd_rz_buffer_notready_interrupt.d \
./common/usbx_device_controllers/ux_dcd_rz_buffer_read.d \
./common/usbx_device_controllers/ux_dcd_rz_buffer_ready_interrupt.d \
./common/usbx_device_controllers/ux_dcd_rz_buffer_write.d \
./common/usbx_device_controllers/ux_dcd_rz_current_endpoint_change.d \
./common/usbx_device_controllers/ux_dcd_rz_data_buffersize.d \
./common/usbx_device_controllers/ux_dcd_rz_dma_register_clear.d \
./common/usbx_device_controllers/ux_dcd_rz_dma_register_read.d \
./common/usbx_device_controllers/ux_dcd_rz_dma_register_set.d \
./common/usbx_device_controllers/ux_dcd_rz_dma_register_write.d \
./common/usbx_device_controllers/ux_dcd_rz_dma_rx_interrupt_handler.d \
./common/usbx_device_controllers/ux_dcd_rz_dma_tx_interrupt_handler.d \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_create.d \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_destroy.d \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_nak_set.d \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_reset.d \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_stall.d \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_status.d \
./common/usbx_device_controllers/ux_dcd_rz_fifo_port_change.d \
./common/usbx_device_controllers/ux_dcd_rz_fifo_read.d \
./common/usbx_device_controllers/ux_dcd_rz_fifoc_write.d \
./common/usbx_device_controllers/ux_dcd_rz_fifod_write.d \
./common/usbx_device_controllers/ux_dcd_rz_frame_number_get.d \
./common/usbx_device_controllers/ux_dcd_rz_function.d \
./common/usbx_device_controllers/ux_dcd_rz_initialize.d \
./common/usbx_device_controllers/ux_dcd_rz_initialize_complete.d \
./common/usbx_device_controllers/ux_dcd_rz_interrupt_handler.d \
./common/usbx_device_controllers/ux_dcd_rz_low_level_setup.d \
./common/usbx_device_controllers/ux_dcd_rz_pipe_select.d \
./common/usbx_device_controllers/ux_dcd_rz_register_clear.d \
./common/usbx_device_controllers/ux_dcd_rz_register_read.d \
./common/usbx_device_controllers/ux_dcd_rz_register_set.d \
./common/usbx_device_controllers/ux_dcd_rz_register_write.d \
./common/usbx_device_controllers/ux_dcd_rz_state_change.d \
./common/usbx_device_controllers/ux_dcd_rz_transfer_callback.d \
./common/usbx_device_controllers/ux_dcd_rz_transfer_request.d 

OBJS += \
./common/usbx_device_controllers/ux_dcd_rz_address_set.o \
./common/usbx_device_controllers/ux_dcd_rz_buffer_empty_interrupt.o \
./common/usbx_device_controllers/ux_dcd_rz_buffer_notready_interrupt.o \
./common/usbx_device_controllers/ux_dcd_rz_buffer_read.o \
./common/usbx_device_controllers/ux_dcd_rz_buffer_ready_interrupt.o \
./common/usbx_device_controllers/ux_dcd_rz_buffer_write.o \
./common/usbx_device_controllers/ux_dcd_rz_current_endpoint_change.o \
./common/usbx_device_controllers/ux_dcd_rz_data_buffersize.o \
./common/usbx_device_controllers/ux_dcd_rz_dma_register_clear.o \
./common/usbx_device_controllers/ux_dcd_rz_dma_register_read.o \
./common/usbx_device_controllers/ux_dcd_rz_dma_register_set.o \
./common/usbx_device_controllers/ux_dcd_rz_dma_register_write.o \
./common/usbx_device_controllers/ux_dcd_rz_dma_rx_interrupt_handler.o \
./common/usbx_device_controllers/ux_dcd_rz_dma_tx_interrupt_handler.o \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_create.o \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_destroy.o \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_nak_set.o \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_reset.o \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_stall.o \
./common/usbx_device_controllers/ux_dcd_rz_endpoint_status.o \
./common/usbx_device_controllers/ux_dcd_rz_fifo_port_change.o \
./common/usbx_device_controllers/ux_dcd_rz_fifo_read.o \
./common/usbx_device_controllers/ux_dcd_rz_fifoc_write.o \
./common/usbx_device_controllers/ux_dcd_rz_fifod_write.o \
./common/usbx_device_controllers/ux_dcd_rz_frame_number_get.o \
./common/usbx_device_controllers/ux_dcd_rz_function.o \
./common/usbx_device_controllers/ux_dcd_rz_initialize.o \
./common/usbx_device_controllers/ux_dcd_rz_initialize_complete.o \
./common/usbx_device_controllers/ux_dcd_rz_interrupt_handler.o \
./common/usbx_device_controllers/ux_dcd_rz_low_level_setup.o \
./common/usbx_device_controllers/ux_dcd_rz_pipe_select.o \
./common/usbx_device_controllers/ux_dcd_rz_register_clear.o \
./common/usbx_device_controllers/ux_dcd_rz_register_read.o \
./common/usbx_device_controllers/ux_dcd_rz_register_set.o \
./common/usbx_device_controllers/ux_dcd_rz_register_write.o \
./common/usbx_device_controllers/ux_dcd_rz_state_change.o \
./common/usbx_device_controllers/ux_dcd_rz_transfer_callback.o \
./common/usbx_device_controllers/ux_dcd_rz_transfer_request.o 


# Each subdirectory must supply rules for building sources it contributes
common/usbx_device_controllers/%.o: ../common/usbx_device_controllers/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=1000 -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\filex\common\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\azure\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\netxduo\common\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\usbx\common\core\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\threadx\common\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\usbx\common\usbx_host_classes\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\usbx\common\usbx_device_classes\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\usbx\common\usbx_device_controllers" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\usbx\common\usbx_host_controllers" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\usbx\common\usbx_network\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\usbx\common\usbx_pictbridge\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\arm\cmsis\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\compiler\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\configuration" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\configuration\os_abstraction\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\configuration\sc\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\graphics\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\console\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\system\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\system\iodefines" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\application\system\iobitmasks" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\adc\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\intc\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\ostm\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_cache\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_ceu\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_dmac\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_eeprom\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_i2c\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_jcu\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_l2cc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_led\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_rspi\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_rtc\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_sci\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_ssif\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_switch\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_vdc_vdec\inc" -I"C:\workspace\RZA1\Azure\rza1h_azure_gcc\rz_bsp\src\renesas\drivers\r_wdt\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

