################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/core/src/ux_dcd_sim_slave_address_set.c \
../common/core/src/ux_dcd_sim_slave_endpoint_create.c \
../common/core/src/ux_dcd_sim_slave_endpoint_destroy.c \
../common/core/src/ux_dcd_sim_slave_endpoint_reset.c \
../common/core/src/ux_dcd_sim_slave_endpoint_stall.c \
../common/core/src/ux_dcd_sim_slave_endpoint_status.c \
../common/core/src/ux_dcd_sim_slave_frame_number_get.c \
../common/core/src/ux_dcd_sim_slave_function.c \
../common/core/src/ux_dcd_sim_slave_initialize.c \
../common/core/src/ux_dcd_sim_slave_initialize_complete.c \
../common/core/src/ux_dcd_sim_slave_state_change.c \
../common/core/src/ux_dcd_sim_slave_transfer_abort.c \
../common/core/src/ux_dcd_sim_slave_transfer_request.c \
../common/core/src/ux_device_class_dpump_activate.c \
../common/core/src/ux_device_class_dpump_change.c \
../common/core/src/ux_device_class_dpump_deactivate.c \
../common/core/src/ux_device_class_dpump_entry.c \
../common/core/src/ux_device_class_dpump_initialize.c \
../common/core/src/ux_device_class_dpump_read.c \
../common/core/src/ux_device_class_dpump_thread.c \
../common/core/src/ux_device_class_dpump_write.c \
../common/core/src/ux_device_stack_alternate_setting_get.c \
../common/core/src/ux_device_stack_alternate_setting_set.c \
../common/core/src/ux_device_stack_class_register.c \
../common/core/src/ux_device_stack_class_unregister.c \
../common/core/src/ux_device_stack_clear_feature.c \
../common/core/src/ux_device_stack_configuration_get.c \
../common/core/src/ux_device_stack_configuration_set.c \
../common/core/src/ux_device_stack_control_request_process.c \
../common/core/src/ux_device_stack_descriptor_send.c \
../common/core/src/ux_device_stack_disconnect.c \
../common/core/src/ux_device_stack_endpoint_stall.c \
../common/core/src/ux_device_stack_get_status.c \
../common/core/src/ux_device_stack_host_wakeup.c \
../common/core/src/ux_device_stack_initialize.c \
../common/core/src/ux_device_stack_interface_delete.c \
../common/core/src/ux_device_stack_interface_get.c \
../common/core/src/ux_device_stack_interface_set.c \
../common/core/src/ux_device_stack_interface_start.c \
../common/core/src/ux_device_stack_microsoft_extension_register.c \
../common/core/src/ux_device_stack_set_feature.c \
../common/core/src/ux_device_stack_transfer_abort.c \
../common/core/src/ux_device_stack_transfer_all_request_abort.c \
../common/core/src/ux_device_stack_transfer_request.c \
../common/core/src/ux_device_stack_uninitialize.c \
../common/core/src/ux_hcd_sim_host_asynch_queue_process.c \
../common/core/src/ux_hcd_sim_host_asynch_schedule.c \
../common/core/src/ux_hcd_sim_host_asynchronous_endpoint_create.c \
../common/core/src/ux_hcd_sim_host_asynchronous_endpoint_destroy.c \
../common/core/src/ux_hcd_sim_host_controller_disable.c \
../common/core/src/ux_hcd_sim_host_ed_obtain.c \
../common/core/src/ux_hcd_sim_host_ed_td_clean.c \
../common/core/src/ux_hcd_sim_host_endpoint_reset.c \
../common/core/src/ux_hcd_sim_host_entry.c \
../common/core/src/ux_hcd_sim_host_frame_number_get.c \
../common/core/src/ux_hcd_sim_host_frame_number_set.c \
../common/core/src/ux_hcd_sim_host_initialize.c \
../common/core/src/ux_hcd_sim_host_interrupt_endpoint_create.c \
../common/core/src/ux_hcd_sim_host_iso_queue_process.c \
../common/core/src/ux_hcd_sim_host_iso_schedule.c \
../common/core/src/ux_hcd_sim_host_isochronous_endpoint_create.c \
../common/core/src/ux_hcd_sim_host_isochronous_td_obtain.c \
../common/core/src/ux_hcd_sim_host_least_traffic_list_get.c \
../common/core/src/ux_hcd_sim_host_periodic_endpoint_destroy.c \
../common/core/src/ux_hcd_sim_host_periodic_schedule.c \
../common/core/src/ux_hcd_sim_host_periodic_tree_create.c \
../common/core/src/ux_hcd_sim_host_port_reset.c \
../common/core/src/ux_hcd_sim_host_port_status_get.c \
../common/core/src/ux_hcd_sim_host_regular_td_obtain.c \
../common/core/src/ux_hcd_sim_host_request_bulk_transfer.c \
../common/core/src/ux_hcd_sim_host_request_control_transfer.c \
../common/core/src/ux_hcd_sim_host_request_interupt_transfer.c \
../common/core/src/ux_hcd_sim_host_request_isochronous_transfer.c \
../common/core/src/ux_hcd_sim_host_request_transfer.c \
../common/core/src/ux_hcd_sim_host_timer_function.c \
../common/core/src/ux_hcd_sim_host_transaction_schedule.c \
../common/core/src/ux_hcd_sim_host_transfer_abort.c \
../common/core/src/ux_host_class_dpump_activate.c \
../common/core/src/ux_host_class_dpump_configure.c \
../common/core/src/ux_host_class_dpump_deactivate.c \
../common/core/src/ux_host_class_dpump_endpoints_get.c \
../common/core/src/ux_host_class_dpump_entry.c \
../common/core/src/ux_host_class_dpump_ioctl.c \
../common/core/src/ux_host_class_dpump_read.c \
../common/core/src/ux_host_class_dpump_write.c \
../common/core/src/ux_host_stack_bandwidth_check.c \
../common/core/src/ux_host_stack_bandwidth_claim.c \
../common/core/src/ux_host_stack_bandwidth_release.c \
../common/core/src/ux_host_stack_class_call.c \
../common/core/src/ux_host_stack_class_device_scan.c \
../common/core/src/ux_host_stack_class_get.c \
../common/core/src/ux_host_stack_class_instance_create.c \
../common/core/src/ux_host_stack_class_instance_destroy.c \
../common/core/src/ux_host_stack_class_instance_get.c \
../common/core/src/ux_host_stack_class_instance_verify.c \
../common/core/src/ux_host_stack_class_interface_scan.c \
../common/core/src/ux_host_stack_class_register.c \
../common/core/src/ux_host_stack_class_unregister.c \
../common/core/src/ux_host_stack_configuration_descriptor_parse.c \
../common/core/src/ux_host_stack_configuration_enumerate.c \
../common/core/src/ux_host_stack_configuration_instance_create.c \
../common/core/src/ux_host_stack_configuration_instance_delete.c \
../common/core/src/ux_host_stack_configuration_interface_get.c \
../common/core/src/ux_host_stack_configuration_set.c \
../common/core/src/ux_host_stack_delay_ms.c \
../common/core/src/ux_host_stack_device_address_set.c \
../common/core/src/ux_host_stack_device_configuration_get.c \
../common/core/src/ux_host_stack_device_configuration_reset.c \
../common/core/src/ux_host_stack_device_configuration_select.c \
../common/core/src/ux_host_stack_device_descriptor_read.c \
../common/core/src/ux_host_stack_device_get.c \
../common/core/src/ux_host_stack_device_remove.c \
../common/core/src/ux_host_stack_device_resources_free.c \
../common/core/src/ux_host_stack_endpoint_instance_create.c \
../common/core/src/ux_host_stack_endpoint_instance_delete.c \
../common/core/src/ux_host_stack_endpoint_reset.c \
../common/core/src/ux_host_stack_endpoint_transfer_abort.c \
../common/core/src/ux_host_stack_enum_thread_entry.c \
../common/core/src/ux_host_stack_hcd_register.c \
../common/core/src/ux_host_stack_hcd_thread_entry.c \
../common/core/src/ux_host_stack_hcd_transfer_request.c \
../common/core/src/ux_host_stack_hcd_unregister.c \
../common/core/src/ux_host_stack_hnp_polling_thread_entry.c \
../common/core/src/ux_host_stack_initialize.c \
../common/core/src/ux_host_stack_interface_endpoint_get.c \
../common/core/src/ux_host_stack_interface_instance_create.c \
../common/core/src/ux_host_stack_interface_instance_delete.c \
../common/core/src/ux_host_stack_interface_set.c \
../common/core/src/ux_host_stack_interface_setting_select.c \
../common/core/src/ux_host_stack_interfaces_scan.c \
../common/core/src/ux_host_stack_new_configuration_create.c \
../common/core/src/ux_host_stack_new_device_create.c \
../common/core/src/ux_host_stack_new_device_get.c \
../common/core/src/ux_host_stack_new_endpoint_create.c \
../common/core/src/ux_host_stack_new_interface_create.c \
../common/core/src/ux_host_stack_rh_change_process.c \
../common/core/src/ux_host_stack_rh_device_extraction.c \
../common/core/src/ux_host_stack_rh_device_insertion.c \
../common/core/src/ux_host_stack_role_swap.c \
../common/core/src/ux_host_stack_transfer_request.c \
../common/core/src/ux_host_stack_transfer_request_abort.c \
../common/core/src/ux_host_stack_uninitialize.c \
../common/core/src/ux_system_error_handler.c \
../common/core/src/ux_system_initialize.c \
../common/core/src/ux_system_uninitialize.c \
../common/core/src/ux_trace_event_insert.c \
../common/core/src/ux_trace_event_update.c \
../common/core/src/ux_trace_object_register.c \
../common/core/src/ux_trace_object_unregister.c \
../common/core/src/ux_utility_debug_callback_register.c \
../common/core/src/ux_utility_debug_log.c \
../common/core/src/ux_utility_delay_ms.c \
../common/core/src/ux_utility_descriptor_pack.c \
../common/core/src/ux_utility_descriptor_parse.c \
../common/core/src/ux_utility_error_callback_register.c \
../common/core/src/ux_utility_event_flags_create.c \
../common/core/src/ux_utility_event_flags_delete.c \
../common/core/src/ux_utility_event_flags_get.c \
../common/core/src/ux_utility_event_flags_set.c \
../common/core/src/ux_utility_long_get.c \
../common/core/src/ux_utility_long_get_big_endian.c \
../common/core/src/ux_utility_long_put.c \
../common/core/src/ux_utility_long_put_big_endian.c \
../common/core/src/ux_utility_memory_allocate.c \
../common/core/src/ux_utility_memory_allocate_add_safe.c \
../common/core/src/ux_utility_memory_allocate_mulc_safe.c \
../common/core/src/ux_utility_memory_allocate_mulv_safe.c \
../common/core/src/ux_utility_memory_compare.c \
../common/core/src/ux_utility_memory_copy.c \
../common/core/src/ux_utility_memory_free.c \
../common/core/src/ux_utility_memory_free_block_best_get.c \
../common/core/src/ux_utility_memory_set.c \
../common/core/src/ux_utility_mutex_create.c \
../common/core/src/ux_utility_mutex_delete.c \
../common/core/src/ux_utility_mutex_off.c \
../common/core/src/ux_utility_mutex_on.c \
../common/core/src/ux_utility_pci_class_scan.c \
../common/core/src/ux_utility_pci_read.c \
../common/core/src/ux_utility_pci_write.c \
../common/core/src/ux_utility_physical_address.c \
../common/core/src/ux_utility_semaphore_create.c \
../common/core/src/ux_utility_semaphore_delete.c \
../common/core/src/ux_utility_semaphore_get.c \
../common/core/src/ux_utility_semaphore_put.c \
../common/core/src/ux_utility_set_interrupt_handler.c \
../common/core/src/ux_utility_short_get.c \
../common/core/src/ux_utility_short_get_big_endian.c \
../common/core/src/ux_utility_short_put.c \
../common/core/src/ux_utility_short_put_big_endian.c \
../common/core/src/ux_utility_string_length_check.c \
../common/core/src/ux_utility_string_length_get.c \
../common/core/src/ux_utility_string_to_unicode.c \
../common/core/src/ux_utility_thread_create.c \
../common/core/src/ux_utility_thread_delete.c \
../common/core/src/ux_utility_thread_identify.c \
../common/core/src/ux_utility_thread_relinquish.c \
../common/core/src/ux_utility_thread_resume.c \
../common/core/src/ux_utility_thread_schedule_other.c \
../common/core/src/ux_utility_thread_sleep.c \
../common/core/src/ux_utility_thread_suspend.c \
../common/core/src/ux_utility_timer_create.c \
../common/core/src/ux_utility_timer_delete.c \
../common/core/src/ux_utility_unicode_to_string.c \
../common/core/src/ux_utility_virtual_address.c 

