################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/usbx_device_classes/src/ux_device_class_audio10_control_process.c \
../common/usbx_device_classes/src/ux_device_class_audio20_control_process.c \
../common/usbx_device_classes/src/ux_device_class_audio_activate.c \
../common/usbx_device_classes/src/ux_device_class_audio_change.c \
../common/usbx_device_classes/src/ux_device_class_audio_control_request.c \
../common/usbx_device_classes/src/ux_device_class_audio_deactivate.c \
../common/usbx_device_classes/src/ux_device_class_audio_entry.c \
../common/usbx_device_classes/src/ux_device_class_audio_frame_write.c \
../common/usbx_device_classes/src/ux_device_class_audio_initialize.c \
../common/usbx_device_classes/src/ux_device_class_audio_ioctl.c \
../common/usbx_device_classes/src/ux_device_class_audio_read_frame_free.c \
../common/usbx_device_classes/src/ux_device_class_audio_read_frame_get.c \
../common/usbx_device_classes/src/ux_device_class_audio_read_thread_entry.c \
../common/usbx_device_classes/src/ux_device_class_audio_reception_start.c \
../common/usbx_device_classes/src/ux_device_class_audio_sample_read16.c \
../common/usbx_device_classes/src/ux_device_class_audio_sample_read24.c \
../common/usbx_device_classes/src/ux_device_class_audio_sample_read32.c \
../common/usbx_device_classes/src/ux_device_class_audio_sample_read8.c \
../common/usbx_device_classes/src/ux_device_class_audio_stream_get.c \
../common/usbx_device_classes/src/ux_device_class_audio_transmission_start.c \
../common/usbx_device_classes/src/ux_device_class_audio_unitialize.c \
../common/usbx_device_classes/src/ux_device_class_audio_write_frame_commit.c \
../common/usbx_device_classes/src/ux_device_class_audio_write_frame_get.c \
../common/usbx_device_classes/src/ux_device_class_audio_write_thread_entry.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_activate.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_bulkin_thread.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_bulkout_thread.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_control_request.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_deactivate.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_entry.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_initialize.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_ioctl.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_read.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_unitialize.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_write.c \
../common/usbx_device_classes/src/ux_device_class_cdc_acm_write_with_callback.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_activate.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_bulkin_thread.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_bulkout_thread.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_change.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_control_request.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_deactivate.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_entry.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_initialize.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_interrupt_thread.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_uninitialize.c \
../common/usbx_device_classes/src/ux_device_class_cdc_ecm_write.c \
../common/usbx_device_classes/src/ux_device_class_dfu_activate.c \
../common/usbx_device_classes/src/ux_device_class_dfu_control_request.c \
../common/usbx_device_classes/src/ux_device_class_dfu_deactivate.c \
../common/usbx_device_classes/src/ux_device_class_dfu_entry.c \
../common/usbx_device_classes/src/ux_device_class_dfu_initialize.c \
../common/usbx_device_classes/src/ux_device_class_dfu_thread.c \
../common/usbx_device_classes/src/ux_device_class_hid_activate.c \
../common/usbx_device_classes/src/ux_device_class_hid_control_request.c \
../common/usbx_device_classes/src/ux_device_class_hid_deactivate.c \
../common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.c \
../common/usbx_device_classes/src/ux_device_class_hid_entry.c \
../common/usbx_device_classes/src/ux_device_class_hid_event_get.c \
../common/usbx_device_classes/src/ux_device_class_hid_event_set.c \
../common/usbx_device_classes/src/ux_device_class_hid_initialize.c \
../common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.c \
../common/usbx_device_classes/src/ux_device_class_hid_report_get.c \
../common/usbx_device_classes/src/ux_device_class_hid_report_set.c \
../common/usbx_device_classes/src/ux_device_class_hid_uninitialize.c \
../common/usbx_device_classes/src/ux_device_class_pima_activate.c \
../common/usbx_device_classes/src/ux_device_class_pima_control_request.c \
../common/usbx_device_classes/src/ux_device_class_pima_data.c \
../common/usbx_device_classes/src/ux_device_class_pima_deactivate.c \
../common/usbx_device_classes/src/ux_device_class_pima_device_info_send.c \
../common/usbx_device_classes/src/ux_device_class_pima_device_prop_desc_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_device_prop_value_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_device_prop_value_set.c \
../common/usbx_device_classes/src/ux_device_class_pima_device_reset.c \
../common/usbx_device_classes/src/ux_device_class_pima_entry.c \
../common/usbx_device_classes/src/ux_device_class_pima_event_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_event_set.c \
../common/usbx_device_classes/src/ux_device_class_pima_initialize.c \
../common/usbx_device_classes/src/ux_device_class_pima_interrupt_thread.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_add.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_data_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_data_send.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_delete.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_handles_send.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_info_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_info_send.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_prop_desc_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_prop_value_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_prop_value_set.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_props_supported_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_references_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_object_references_set.c \
../common/usbx_device_classes/src/ux_device_class_pima_objects_number_send.c \
../common/usbx_device_classes/src/ux_device_class_pima_partial_object_data_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_response_send.c \
../common/usbx_device_classes/src/ux_device_class_pima_storage_format.c \
../common/usbx_device_classes/src/ux_device_class_pima_storage_id_send.c \
../common/usbx_device_classes/src/ux_device_class_pima_storage_info_get.c \
../common/usbx_device_classes/src/ux_device_class_pima_thread.c \
../common/usbx_device_classes/src/ux_device_class_rndis_activate.c \
../common/usbx_device_classes/src/ux_device_class_rndis_bulkin_thread.c \
../common/usbx_device_classes/src/ux_device_class_rndis_bulkout_thread.c \
../common/usbx_device_classes/src/ux_device_class_rndis_control_request.c \
../common/usbx_device_classes/src/ux_device_class_rndis_deactivate.c \
../common/usbx_device_classes/src/ux_device_class_rndis_entry.c \
../common/usbx_device_classes/src/ux_device_class_rndis_initialize.c \
../common/usbx_device_classes/src/ux_device_class_rndis_interrupt_thread.c \
../common/usbx_device_classes/src/ux_device_class_rndis_msg_initialize.c \
../common/usbx_device_classes/src/ux_device_class_rndis_msg_keep_alive.c \
../common/usbx_device_classes/src/ux_device_class_rndis_msg_query.c \
../common/usbx_device_classes/src/ux_device_class_rndis_msg_reset.c \
../common/usbx_device_classes/src/ux_device_class_rndis_msg_set.c \
../common/usbx_device_classes/src/ux_device_class_rndis_write.c \
../common/usbx_device_classes/src/ux_device_class_storage_activate.c \
../common/usbx_device_classes/src/ux_device_class_storage_control_request.c \
../common/usbx_device_classes/src/ux_device_class_storage_csw_send.c \
../common/usbx_device_classes/src/ux_device_class_storage_deactivate.c \
../common/usbx_device_classes/src/ux_device_class_storage_entry.c \
../common/usbx_device_classes/src/ux_device_class_storage_format.c \
../common/usbx_device_classes/src/ux_device_class_storage_get_configuration.c \
../common/usbx_device_classes/src/ux_device_class_storage_get_performance.c \
../common/usbx_device_classes/src/ux_device_class_storage_get_status_notification.c \
../common/usbx_device_classes/src/ux_device_class_storage_initialize.c \
../common/usbx_device_classes/src/ux_device_class_storage_inquiry.c \
../common/usbx_device_classes/src/ux_device_class_storage_mode_select.c \
../common/usbx_device_classes/src/ux_device_class_storage_mode_sense.c \
../common/usbx_device_classes/src/ux_device_class_storage_prevent_allow_media_removal.c \
../common/usbx_device_classes/src/ux_device_class_storage_read.c \
../common/usbx_device_classes/src/ux_device_class_storage_read_capacity.c \
../common/usbx_device_classes/src/ux_device_class_storage_read_disk_information.c \
../common/usbx_device_classes/src/ux_device_class_storage_read_dvd_structure.c \
../common/usbx_device_classes/src/ux_device_class_storage_read_format_capacity.c \
../common/usbx_device_classes/src/ux_device_class_storage_read_toc.c \
../common/usbx_device_classes/src/ux_device_class_storage_report_key.c \
../common/usbx_device_classes/src/ux_device_class_storage_request_sense.c \
../common/usbx_device_classes/src/ux_device_class_storage_start_stop.c \
../common/usbx_device_classes/src/ux_device_class_storage_synchronize_cache.c \
../common/usbx_device_classes/src/ux_device_class_storage_test_ready.c \
../common/usbx_device_classes/src/ux_device_class_storage_thread.c \
../common/usbx_device_classes/src/ux_device_class_storage_uninitialize.c \
../common/usbx_device_classes/src/ux_device_class_storage_verify.c \
../common/usbx_device_classes/src/ux_device_class_storage_write.c 