C_DEPS += \
./common/core/src/ux_dcd_sim_slave_address_set.d \
./common/core/src/ux_dcd_sim_slave_endpoint_create.d \
./common/core/src/ux_dcd_sim_slave_endpoint_destroy.d \
./common/core/src/ux_dcd_sim_slave_endpoint_reset.d \
./common/core/src/ux_dcd_sim_slave_endpoint_stall.d \
./common/core/src/ux_dcd_sim_slave_endpoint_status.d \
./common/core/src/ux_dcd_sim_slave_frame_number_get.d \
./common/core/src/ux_dcd_sim_slave_function.d \
./common/core/src/ux_dcd_sim_slave_initialize.d \
./common/core/src/ux_dcd_sim_slave_initialize_complete.d \
./common/core/src/ux_dcd_sim_slave_state_change.d \
./common/core/src/ux_dcd_sim_slave_transfer_abort.d \
./common/core/src/ux_dcd_sim_slave_transfer_request.d \
./common/core/src/ux_device_class_dpump_activate.d \
./common/core/src/ux_device_class_dpump_change.d \
./common/core/src/ux_device_class_dpump_deactivate.d \
./common/core/src/ux_device_class_dpump_entry.d \
./common/core/src/ux_device_class_dpump_initialize.d \
./common/core/src/ux_device_class_dpump_read.d \
./common/core/src/ux_device_class_dpump_thread.d \
./common/core/src/ux_device_class_dpump_write.d \
./common/core/src/ux_device_stack_alternate_setting_get.d \
./common/core/src/ux_device_stack_alternate_setting_set.d \
./common/core/src/ux_device_stack_class_register.d \
./common/core/src/ux_device_stack_class_unregister.d \
./common/core/src/ux_device_stack_clear_feature.d \
./common/core/src/ux_device_stack_configuration_get.d \
./common/core/src/ux_device_stack_configuration_set.d \
./common/core/src/ux_device_stack_control_request_process.d \
./common/core/src/ux_device_stack_descriptor_send.d \
./common/core/src/ux_device_stack_disconnect.d \
./common/core/src/ux_device_stack_endpoint_stall.d \
./common/core/src/ux_device_stack_get_status.d \
./common/core/src/ux_device_stack_host_wakeup.d \
./common/core/src/ux_device_stack_initialize.d \
./common/core/src/ux_device_stack_interface_delete.d \
./common/core/src/ux_device_stack_interface_get.d \
./common/core/src/ux_device_stack_interface_set.d \
./common/core/src/ux_device_stack_interface_start.d \
./common/core/src/ux_device_stack_microsoft_extension_register.d \
./common/core/src/ux_device_stack_set_feature.d \
./common/core/src/ux_device_stack_transfer_abort.d \
./common/core/src/ux_device_stack_transfer_all_request_abort.d \
./common/core/src/ux_device_stack_transfer_request.d \
./common/core/src/ux_device_stack_uninitialize.d \
./common/core/src/ux_hcd_sim_host_asynch_queue_process.d \
./common/core/src/ux_hcd_sim_host_asynch_schedule.d \
./common/core/src/ux_hcd_sim_host_asynchronous_endpoint_create.d \
./common/core/src/ux_hcd_sim_host_asynchronous_endpoint_destroy.d \
./common/core/src/ux_hcd_sim_host_controller_disable.d \
./common/core/src/ux_hcd_sim_host_ed_obtain.d \
./common/core/src/ux_hcd_sim_host_ed_td_clean.d \
./common/core/src/ux_hcd_sim_host_endpoint_reset.d \
./common/core/src/ux_hcd_sim_host_entry.d \
./common/core/src/ux_hcd_sim_host_frame_number_get.d \
./common/core/src/ux_hcd_sim_host_frame_number_set.d \
./common/core/src/ux_hcd_sim_host_initialize.d \
./common/core/src/ux_hcd_sim_host_interrupt_endpoint_create.d \
./common/core/src/ux_hcd_sim_host_iso_queue_process.d \
./common/core/src/ux_hcd_sim_host_iso_schedule.d \
./common/core/src/ux_hcd_sim_host_isochronous_endpoint_create.d \
./common/core/src/ux_hcd_sim_host_isochronous_td_obtain.d \
./common/core/src/ux_hcd_sim_host_least_traffic_list_get.d \
./common/core/src/ux_hcd_sim_host_periodic_endpoint_destroy.d \
./common/core/src/ux_hcd_sim_host_periodic_schedule.d \
./common/core/src/ux_hcd_sim_host_periodic_tree_create.d \
./common/core/src/ux_hcd_sim_host_port_reset.d \
./common/core/src/ux_hcd_sim_host_port_status_get.d \
./common/core/src/ux_hcd_sim_host_regular_td_obtain.d \
./common/core/src/ux_hcd_sim_host_request_bulk_transfer.d \
./common/core/src/ux_hcd_sim_host_request_control_transfer.d \
./common/core/src/ux_hcd_sim_host_request_interupt_transfer.d \
./common/core/src/ux_hcd_sim_host_request_isochronous_transfer.d \
./common/core/src/ux_hcd_sim_host_request_transfer.d \
./common/core/src/ux_hcd_sim_host_timer_function.d \
./common/core/src/ux_hcd_sim_host_transaction_schedule.d \
./common/core/src/ux_hcd_sim_host_transfer_abort.d \
./common/core/src/ux_host_class_dpump_activate.d \
./common/core/src/ux_host_class_dpump_configure.d \
./common/core/src/ux_host_class_dpump_deactivate.d \
./common/core/src/ux_host_class_dpump_endpoints_get.d \
./common/core/src/ux_host_class_dpump_entry.d \
./common/core/src/ux_host_class_dpump_ioctl.d \
./common/core/src/ux_host_class_dpump_read.d \
./common/core/src/ux_host_class_dpump_write.d \
./common/core/src/ux_host_stack_bandwidth_check.d \
./common/core/src/ux_host_stack_bandwidth_claim.d \
./common/core/src/ux_host_stack_bandwidth_release.d \
./common/core/src/ux_host_stack_class_call.d \
./common/core/src/ux_host_stack_class_device_scan.d \
./common/core/src/ux_host_stack_class_get.d \
./common/core/src/ux_host_stack_class_instance_create.d \
./common/core/src/ux_host_stack_class_instance_destroy.d \
./common/core/src/ux_host_stack_class_instance_get.d \
./common/core/src/ux_host_stack_class_instance_verify.d \
./common/core/src/ux_host_stack_class_interface_scan.d \
./common/core/src/ux_host_stack_class_register.d \
./common/core/src/ux_host_stack_class_unregister.d \
./common/core/src/ux_host_stack_configuration_descriptor_parse.d \
./common/core/src/ux_host_stack_configuration_enumerate.d \
./common/core/src/ux_host_stack_configuration_instance_create.d \
./common/core/src/ux_host_stack_configuration_instance_delete.d \
./common/core/src/ux_host_stack_configuration_interface_get.d \
./common/core/src/ux_host_stack_configuration_set.d \
./common/core/src/ux_host_stack_delay_ms.d \
./common/core/src/ux_host_stack_device_address_set.d \
./common/core/src/ux_host_stack_device_configuration_get.d \
./common/core/src/ux_host_stack_device_configuration_reset.d \
./common/core/src/ux_host_stack_device_configuration_select.d \
./common/core/src/ux_host_stack_device_descriptor_read.d \
./common/core/src/ux_host_stack_device_get.d \
./common/core/src/ux_host_stack_device_remove.d \
./common/core/src/ux_host_stack_device_resources_free.d \
./common/core/src/ux_host_stack_endpoint_instance_create.d \
./common/core/src/ux_host_stack_endpoint_instance_delete.d \
./common/core/src/ux_host_stack_endpoint_reset.d \
./common/core/src/ux_host_stack_endpoint_transfer_abort.d \
./common/core/src/ux_host_stack_enum_thread_entry.d \
./common/core/src/ux_host_stack_hcd_register.d \
./common/core/src/ux_host_stack_hcd_thread_entry.d \
./common/core/src/ux_host_stack_hcd_transfer_request.d \
./common/core/src/ux_host_stack_hcd_unregister.d \
./common/core/src/ux_host_stack_hnp_polling_thread_entry.d \
./common/core/src/ux_host_stack_initialize.d \
./common/core/src/ux_host_stack_interface_endpoint_get.d \
./common/core/src/ux_host_stack_interface_instance_create.d \
./common/core/src/ux_host_stack_interface_instance_delete.d \
./common/core/src/ux_host_stack_interface_set.d \
./common/core/src/ux_host_stack_interface_setting_select.d \
./common/core/src/ux_host_stack_interfaces_scan.d \
./common/core/src/ux_host_stack_new_configuration_create.d \
./common/core/src/ux_host_stack_new_device_create.d \
./common/core/src/ux_host_stack_new_device_get.d \
./common/core/src/ux_host_stack_new_endpoint_create.d \
./common/core/src/ux_host_stack_new_interface_create.d \
./common/core/src/ux_host_stack_rh_change_process.d \
./common/core/src/ux_host_stack_rh_device_extraction.d \
./common/core/src/ux_host_stack_rh_device_insertion.d \
./common/core/src/ux_host_stack_role_swap.d \
./common/core/src/ux_host_stack_transfer_request.d \
./common/core/src/ux_host_stack_transfer_request_abort.d \
./common/core/src/ux_host_stack_uninitialize.d \
./common/core/src/ux_system_error_handler.d \
./common/core/src/ux_system_initialize.d \
./common/core/src/ux_system_uninitialize.d \
./common/core/src/ux_trace_event_insert.d \
./common/core/src/ux_trace_event_update.d \
./common/core/src/ux_trace_object_register.d \
./common/core/src/ux_trace_object_unregister.d \
./common/core/src/ux_utility_debug_callback_register.d \
./common/core/src/ux_utility_debug_log.d \
./common/core/src/ux_utility_delay_ms.d \
./common/core/src/ux_utility_descriptor_pack.d \
./common/core/src/ux_utility_descriptor_parse.d \
./common/core/src/ux_utility_error_callback_register.d \
./common/core/src/ux_utility_event_flags_create.d \
./common/core/src/ux_utility_event_flags_delete.d \
./common/core/src/ux_utility_event_flags_get.d \
./common/core/src/ux_utility_event_flags_set.d \
./common/core/src/ux_utility_long_get.d \
./common/core/src/ux_utility_long_get_big_endian.d \
./common/core/src/ux_utility_long_put.d \
./common/core/src/ux_utility_long_put_big_endian.d \
./common/core/src/ux_utility_memory_allocate.d \
./common/core/src/ux_utility_memory_allocate_add_safe.d \
./common/core/src/ux_utility_memory_allocate_mulc_safe.d \
./common/core/src/ux_utility_memory_allocate_mulv_safe.d \
./common/core/src/ux_utility_memory_compare.d \
./common/core/src/ux_utility_memory_copy.d \
./common/core/src/ux_utility_memory_free.d \
./common/core/src/ux_utility_memory_free_block_best_get.d \
./common/core/src/ux_utility_memory_set.d \
./common/core/src/ux_utility_mutex_create.d \
./common/core/src/ux_utility_mutex_delete.d \
./common/core/src/ux_utility_mutex_off.d \
./common/core/src/ux_utility_mutex_on.d \
./common/core/src/ux_utility_pci_class_scan.d \
./common/core/src/ux_utility_pci_read.d \
./common/core/src/ux_utility_pci_write.d \
./common/core/src/ux_utility_physical_address.d \
./common/core/src/ux_utility_semaphore_create.d \
./common/core/src/ux_utility_semaphore_delete.d \
./common/core/src/ux_utility_semaphore_get.d \
./common/core/src/ux_utility_semaphore_put.d \
./common/core/src/ux_utility_set_interrupt_handler.d \
./common/core/src/ux_utility_short_get.d \
./common/core/src/ux_utility_short_get_big_endian.d \
./common/core/src/ux_utility_short_put.d \
./common/core/src/ux_utility_short_put_big_endian.d \
./common/core/src/ux_utility_string_length_check.d \
./common/core/src/ux_utility_string_length_get.d \
./common/core/src/ux_utility_string_to_unicode.d \
./common/core/src/ux_utility_thread_create.d \
./common/core/src/ux_utility_thread_delete.d \
./common/core/src/ux_utility_thread_identify.d \
./common/core/src/ux_utility_thread_relinquish.d \
./common/core/src/ux_utility_thread_resume.d \
./common/core/src/ux_utility_thread_schedule_other.d \
./common/core/src/ux_utility_thread_sleep.d \
./common/core/src/ux_utility_thread_suspend.d \
./common/core/src/ux_utility_timer_create.d \
./common/core/src/ux_utility_timer_delete.d \
./common/core/src/ux_utility_unicode_to_string.d \
./common/core/src/ux_utility_virtual_address.d 