C_DEPS += \
./common/usbx_device_classes/src/ux_device_class_audio10_control_process.d \
./common/usbx_device_classes/src/ux_device_class_audio20_control_process.d \
./common/usbx_device_classes/src/ux_device_class_audio_activate.d \
./common/usbx_device_classes/src/ux_device_class_audio_change.d \
./common/usbx_device_classes/src/ux_device_class_audio_control_request.d \
./common/usbx_device_classes/src/ux_device_class_audio_deactivate.d \
./common/usbx_device_classes/src/ux_device_class_audio_entry.d \
./common/usbx_device_classes/src/ux_device_class_audio_frame_write.d \
./common/usbx_device_classes/src/ux_device_class_audio_initialize.d \
./common/usbx_device_classes/src/ux_device_class_audio_ioctl.d \
./common/usbx_device_classes/src/ux_device_class_audio_read_frame_free.d \
./common/usbx_device_classes/src/ux_device_class_audio_read_frame_get.d \
./common/usbx_device_classes/src/ux_device_class_audio_read_thread_entry.d \
./common/usbx_device_classes/src/ux_device_class_audio_reception_start.d \
./common/usbx_device_classes/src/ux_device_class_audio_sample_read16.d \
./common/usbx_device_classes/src/ux_device_class_audio_sample_read24.d \
./common/usbx_device_classes/src/ux_device_class_audio_sample_read32.d \
./common/usbx_device_classes/src/ux_device_class_audio_sample_read8.d \
./common/usbx_device_classes/src/ux_device_class_audio_stream_get.d \
./common/usbx_device_classes/src/ux_device_class_audio_transmission_start.d \
./common/usbx_device_classes/src/ux_device_class_audio_unitialize.d \
./common/usbx_device_classes/src/ux_device_class_audio_write_frame_commit.d \
./common/usbx_device_classes/src/ux_device_class_audio_write_frame_get.d \
./common/usbx_device_classes/src/ux_device_class_audio_write_thread_entry.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_activate.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_bulkin_thread.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_bulkout_thread.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_control_request.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_deactivate.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_entry.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_initialize.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_ioctl.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_read.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_unitialize.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_write.d \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_write_with_callback.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_activate.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_bulkin_thread.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_bulkout_thread.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_change.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_control_request.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_deactivate.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_entry.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_initialize.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_interrupt_thread.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_uninitialize.d \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_write.d \
./common/usbx_device_classes/src/ux_device_class_dfu_activate.d \
./common/usbx_device_classes/src/ux_device_class_dfu_control_request.d \
./common/usbx_device_classes/src/ux_device_class_dfu_deactivate.d \
./common/usbx_device_classes/src/ux_device_class_dfu_entry.d \
./common/usbx_device_classes/src/ux_device_class_dfu_initialize.d \
./common/usbx_device_classes/src/ux_device_class_dfu_thread.d \
./common/usbx_device_classes/src/ux_device_class_hid_activate.d \
./common/usbx_device_classes/src/ux_device_class_hid_control_request.d \
./common/usbx_device_classes/src/ux_device_class_hid_deactivate.d \
./common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.d \
./common/usbx_device_classes/src/ux_device_class_hid_entry.d \
./common/usbx_device_classes/src/ux_device_class_hid_event_get.d \
./common/usbx_device_classes/src/ux_device_class_hid_event_set.d \
./common/usbx_device_classes/src/ux_device_class_hid_initialize.d \
./common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.d \
./common/usbx_device_classes/src/ux_device_class_hid_report_get.d \
./common/usbx_device_classes/src/ux_device_class_hid_report_set.d \
./common/usbx_device_classes/src/ux_device_class_hid_uninitialize.d \
./common/usbx_device_classes/src/ux_device_class_pima_activate.d \
./common/usbx_device_classes/src/ux_device_class_pima_control_request.d \
./common/usbx_device_classes/src/ux_device_class_pima_data.d \
./common/usbx_device_classes/src/ux_device_class_pima_deactivate.d \
./common/usbx_device_classes/src/ux_device_class_pima_device_info_send.d \
./common/usbx_device_classes/src/ux_device_class_pima_device_prop_desc_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_device_prop_value_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_device_prop_value_set.d \
./common/usbx_device_classes/src/ux_device_class_pima_device_reset.d \
./common/usbx_device_classes/src/ux_device_class_pima_entry.d \
./common/usbx_device_classes/src/ux_device_class_pima_event_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_event_set.d \
./common/usbx_device_classes/src/ux_device_class_pima_initialize.d \
./common/usbx_device_classes/src/ux_device_class_pima_interrupt_thread.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_add.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_data_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_data_send.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_delete.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_handles_send.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_info_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_info_send.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_prop_desc_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_prop_value_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_prop_value_set.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_props_supported_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_references_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_object_references_set.d \
./common/usbx_device_classes/src/ux_device_class_pima_objects_number_send.d \
./common/usbx_device_classes/src/ux_device_class_pima_partial_object_data_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_response_send.d \
./common/usbx_device_classes/src/ux_device_class_pima_storage_format.d \
./common/usbx_device_classes/src/ux_device_class_pima_storage_id_send.d \
./common/usbx_device_classes/src/ux_device_class_pima_storage_info_get.d \
./common/usbx_device_classes/src/ux_device_class_pima_thread.d \
./common/usbx_device_classes/src/ux_device_class_rndis_activate.d \
./common/usbx_device_classes/src/ux_device_class_rndis_bulkin_thread.d \
./common/usbx_device_classes/src/ux_device_class_rndis_bulkout_thread.d \
./common/usbx_device_classes/src/ux_device_class_rndis_control_request.d \
./common/usbx_device_classes/src/ux_device_class_rndis_deactivate.d \
./common/usbx_device_classes/src/ux_device_class_rndis_entry.d \
./common/usbx_device_classes/src/ux_device_class_rndis_initialize.d \
./common/usbx_device_classes/src/ux_device_class_rndis_interrupt_thread.d \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_initialize.d \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_keep_alive.d \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_query.d \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_reset.d \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_set.d \
./common/usbx_device_classes/src/ux_device_class_rndis_write.d \
./common/usbx_device_classes/src/ux_device_class_storage_activate.d \
./common/usbx_device_classes/src/ux_device_class_storage_control_request.d \
./common/usbx_device_classes/src/ux_device_class_storage_csw_send.d \
./common/usbx_device_classes/src/ux_device_class_storage_deactivate.d \
./common/usbx_device_classes/src/ux_device_class_storage_entry.d \
./common/usbx_device_classes/src/ux_device_class_storage_format.d \
./common/usbx_device_classes/src/ux_device_class_storage_get_configuration.d \
./common/usbx_device_classes/src/ux_device_class_storage_get_performance.d \
./common/usbx_device_classes/src/ux_device_class_storage_get_status_notification.d \
./common/usbx_device_classes/src/ux_device_class_storage_initialize.d \
./common/usbx_device_classes/src/ux_device_class_storage_inquiry.d \
./common/usbx_device_classes/src/ux_device_class_storage_mode_select.d \
./common/usbx_device_classes/src/ux_device_class_storage_mode_sense.d \
./common/usbx_device_classes/src/ux_device_class_storage_prevent_allow_media_removal.d \
./common/usbx_device_classes/src/ux_device_class_storage_read.d \
./common/usbx_device_classes/src/ux_device_class_storage_read_capacity.d \
./common/usbx_device_classes/src/ux_device_class_storage_read_disk_information.d \
./common/usbx_device_classes/src/ux_device_class_storage_read_dvd_structure.d \
./common/usbx_device_classes/src/ux_device_class_storage_read_format_capacity.d \
./common/usbx_device_classes/src/ux_device_class_storage_read_toc.d \
./common/usbx_device_classes/src/ux_device_class_storage_report_key.d \
./common/usbx_device_classes/src/ux_device_class_storage_request_sense.d \
./common/usbx_device_classes/src/ux_device_class_storage_start_stop.d \
./common/usbx_device_classes/src/ux_device_class_storage_synchronize_cache.d \
./common/usbx_device_classes/src/ux_device_class_storage_test_ready.d \
./common/usbx_device_classes/src/ux_device_class_storage_thread.d \
./common/usbx_device_classes/src/ux_device_class_storage_uninitialize.d \
./common/usbx_device_classes/src/ux_device_class_storage_verify.d \
./common/usbx_device_classes/src/ux_device_class_storage_write.d 

OBJS += \
./common/usbx_device_classes/src/ux_device_class_audio10_control_process.o \
./common/usbx_device_classes/src/ux_device_class_audio20_control_process.o \
./common/usbx_device_classes/src/ux_device_class_audio_activate.o \
./common/usbx_device_classes/src/ux_device_class_audio_change.o \
./common/usbx_device_classes/src/ux_device_class_audio_control_request.o \
./common/usbx_device_classes/src/ux_device_class_audio_deactivate.o \
./common/usbx_device_classes/src/ux_device_class_audio_entry.o \
./common/usbx_device_classes/src/ux_device_class_audio_frame_write.o \
./common/usbx_device_classes/src/ux_device_class_audio_initialize.o \
./common/usbx_device_classes/src/ux_device_class_audio_ioctl.o \
./common/usbx_device_classes/src/ux_device_class_audio_read_frame_free.o \
./common/usbx_device_classes/src/ux_device_class_audio_read_frame_get.o \
./common/usbx_device_classes/src/ux_device_class_audio_read_thread_entry.o \
./common/usbx_device_classes/src/ux_device_class_audio_reception_start.o \
./common/usbx_device_classes/src/ux_device_class_audio_sample_read16.o \
./common/usbx_device_classes/src/ux_device_class_audio_sample_read24.o \
./common/usbx_device_classes/src/ux_device_class_audio_sample_read32.o \
./common/usbx_device_classes/src/ux_device_class_audio_sample_read8.o \
./common/usbx_device_classes/src/ux_device_class_audio_stream_get.o \
./common/usbx_device_classes/src/ux_device_class_audio_transmission_start.o \
./common/usbx_device_classes/src/ux_device_class_audio_unitialize.o \
./common/usbx_device_classes/src/ux_device_class_audio_write_frame_commit.o \
./common/usbx_device_classes/src/ux_device_class_audio_write_frame_get.o \
./common/usbx_device_classes/src/ux_device_class_audio_write_thread_entry.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_activate.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_bulkin_thread.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_bulkout_thread.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_control_request.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_deactivate.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_entry.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_initialize.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_ioctl.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_read.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_unitialize.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_write.o \
./common/usbx_device_classes/src/ux_device_class_cdc_acm_write_with_callback.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_activate.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_bulkin_thread.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_bulkout_thread.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_change.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_control_request.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_deactivate.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_entry.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_initialize.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_interrupt_thread.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_uninitialize.o \
./common/usbx_device_classes/src/ux_device_class_cdc_ecm_write.o \
./common/usbx_device_classes/src/ux_device_class_dfu_activate.o \
./common/usbx_device_classes/src/ux_device_class_dfu_control_request.o \
./common/usbx_device_classes/src/ux_device_class_dfu_deactivate.o \
./common/usbx_device_classes/src/ux_device_class_dfu_entry.o \
./common/usbx_device_classes/src/ux_device_class_dfu_initialize.o \
./common/usbx_device_classes/src/ux_device_class_dfu_thread.o \
./common/usbx_device_classes/src/ux_device_class_hid_activate.o \
./common/usbx_device_classes/src/ux_device_class_hid_control_request.o \
./common/usbx_device_classes/src/ux_device_class_hid_deactivate.o \
./common/usbx_device_classes/src/ux_device_class_hid_descriptor_send.o \
./common/usbx_device_classes/src/ux_device_class_hid_entry.o \
./common/usbx_device_classes/src/ux_device_class_hid_event_get.o \
./common/usbx_device_classes/src/ux_device_class_hid_event_set.o \
./common/usbx_device_classes/src/ux_device_class_hid_initialize.o \
./common/usbx_device_classes/src/ux_device_class_hid_interrupt_thread.o \
./common/usbx_device_classes/src/ux_device_class_hid_report_get.o \
./common/usbx_device_classes/src/ux_device_class_hid_report_set.o \
./common/usbx_device_classes/src/ux_device_class_hid_uninitialize.o \
./common/usbx_device_classes/src/ux_device_class_pima_activate.o \
./common/usbx_device_classes/src/ux_device_class_pima_control_request.o \
./common/usbx_device_classes/src/ux_device_class_pima_data.o \
./common/usbx_device_classes/src/ux_device_class_pima_deactivate.o \
./common/usbx_device_classes/src/ux_device_class_pima_device_info_send.o \
./common/usbx_device_classes/src/ux_device_class_pima_device_prop_desc_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_device_prop_value_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_device_prop_value_set.o \
./common/usbx_device_classes/src/ux_device_class_pima_device_reset.o \
./common/usbx_device_classes/src/ux_device_class_pima_entry.o \
./common/usbx_device_classes/src/ux_device_class_pima_event_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_event_set.o \
./common/usbx_device_classes/src/ux_device_class_pima_initialize.o \
./common/usbx_device_classes/src/ux_device_class_pima_interrupt_thread.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_add.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_data_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_data_send.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_delete.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_handles_send.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_info_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_info_send.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_prop_desc_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_prop_value_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_prop_value_set.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_props_supported_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_references_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_object_references_set.o \
./common/usbx_device_classes/src/ux_device_class_pima_objects_number_send.o \
./common/usbx_device_classes/src/ux_device_class_pima_partial_object_data_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_response_send.o \
./common/usbx_device_classes/src/ux_device_class_pima_storage_format.o \
./common/usbx_device_classes/src/ux_device_class_pima_storage_id_send.o \
./common/usbx_device_classes/src/ux_device_class_pima_storage_info_get.o \
./common/usbx_device_classes/src/ux_device_class_pima_thread.o \
./common/usbx_device_classes/src/ux_device_class_rndis_activate.o \
./common/usbx_device_classes/src/ux_device_class_rndis_bulkin_thread.o \
./common/usbx_device_classes/src/ux_device_class_rndis_bulkout_thread.o \
./common/usbx_device_classes/src/ux_device_class_rndis_control_request.o \
./common/usbx_device_classes/src/ux_device_class_rndis_deactivate.o \
./common/usbx_device_classes/src/ux_device_class_rndis_entry.o \
./common/usbx_device_classes/src/ux_device_class_rndis_initialize.o \
./common/usbx_device_classes/src/ux_device_class_rndis_interrupt_thread.o \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_initialize.o \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_keep_alive.o \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_query.o \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_reset.o \
./common/usbx_device_classes/src/ux_device_class_rndis_msg_set.o \
./common/usbx_device_classes/src/ux_device_class_rndis_write.o \
./common/usbx_device_classes/src/ux_device_class_storage_activate.o \
./common/usbx_device_classes/src/ux_device_class_storage_control_request.o \
./common/usbx_device_classes/src/ux_device_class_storage_csw_send.o \
./common/usbx_device_classes/src/ux_device_class_storage_deactivate.o \
./common/usbx_device_classes/src/ux_device_class_storage_entry.o \
./common/usbx_device_classes/src/ux_device_class_storage_format.o \
./common/usbx_device_classes/src/ux_device_class_storage_get_configuration.o \
./common/usbx_device_classes/src/ux_device_class_storage_get_performance.o \
./common/usbx_device_classes/src/ux_device_class_storage_get_status_notification.o \
./common/usbx_device_classes/src/ux_device_class_storage_initialize.o \
./common/usbx_device_classes/src/ux_device_class_storage_inquiry.o \
./common/usbx_device_classes/src/ux_device_class_storage_mode_select.o \
./common/usbx_device_classes/src/ux_device_class_storage_mode_sense.o \
./common/usbx_device_classes/src/ux_device_class_storage_prevent_allow_media_removal.o \
./common/usbx_device_classes/src/ux_device_class_storage_read.o \
./common/usbx_device_classes/src/ux_device_class_storage_read_capacity.o \
./common/usbx_device_classes/src/ux_device_class_storage_read_disk_information.o \
./common/usbx_device_classes/src/ux_device_class_storage_read_dvd_structure.o \
./common/usbx_device_classes/src/ux_device_class_storage_read_format_capacity.o \
./common/usbx_device_classes/src/ux_device_class_storage_read_toc.o \
./common/usbx_device_classes/src/ux_device_class_storage_report_key.o \
./common/usbx_device_classes/src/ux_device_class_storage_request_sense.o \
./common/usbx_device_classes/src/ux_device_class_storage_start_stop.o \
./common/usbx_device_classes/src/ux_device_class_storage_synchronize_cache.o \
./common/usbx_device_classes/src/ux_device_class_storage_test_ready.o \
./common/usbx_device_classes/src/ux_device_class_storage_thread.o \
./common/usbx_device_classes/src/ux_device_class_storage_uninitialize.o \
./common/usbx_device_classes/src/ux_device_class_storage_verify.o \
./common/usbx_device_classes/src/ux_device_class_storage_write.o 


# Each subdirectory must supply rules for building sources it contributes
common/usbx_device_classes/src/%.o: ../common/usbx_device_classes/src/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=1000 -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\filex\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\azure\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\netxduo\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\core\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\threadx\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_host_classes\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_device_classes\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_device_controllers" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_host_controllers" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_network\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\common\usbx_pictbridge\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\usbx\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\arm\cmsis\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\compiler\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration\os_abstraction\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\configuration\sc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\graphics\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\console\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\iodefines" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\application\system\iobitmasks" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\adc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\intc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\ostm\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_cache\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_ceu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_dmac\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_eeprom\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_i2c\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_jcu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_l2cc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_led\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_rspi\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_rtc\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_sci\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_ssif\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_switch\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_vdc_vdec\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\updated\rz_bsp\src\renesas\drivers\r_wdt\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