OBJS += \
./common/core/src/ux_dcd_sim_slave_address_set.o \
./common/core/src/ux_dcd_sim_slave_endpoint_create.o \
./common/core/src/ux_dcd_sim_slave_endpoint_destroy.o \
./common/core/src/ux_dcd_sim_slave_endpoint_reset.o \
./common/core/src/ux_dcd_sim_slave_endpoint_stall.o \
./common/core/src/ux_dcd_sim_slave_endpoint_status.o \
./common/core/src/ux_dcd_sim_slave_frame_number_get.o \
./common/core/src/ux_dcd_sim_slave_function.o \
./common/core/src/ux_dcd_sim_slave_initialize.o \
./common/core/src/ux_dcd_sim_slave_initialize_complete.o \
./common/core/src/ux_dcd_sim_slave_state_change.o \
./common/core/src/ux_dcd_sim_slave_transfer_abort.o \
./common/core/src/ux_dcd_sim_slave_transfer_request.o \
./common/core/src/ux_device_class_dpump_activate.o \
./common/core/src/ux_device_class_dpump_change.o \
./common/core/src/ux_device_class_dpump_deactivate.o \
./common/core/src/ux_device_class_dpump_entry.o \
./common/core/src/ux_device_class_dpump_initialize.o \
./common/core/src/ux_device_class_dpump_read.o \
./common/core/src/ux_device_class_dpump_thread.o \
./common/core/src/ux_device_class_dpump_write.o \
./common/core/src/ux_device_stack_alternate_setting_get.o \
./common/core/src/ux_device_stack_alternate_setting_set.o \
./common/core/src/ux_device_stack_class_register.o \
./common/core/src/ux_device_stack_class_unregister.o \
./common/core/src/ux_device_stack_clear_feature.o \
./common/core/src/ux_device_stack_configuration_get.o \
./common/core/src/ux_device_stack_configuration_set.o \
./common/core/src/ux_device_stack_control_request_process.o \
./common/core/src/ux_device_stack_descriptor_send.o \
./common/core/src/ux_device_stack_disconnect.o \
./common/core/src/ux_device_stack_endpoint_stall.o \
./common/core/src/ux_device_stack_get_status.o \
./common/core/src/ux_device_stack_host_wakeup.o \
./common/core/src/ux_device_stack_initialize.o \
./common/core/src/ux_device_stack_interface_delete.o \
./common/core/src/ux_device_stack_interface_get.o \
./common/core/src/ux_device_stack_interface_set.o \
./common/core/src/ux_device_stack_interface_start.o \
./common/core/src/ux_device_stack_microsoft_extension_register.o \
./common/core/src/ux_device_stack_set_feature.o \
./common/core/src/ux_device_stack_transfer_abort.o \
./common/core/src/ux_device_stack_transfer_all_request_abort.o \
./common/core/src/ux_device_stack_transfer_request.o \
./common/core/src/ux_device_stack_uninitialize.o \
./common/core/src/ux_hcd_sim_host_asynch_queue_process.o \
./common/core/src/ux_hcd_sim_host_asynch_schedule.o \
./common/core/src/ux_hcd_sim_host_asynchronous_endpoint_create.o \
./common/core/src/ux_hcd_sim_host_asynchronous_endpoint_destroy.o \
./common/core/src/ux_hcd_sim_host_controller_disable.o \
./common/core/src/ux_hcd_sim_host_ed_obtain.o \
./common/core/src/ux_hcd_sim_host_ed_td_clean.o \
./common/core/src/ux_hcd_sim_host_endpoint_reset.o \
./common/core/src/ux_hcd_sim_host_entry.o \
./common/core/src/ux_hcd_sim_host_frame_number_get.o \
./common/core/src/ux_hcd_sim_host_frame_number_set.o \
./common/core/src/ux_hcd_sim_host_initialize.o \
./common/core/src/ux_hcd_sim_host_interrupt_endpoint_create.o \
./common/core/src/ux_hcd_sim_host_iso_queue_process.o \
./common/core/src/ux_hcd_sim_host_iso_schedule.o \
./common/core/src/ux_hcd_sim_host_isochronous_endpoint_create.o \
./common/core/src/ux_hcd_sim_host_isochronous_td_obtain.o \
./common/core/src/ux_hcd_sim_host_least_traffic_list_get.o \
./common/core/src/ux_hcd_sim_host_periodic_endpoint_destroy.o \
./common/core/src/ux_hcd_sim_host_periodic_schedule.o \
./common/core/src/ux_hcd_sim_host_periodic_tree_create.o \
./common/core/src/ux_hcd_sim_host_port_reset.o \
./common/core/src/ux_hcd_sim_host_port_status_get.o \
./common/core/src/ux_hcd_sim_host_regular_td_obtain.o \
./common/core/src/ux_hcd_sim_host_request_bulk_transfer.o \
./common/core/src/ux_hcd_sim_host_request_control_transfer.o \
./common/core/src/ux_hcd_sim_host_request_interupt_transfer.o \
./common/core/src/ux_hcd_sim_host_request_isochronous_transfer.o \
./common/core/src/ux_hcd_sim_host_request_transfer.o \
./common/core/src/ux_hcd_sim_host_timer_function.o \
./common/core/src/ux_hcd_sim_host_transaction_schedule.o \
./common/core/src/ux_hcd_sim_host_transfer_abort.o \
./common/core/src/ux_host_class_dpump_activate.o \
./common/core/src/ux_host_class_dpump_configure.o \
./common/core/src/ux_host_class_dpump_deactivate.o \
./common/core/src/ux_host_class_dpump_endpoints_get.o \
./common/core/src/ux_host_class_dpump_entry.o \
./common/core/src/ux_host_class_dpump_ioctl.o \
./common/core/src/ux_host_class_dpump_read.o \
./common/core/src/ux_host_class_dpump_write.o \
./common/core/src/ux_host_stack_bandwidth_check.o \
./common/core/src/ux_host_stack_bandwidth_claim.o \
./common/core/src/ux_host_stack_bandwidth_release.o \
./common/core/src/ux_host_stack_class_call.o \
./common/core/src/ux_host_stack_class_device_scan.o \
./common/core/src/ux_host_stack_class_get.o \
./common/core/src/ux_host_stack_class_instance_create.o \
./common/core/src/ux_host_stack_class_instance_destroy.o \
./common/core/src/ux_host_stack_class_instance_get.o \
./common/core/src/ux_host_stack_class_instance_verify.o \
./common/core/src/ux_host_stack_class_interface_scan.o \
./common/core/src/ux_host_stack_class_register.o \
./common/core/src/ux_host_stack_class_unregister.o \
./common/core/src/ux_host_stack_configuration_descriptor_parse.o \
./common/core/src/ux_host_stack_configuration_enumerate.o \
./common/core/src/ux_host_stack_configuration_instance_create.o \
./common/core/src/ux_host_stack_configuration_instance_delete.o \
./common/core/src/ux_host_stack_configuration_interface_get.o \
./common/core/src/ux_host_stack_configuration_set.o \
./common/core/src/ux_host_stack_delay_ms.o \
./common/core/src/ux_host_stack_device_address_set.o \
./common/core/src/ux_host_stack_device_configuration_get.o \
./common/core/src/ux_host_stack_device_configuration_reset.o \
./common/core/src/ux_host_stack_device_configuration_select.o \
./common/core/src/ux_host_stack_device_descriptor_read.o \
./common/core/src/ux_host_stack_device_get.o \
./common/core/src/ux_host_stack_device_remove.o \
./common/core/src/ux_host_stack_device_resources_free.o \
./common/core/src/ux_host_stack_endpoint_instance_create.o \
./common/core/src/ux_host_stack_endpoint_instance_delete.o \
./common/core/src/ux_host_stack_endpoint_reset.o \
./common/core/src/ux_host_stack_endpoint_transfer_abort.o \
./common/core/src/ux_host_stack_enum_thread_entry.o \
./common/core/src/ux_host_stack_hcd_register.o \
./common/core/src/ux_host_stack_hcd_thread_entry.o \
./common/core/src/ux_host_stack_hcd_transfer_request.o \
./common/core/src/ux_host_stack_hcd_unregister.o \
./common/core/src/ux_host_stack_hnp_polling_thread_entry.o \
./common/core/src/ux_host_stack_initialize.o \
./common/core/src/ux_host_stack_interface_endpoint_get.o \
./common/core/src/ux_host_stack_interface_instance_create.o \
./common/core/src/ux_host_stack_interface_instance_delete.o \
./common/core/src/ux_host_stack_interface_set.o \
./common/core/src/ux_host_stack_interface_setting_select.o \
./common/core/src/ux_host_stack_interfaces_scan.o \
./common/core/src/ux_host_stack_new_configuration_create.o \
./common/core/src/ux_host_stack_new_device_create.o \
./common/core/src/ux_host_stack_new_device_get.o \
./common/core/src/ux_host_stack_new_endpoint_create.o \
./common/core/src/ux_host_stack_new_interface_create.o \
./common/core/src/ux_host_stack_rh_change_process.o \
./common/core/src/ux_host_stack_rh_device_extraction.o \
./common/core/src/ux_host_stack_rh_device_insertion.o \
./common/core/src/ux_host_stack_role_swap.o \
./common/core/src/ux_host_stack_transfer_request.o \
./common/core/src/ux_host_stack_transfer_request_abort.o \
./common/core/src/ux_host_stack_uninitialize.o \
./common/core/src/ux_system_error_handler.o \
./common/core/src/ux_system_initialize.o \
./common/core/src/ux_system_uninitialize.o \
./common/core/src/ux_trace_event_insert.o \
./common/core/src/ux_trace_event_update.o \
./common/core/src/ux_trace_object_register.o \
./common/core/src/ux_trace_object_unregister.o \
./common/core/src/ux_utility_debug_callback_register.o \
./common/core/src/ux_utility_debug_log.o \
./common/core/src/ux_utility_delay_ms.o \
./common/core/src/ux_utility_descriptor_pack.o \
./common/core/src/ux_utility_descriptor_parse.o \
./common/core/src/ux_utility_error_callback_register.o \
./common/core/src/ux_utility_event_flags_create.o \
./common/core/src/ux_utility_event_flags_delete.o \
./common/core/src/ux_utility_event_flags_get.o \
./common/core/src/ux_utility_event_flags_set.o \
./common/core/src/ux_utility_long_get.o \
./common/core/src/ux_utility_long_get_big_endian.o \
./common/core/src/ux_utility_long_put.o \
./common/core/src/ux_utility_long_put_big_endian.o \
./common/core/src/ux_utility_memory_allocate.o \
./common/core/src/ux_utility_memory_allocate_add_safe.o \
./common/core/src/ux_utility_memory_allocate_mulc_safe.o \
./common/core/src/ux_utility_memory_allocate_mulv_safe.o \
./common/core/src/ux_utility_memory_compare.o \
./common/core/src/ux_utility_memory_copy.o \
./common/core/src/ux_utility_memory_free.o \
./common/core/src/ux_utility_memory_free_block_best_get.o \
./common/core/src/ux_utility_memory_set.o \
./common/core/src/ux_utility_mutex_create.o \
./common/core/src/ux_utility_mutex_delete.o \
./common/core/src/ux_utility_mutex_off.o \
./common/core/src/ux_utility_mutex_on.o \
./common/core/src/ux_utility_pci_class_scan.o \
./common/core/src/ux_utility_pci_read.o \
./common/core/src/ux_utility_pci_write.o \
./common/core/src/ux_utility_physical_address.o \
./common/core/src/ux_utility_semaphore_create.o \
./common/core/src/ux_utility_semaphore_delete.o \
./common/core/src/ux_utility_semaphore_get.o \
./common/core/src/ux_utility_semaphore_put.o \
./common/core/src/ux_utility_set_interrupt_handler.o \
./common/core/src/ux_utility_short_get.o \
./common/core/src/ux_utility_short_get_big_endian.o \
./common/core/src/ux_utility_short_put.o \
./common/core/src/ux_utility_short_put_big_endian.o \
./common/core/src/ux_utility_string_length_check.o \
./common/core/src/ux_utility_string_length_get.o \
./common/core/src/ux_utility_string_to_unicode.o \
./common/core/src/ux_utility_thread_create.o \
./common/core/src/ux_utility_thread_delete.o \
./common/core/src/ux_utility_thread_identify.o \
./common/core/src/ux_utility_thread_relinquish.o \
./common/core/src/ux_utility_thread_resume.o \
./common/core/src/ux_utility_thread_schedule_other.o \
./common/core/src/ux_utility_thread_sleep.o \
./common/core/src/ux_utility_thread_suspend.o \
./common/core/src/ux_utility_timer_create.o \
./common/core/src/ux_utility_timer_delete.o \
./common/core/src/ux_utility_unicode_to_string.o \
./common/core/src/ux_utility_virtual_address.o 


# Each subdirectory must supply rules for building sources it contributes
common/core/src/%.o: ../common/core/src/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=1000 -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\filex\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\azure\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\core\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\threadx\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_host_classes\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_device_classes\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_device_controllers" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_host_controllers" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_network\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_pictbridge\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\arm\cmsis\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\compiler\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration\os_abstraction\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration\sc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\graphics\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\console\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\iodefines" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\iobitmasks" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\adc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\intc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\ostm\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_cache\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_ceu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_dmac\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_eeprom\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_i2c\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_jcu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_l2cc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_led\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_rspi\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_rtc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_sci\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_ssif\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_switch\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_vdc_vdec\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_wdt\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
