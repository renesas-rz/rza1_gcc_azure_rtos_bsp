################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/src/fx_directory_attributes_read.c \
../common/src/fx_directory_attributes_set.c \
../common/src/fx_directory_create.c \
../common/src/fx_directory_default_get.c \
../common/src/fx_directory_default_get_copy.c \
../common/src/fx_directory_default_set.c \
../common/src/fx_directory_delete.c \
../common/src/fx_directory_entry_read.c \
../common/src/fx_directory_entry_write.c \
../common/src/fx_directory_exFAT_entry_read.c \
../common/src/fx_directory_exFAT_entry_write.c \
../common/src/fx_directory_exFAT_free_search.c \
../common/src/fx_directory_exFAT_unicode_entry_write.c \
../common/src/fx_directory_first_entry_find.c \
../common/src/fx_directory_first_full_entry_find.c \
../common/src/fx_directory_free_search.c \
../common/src/fx_directory_information_get.c \
../common/src/fx_directory_local_path_clear.c \
../common/src/fx_directory_local_path_get.c \
../common/src/fx_directory_local_path_get_copy.c \
../common/src/fx_directory_local_path_restore.c \
../common/src/fx_directory_local_path_set.c \
../common/src/fx_directory_long_name_get.c \
../common/src/fx_directory_long_name_get_extended.c \
../common/src/fx_directory_name_extract.c \
../common/src/fx_directory_name_test.c \
../common/src/fx_directory_next_entry_find.c \
../common/src/fx_directory_next_full_entry_find.c \
../common/src/fx_directory_rename.c \
../common/src/fx_directory_search.c \
../common/src/fx_directory_short_name_get.c \
../common/src/fx_directory_short_name_get_extended.c \
../common/src/fx_fault_tolerant_add_FAT_log.c \
../common/src/fx_fault_tolerant_add_bitmap_log.c \
../common/src/fx_fault_tolerant_add_checksum_log.c \
../common/src/fx_fault_tolerant_add_dir_log.c \
../common/src/fx_fault_tolerant_apply_logs.c \
../common/src/fx_fault_tolerant_calculate_checksum.c \
../common/src/fx_fault_tolerant_cleanup_FAT_chain.c \
../common/src/fx_fault_tolerant_create_log_file.c \
../common/src/fx_fault_tolerant_enable.c \
../common/src/fx_fault_tolerant_read_FAT.c \
../common/src/fx_fault_tolerant_read_directory_sector.c \
../common/src/fx_fault_tolerant_read_log_file.c \
../common/src/fx_fault_tolerant_recover.c \
../common/src/fx_fault_tolerant_reset_log_file.c \
../common/src/fx_fault_tolerant_set_FAT_chain.c \
../common/src/fx_fault_tolerant_transaction_end.c \
../common/src/fx_fault_tolerant_transaction_fail.c \
../common/src/fx_fault_tolerant_transaction_start.c \
../common/src/fx_fault_tolerant_write_log_file.c \
../common/src/fx_file_allocate.c \
../common/src/fx_file_attributes_read.c \
../common/src/fx_file_attributes_set.c \
../common/src/fx_file_best_effort_allocate.c \
../common/src/fx_file_close.c \
../common/src/fx_file_create.c \
../common/src/fx_file_date_time_set.c \
../common/src/fx_file_delete.c \
../common/src/fx_file_extended_allocate.c \
../common/src/fx_file_extended_best_effort_allocate.c \
../common/src/fx_file_extended_relative_seek.c \
../common/src/fx_file_extended_seek.c \
../common/src/fx_file_extended_truncate.c \
../common/src/fx_file_extended_truncate_release.c \
../common/src/fx_file_open.c \
../common/src/fx_file_read.c \
../common/src/fx_file_relative_seek.c \
../common/src/fx_file_rename.c \
../common/src/fx_file_seek.c \
../common/src/fx_file_truncate.c \
../common/src/fx_file_truncate_release.c \
../common/src/fx_file_write.c \
../common/src/fx_file_write_notify_set.c \
../common/src/fx_media_abort.c \
../common/src/fx_media_boot_info_extract.c \
../common/src/fx_media_cache_invalidate.c \
../common/src/fx_media_check.c \
../common/src/fx_media_check_FAT_chain_check.c \
../common/src/fx_media_check_lost_cluster_check.c \
../common/src/fx_media_close.c \
../common/src/fx_media_close_notify_set.c \
../common/src/fx_media_exFAT_format.c \
../common/src/fx_media_extended_space_available.c \
../common/src/fx_media_flush.c \
../common/src/fx_media_format.c \
../common/src/fx_media_format_oem_name_set.c \
../common/src/fx_media_format_type_set.c \
../common/src/fx_media_format_volume_id_set.c \
../common/src/fx_media_open.c \
../common/src/fx_media_open_notify_set.c \
../common/src/fx_media_read.c \
../common/src/fx_media_space_available.c \
../common/src/fx_media_volume_get.c \
../common/src/fx_media_volume_get_extended.c \
../common/src/fx_media_volume_set.c \
../common/src/fx_media_write.c \
../common/src/fx_partition_offset_calculate.c \
../common/src/fx_ram_driver.c \
../common/src/fx_system_date_get.c \
../common/src/fx_system_date_set.c \
../common/src/fx_system_initialize.c \
../common/src/fx_system_time_get.c \
../common/src/fx_system_time_set.c \
../common/src/fx_system_timer_entry.c \
../common/src/fx_trace_event_insert.c \
../common/src/fx_trace_event_update.c \
../common/src/fx_trace_object_register.c \
../common/src/fx_trace_object_unregister.c \
../common/src/fx_unicode_directory_create.c \
../common/src/fx_unicode_directory_entry_change.c \
../common/src/fx_unicode_directory_entry_read.c \
../common/src/fx_unicode_directory_rename.c \
../common/src/fx_unicode_directory_search.c \
../common/src/fx_unicode_file_create.c \
../common/src/fx_unicode_file_rename.c \
../common/src/fx_unicode_length_get.c \
../common/src/fx_unicode_length_get_extended.c \
../common/src/fx_unicode_name_get.c \
../common/src/fx_unicode_name_get_extended.c \
../common/src/fx_unicode_short_name_get.c \
../common/src/fx_unicode_short_name_get_extended.c \
../common/src/fx_utility_16_unsigned_read.c \
../common/src/fx_utility_16_unsigned_write.c \
../common/src/fx_utility_32_unsigned_read.c \
../common/src/fx_utility_32_unsigned_write.c \
../common/src/fx_utility_64_unsigned_read.c \
../common/src/fx_utility_64_unsigned_write.c \
../common/src/fx_utility_FAT_entry_read.c \
../common/src/fx_utility_FAT_entry_write.c \
../common/src/fx_utility_FAT_flush.c \
../common/src/fx_utility_FAT_map_flush.c \
../common/src/fx_utility_FAT_sector_get.c \
../common/src/fx_utility_absolute_path_get.c \
../common/src/fx_utility_exFAT_allocate_new_cluster.c \
../common/src/fx_utility_exFAT_bitmap_cache_prepare.c \
../common/src/fx_utility_exFAT_bitmap_cache_update.c \
../common/src/fx_utility_exFAT_bitmap_flush.c \
../common/src/fx_utility_exFAT_bitmap_free_cluster_find.c \
../common/src/fx_utility_exFAT_bitmap_initialize.c \
../common/src/fx_utility_exFAT_bitmap_start_sector_get.c \
../common/src/fx_utility_exFAT_cluster_free.c \
../common/src/fx_utility_exFAT_cluster_state_get.c \
../common/src/fx_utility_exFAT_cluster_state_set.c \
../common/src/fx_utility_exFAT_geometry_check.c \
../common/src/fx_utility_exFAT_name_hash_get.c \
../common/src/fx_utility_exFAT_size_calculate.c \
../common/src/fx_utility_exFAT_system_area_checksum_verify.c \
../common/src/fx_utility_exFAT_system_area_checksum_write.c \
../common/src/fx_utility_exFAT_system_area_format.c \
../common/src/fx_utility_exFAT_system_sector_write.c \
../common/src/fx_utility_exFAT_unicode_name_hash_get.c \
../common/src/fx_utility_exFAT_upcase_table.c \
../common/src/fx_utility_logical_sector_cache_entry_read.c \
../common/src/fx_utility_logical_sector_flush.c \
../common/src/fx_utility_logical_sector_read.c \
../common/src/fx_utility_logical_sector_write.c \
../common/src/fx_utility_memory_copy.c \
../common/src/fx_utility_memory_set.c \
../common/src/fx_utility_string_length_get.c \
../common/src/fx_utility_token_length_get.c \
../common/src/fxe_directory_attributes_read.c \
../common/src/fxe_directory_attributes_set.c \
../common/src/fxe_directory_create.c \
../common/src/fxe_directory_default_get.c \
../common/src/fxe_directory_default_get_copy.c \
../common/src/fxe_directory_default_set.c \
../common/src/fxe_directory_delete.c \
../common/src/fxe_directory_first_entry_find.c \
../common/src/fxe_directory_first_full_entry_find.c \
../common/src/fxe_directory_information_get.c \
../common/src/fxe_directory_local_path_clear.c \
../common/src/fxe_directory_local_path_get.c \
../common/src/fxe_directory_local_path_get_copy.c \
../common/src/fxe_directory_local_path_restore.c \
../common/src/fxe_directory_local_path_set.c \
../common/src/fxe_directory_long_name_get.c \
../common/src/fxe_directory_long_name_get_extended.c \
../common/src/fxe_directory_name_test.c \
../common/src/fxe_directory_next_entry_find.c \
../common/src/fxe_directory_next_full_entry_find.c \
../common/src/fxe_directory_rename.c \
../common/src/fxe_directory_short_name_get.c \
../common/src/fxe_directory_short_name_get_extended.c \
../common/src/fxe_fault_tolerant_enable.c \
../common/src/fxe_file_allocate.c \
../common/src/fxe_file_attributes_read.c \
../common/src/fxe_file_attributes_set.c \
../common/src/fxe_file_best_effort_allocate.c \
../common/src/fxe_file_close.c \
../common/src/fxe_file_create.c \
../common/src/fxe_file_date_time_set.c \
../common/src/fxe_file_delete.c \
../common/src/fxe_file_extended_allocate.c \
../common/src/fxe_file_extended_best_effort_allocate.c \
../common/src/fxe_file_extended_relative_seek.c \
../common/src/fxe_file_extended_seek.c \
../common/src/fxe_file_extended_truncate.c \
../common/src/fxe_file_extended_truncate_release.c \
../common/src/fxe_file_open.c \
../common/src/fxe_file_read.c \
../common/src/fxe_file_relative_seek.c \
../common/src/fxe_file_rename.c \
../common/src/fxe_file_seek.c \
../common/src/fxe_file_truncate.c \
../common/src/fxe_file_truncate_release.c \
../common/src/fxe_file_write.c \
../common/src/fxe_file_write_notify_set.c \
../common/src/fxe_media_abort.c \
../common/src/fxe_media_cache_invalidate.c \
../common/src/fxe_media_check.c \
../common/src/fxe_media_close.c \
../common/src/fxe_media_close_notify_set.c \
../common/src/fxe_media_exFAT_format.c \
../common/src/fxe_media_extended_space_available.c \
../common/src/fxe_media_flush.c \
../common/src/fxe_media_format.c \
../common/src/fxe_media_open.c \
../common/src/fxe_media_open_notify_set.c \
../common/src/fxe_media_read.c \
../common/src/fxe_media_space_available.c \
../common/src/fxe_media_volume_get.c \
../common/src/fxe_media_volume_get_extended.c \
../common/src/fxe_media_volume_set.c \
../common/src/fxe_media_write.c \
../common/src/fxe_system_date_get.c \
../common/src/fxe_system_date_set.c \
../common/src/fxe_system_time_get.c \
../common/src/fxe_system_time_set.c \
../common/src/fxe_unicode_directory_create.c \
../common/src/fxe_unicode_directory_rename.c \
../common/src/fxe_unicode_file_create.c \
../common/src/fxe_unicode_file_rename.c \
../common/src/fxe_unicode_name_get.c \
../common/src/fxe_unicode_name_get_extended.c \
../common/src/fxe_unicode_short_name_get.c \
../common/src/fxe_unicode_short_name_get_extended.c 

C_DEPS += \
./common/src/fx_directory_attributes_read.d \
./common/src/fx_directory_attributes_set.d \
./common/src/fx_directory_create.d \
./common/src/fx_directory_default_get.d \
./common/src/fx_directory_default_get_copy.d \
./common/src/fx_directory_default_set.d \
./common/src/fx_directory_delete.d \
./common/src/fx_directory_entry_read.d \
./common/src/fx_directory_entry_write.d \
./common/src/fx_directory_exFAT_entry_read.d \
./common/src/fx_directory_exFAT_entry_write.d \
./common/src/fx_directory_exFAT_free_search.d \
./common/src/fx_directory_exFAT_unicode_entry_write.d \
./common/src/fx_directory_first_entry_find.d \
./common/src/fx_directory_first_full_entry_find.d \
./common/src/fx_directory_free_search.d \
./common/src/fx_directory_information_get.d \
./common/src/fx_directory_local_path_clear.d \
./common/src/fx_directory_local_path_get.d \
./common/src/fx_directory_local_path_get_copy.d \
./common/src/fx_directory_local_path_restore.d \
./common/src/fx_directory_local_path_set.d \
./common/src/fx_directory_long_name_get.d \
./common/src/fx_directory_long_name_get_extended.d \
./common/src/fx_directory_name_extract.d \
./common/src/fx_directory_name_test.d \
./common/src/fx_directory_next_entry_find.d \
./common/src/fx_directory_next_full_entry_find.d \
./common/src/fx_directory_rename.d \
./common/src/fx_directory_search.d \
./common/src/fx_directory_short_name_get.d \
./common/src/fx_directory_short_name_get_extended.d \
./common/src/fx_fault_tolerant_add_FAT_log.d \
./common/src/fx_fault_tolerant_add_bitmap_log.d \
./common/src/fx_fault_tolerant_add_checksum_log.d \
./common/src/fx_fault_tolerant_add_dir_log.d \
./common/src/fx_fault_tolerant_apply_logs.d \
./common/src/fx_fault_tolerant_calculate_checksum.d \
./common/src/fx_fault_tolerant_cleanup_FAT_chain.d \
./common/src/fx_fault_tolerant_create_log_file.d \
./common/src/fx_fault_tolerant_enable.d \
./common/src/fx_fault_tolerant_read_FAT.d \
./common/src/fx_fault_tolerant_read_directory_sector.d \
./common/src/fx_fault_tolerant_read_log_file.d \
./common/src/fx_fault_tolerant_recover.d \
./common/src/fx_fault_tolerant_reset_log_file.d \
./common/src/fx_fault_tolerant_set_FAT_chain.d \
./common/src/fx_fault_tolerant_transaction_end.d \
./common/src/fx_fault_tolerant_transaction_fail.d \
./common/src/fx_fault_tolerant_transaction_start.d \
./common/src/fx_fault_tolerant_write_log_file.d \
./common/src/fx_file_allocate.d \
./common/src/fx_file_attributes_read.d \
./common/src/fx_file_attributes_set.d \
./common/src/fx_file_best_effort_allocate.d \
./common/src/fx_file_close.d \
./common/src/fx_file_create.d \
./common/src/fx_file_date_time_set.d \
./common/src/fx_file_delete.d \
./common/src/fx_file_extended_allocate.d \
./common/src/fx_file_extended_best_effort_allocate.d \
./common/src/fx_file_extended_relative_seek.d \
./common/src/fx_file_extended_seek.d \
./common/src/fx_file_extended_truncate.d \
./common/src/fx_file_extended_truncate_release.d \
./common/src/fx_file_open.d \
./common/src/fx_file_read.d \
./common/src/fx_file_relative_seek.d \
./common/src/fx_file_rename.d \
./common/src/fx_file_seek.d \
./common/src/fx_file_truncate.d \
./common/src/fx_file_truncate_release.d \
./common/src/fx_file_write.d \
./common/src/fx_file_write_notify_set.d \
./common/src/fx_media_abort.d \
./common/src/fx_media_boot_info_extract.d \
./common/src/fx_media_cache_invalidate.d \
./common/src/fx_media_check.d \
./common/src/fx_media_check_FAT_chain_check.d \
./common/src/fx_media_check_lost_cluster_check.d \
./common/src/fx_media_close.d \
./common/src/fx_media_close_notify_set.d \
./common/src/fx_media_exFAT_format.d \
./common/src/fx_media_extended_space_available.d \
./common/src/fx_media_flush.d \
./common/src/fx_media_format.d \
./common/src/fx_media_format_oem_name_set.d \
./common/src/fx_media_format_type_set.d \
./common/src/fx_media_format_volume_id_set.d \
./common/src/fx_media_open.d \
./common/src/fx_media_open_notify_set.d \
./common/src/fx_media_read.d \
./common/src/fx_media_space_available.d \
./common/src/fx_media_volume_get.d \
./common/src/fx_media_volume_get_extended.d \
./common/src/fx_media_volume_set.d \
./common/src/fx_media_write.d \
./common/src/fx_partition_offset_calculate.d \
./common/src/fx_ram_driver.d \
./common/src/fx_system_date_get.d \
./common/src/fx_system_date_set.d \
./common/src/fx_system_initialize.d \
./common/src/fx_system_time_get.d \
./common/src/fx_system_time_set.d \
./common/src/fx_system_timer_entry.d \
./common/src/fx_trace_event_insert.d \
./common/src/fx_trace_event_update.d \
./common/src/fx_trace_object_register.d \
./common/src/fx_trace_object_unregister.d \
./common/src/fx_unicode_directory_create.d \
./common/src/fx_unicode_directory_entry_change.d \
./common/src/fx_unicode_directory_entry_read.d \
./common/src/fx_unicode_directory_rename.d \
./common/src/fx_unicode_directory_search.d \
./common/src/fx_unicode_file_create.d \
./common/src/fx_unicode_file_rename.d \
./common/src/fx_unicode_length_get.d \
./common/src/fx_unicode_length_get_extended.d \
./common/src/fx_unicode_name_get.d \
./common/src/fx_unicode_name_get_extended.d \
./common/src/fx_unicode_short_name_get.d \
./common/src/fx_unicode_short_name_get_extended.d \
./common/src/fx_utility_16_unsigned_read.d \
./common/src/fx_utility_16_unsigned_write.d \
./common/src/fx_utility_32_unsigned_read.d \
./common/src/fx_utility_32_unsigned_write.d \
./common/src/fx_utility_64_unsigned_read.d \
./common/src/fx_utility_64_unsigned_write.d \
./common/src/fx_utility_FAT_entry_read.d \
./common/src/fx_utility_FAT_entry_write.d \
./common/src/fx_utility_FAT_flush.d \
./common/src/fx_utility_FAT_map_flush.d \
./common/src/fx_utility_FAT_sector_get.d \
./common/src/fx_utility_absolute_path_get.d \
./common/src/fx_utility_exFAT_allocate_new_cluster.d \
./common/src/fx_utility_exFAT_bitmap_cache_prepare.d \
./common/src/fx_utility_exFAT_bitmap_cache_update.d \
./common/src/fx_utility_exFAT_bitmap_flush.d \
./common/src/fx_utility_exFAT_bitmap_free_cluster_find.d \
./common/src/fx_utility_exFAT_bitmap_initialize.d \
./common/src/fx_utility_exFAT_bitmap_start_sector_get.d \
./common/src/fx_utility_exFAT_cluster_free.d \
./common/src/fx_utility_exFAT_cluster_state_get.d \
./common/src/fx_utility_exFAT_cluster_state_set.d \
./common/src/fx_utility_exFAT_geometry_check.d \
./common/src/fx_utility_exFAT_name_hash_get.d \
./common/src/fx_utility_exFAT_size_calculate.d \
./common/src/fx_utility_exFAT_system_area_checksum_verify.d \
./common/src/fx_utility_exFAT_system_area_checksum_write.d \
./common/src/fx_utility_exFAT_system_area_format.d \
./common/src/fx_utility_exFAT_system_sector_write.d \
./common/src/fx_utility_exFAT_unicode_name_hash_get.d \
./common/src/fx_utility_exFAT_upcase_table.d \
./common/src/fx_utility_logical_sector_cache_entry_read.d \
./common/src/fx_utility_logical_sector_flush.d \
./common/src/fx_utility_logical_sector_read.d \
./common/src/fx_utility_logical_sector_write.d \
./common/src/fx_utility_memory_copy.d \
./common/src/fx_utility_memory_set.d \
./common/src/fx_utility_string_length_get.d \
./common/src/fx_utility_token_length_get.d \
./common/src/fxe_directory_attributes_read.d \
./common/src/fxe_directory_attributes_set.d \
./common/src/fxe_directory_create.d \
./common/src/fxe_directory_default_get.d \
./common/src/fxe_directory_default_get_copy.d \
./common/src/fxe_directory_default_set.d \
./common/src/fxe_directory_delete.d \
./common/src/fxe_directory_first_entry_find.d \
./common/src/fxe_directory_first_full_entry_find.d \
./common/src/fxe_directory_information_get.d \
./common/src/fxe_directory_local_path_clear.d \
./common/src/fxe_directory_local_path_get.d \
./common/src/fxe_directory_local_path_get_copy.d \
./common/src/fxe_directory_local_path_restore.d \
./common/src/fxe_directory_local_path_set.d \
./common/src/fxe_directory_long_name_get.d \
./common/src/fxe_directory_long_name_get_extended.d \
./common/src/fxe_directory_name_test.d \
./common/src/fxe_directory_next_entry_find.d \
./common/src/fxe_directory_next_full_entry_find.d \
./common/src/fxe_directory_rename.d \
./common/src/fxe_directory_short_name_get.d \
./common/src/fxe_directory_short_name_get_extended.d \
./common/src/fxe_fault_tolerant_enable.d \
./common/src/fxe_file_allocate.d \
./common/src/fxe_file_attributes_read.d \
./common/src/fxe_file_attributes_set.d \
./common/src/fxe_file_best_effort_allocate.d \
./common/src/fxe_file_close.d \
./common/src/fxe_file_create.d \
./common/src/fxe_file_date_time_set.d \
./common/src/fxe_file_delete.d \
./common/src/fxe_file_extended_allocate.d \
./common/src/fxe_file_extended_best_effort_allocate.d \
./common/src/fxe_file_extended_relative_seek.d \
./common/src/fxe_file_extended_seek.d \
./common/src/fxe_file_extended_truncate.d \
./common/src/fxe_file_extended_truncate_release.d \
./common/src/fxe_file_open.d \
./common/src/fxe_file_read.d \
./common/src/fxe_file_relative_seek.d \
./common/src/fxe_file_rename.d \
./common/src/fxe_file_seek.d \
./common/src/fxe_file_truncate.d \
./common/src/fxe_file_truncate_release.d \
./common/src/fxe_file_write.d \
./common/src/fxe_file_write_notify_set.d \
./common/src/fxe_media_abort.d \
./common/src/fxe_media_cache_invalidate.d \
./common/src/fxe_media_check.d \
./common/src/fxe_media_close.d \
./common/src/fxe_media_close_notify_set.d \
./common/src/fxe_media_exFAT_format.d \
./common/src/fxe_media_extended_space_available.d \
./common/src/fxe_media_flush.d \
./common/src/fxe_media_format.d \
./common/src/fxe_media_open.d \
./common/src/fxe_media_open_notify_set.d \
./common/src/fxe_media_read.d \
./common/src/fxe_media_space_available.d \
./common/src/fxe_media_volume_get.d \
./common/src/fxe_media_volume_get_extended.d \
./common/src/fxe_media_volume_set.d \
./common/src/fxe_media_write.d \
./common/src/fxe_system_date_get.d \
./common/src/fxe_system_date_set.d \
./common/src/fxe_system_time_get.d \
./common/src/fxe_system_time_set.d \
./common/src/fxe_unicode_directory_create.d \
./common/src/fxe_unicode_directory_rename.d \
./common/src/fxe_unicode_file_create.d \
./common/src/fxe_unicode_file_rename.d \
./common/src/fxe_unicode_name_get.d \
./common/src/fxe_unicode_name_get_extended.d \
./common/src/fxe_unicode_short_name_get.d \
./common/src/fxe_unicode_short_name_get_extended.d 

OBJS += \
./common/src/fx_directory_attributes_read.o \
./common/src/fx_directory_attributes_set.o \
./common/src/fx_directory_create.o \
./common/src/fx_directory_default_get.o \
./common/src/fx_directory_default_get_copy.o \
./common/src/fx_directory_default_set.o \
./common/src/fx_directory_delete.o \
./common/src/fx_directory_entry_read.o \
./common/src/fx_directory_entry_write.o \
./common/src/fx_directory_exFAT_entry_read.o \
./common/src/fx_directory_exFAT_entry_write.o \
./common/src/fx_directory_exFAT_free_search.o \
./common/src/fx_directory_exFAT_unicode_entry_write.o \
./common/src/fx_directory_first_entry_find.o \
./common/src/fx_directory_first_full_entry_find.o \
./common/src/fx_directory_free_search.o \
./common/src/fx_directory_information_get.o \
./common/src/fx_directory_local_path_clear.o \
./common/src/fx_directory_local_path_get.o \
./common/src/fx_directory_local_path_get_copy.o \
./common/src/fx_directory_local_path_restore.o \
./common/src/fx_directory_local_path_set.o \
./common/src/fx_directory_long_name_get.o \
./common/src/fx_directory_long_name_get_extended.o \
./common/src/fx_directory_name_extract.o \
./common/src/fx_directory_name_test.o \
./common/src/fx_directory_next_entry_find.o \
./common/src/fx_directory_next_full_entry_find.o \
./common/src/fx_directory_rename.o \
./common/src/fx_directory_search.o \
./common/src/fx_directory_short_name_get.o \
./common/src/fx_directory_short_name_get_extended.o \
./common/src/fx_fault_tolerant_add_FAT_log.o \
./common/src/fx_fault_tolerant_add_bitmap_log.o \
./common/src/fx_fault_tolerant_add_checksum_log.o \
./common/src/fx_fault_tolerant_add_dir_log.o \
./common/src/fx_fault_tolerant_apply_logs.o \
./common/src/fx_fault_tolerant_calculate_checksum.o \
./common/src/fx_fault_tolerant_cleanup_FAT_chain.o \
./common/src/fx_fault_tolerant_create_log_file.o \
./common/src/fx_fault_tolerant_enable.o \
./common/src/fx_fault_tolerant_read_FAT.o \
./common/src/fx_fault_tolerant_read_directory_sector.o \
./common/src/fx_fault_tolerant_read_log_file.o \
./common/src/fx_fault_tolerant_recover.o \
./common/src/fx_fault_tolerant_reset_log_file.o \
./common/src/fx_fault_tolerant_set_FAT_chain.o \
./common/src/fx_fault_tolerant_transaction_end.o \
./common/src/fx_fault_tolerant_transaction_fail.o \
./common/src/fx_fault_tolerant_transaction_start.o \
./common/src/fx_fault_tolerant_write_log_file.o \
./common/src/fx_file_allocate.o \
./common/src/fx_file_attributes_read.o \
./common/src/fx_file_attributes_set.o \
./common/src/fx_file_best_effort_allocate.o \
./common/src/fx_file_close.o \
./common/src/fx_file_create.o \
./common/src/fx_file_date_time_set.o \
./common/src/fx_file_delete.o \
./common/src/fx_file_extended_allocate.o \
./common/src/fx_file_extended_best_effort_allocate.o \
./common/src/fx_file_extended_relative_seek.o \
./common/src/fx_file_extended_seek.o \
./common/src/fx_file_extended_truncate.o \
./common/src/fx_file_extended_truncate_release.o \
./common/src/fx_file_open.o \
./common/src/fx_file_read.o \
./common/src/fx_file_relative_seek.o \
./common/src/fx_file_rename.o \
./common/src/fx_file_seek.o \
./common/src/fx_file_truncate.o \
./common/src/fx_file_truncate_release.o \
./common/src/fx_file_write.o \
./common/src/fx_file_write_notify_set.o \
./common/src/fx_media_abort.o \
./common/src/fx_media_boot_info_extract.o \
./common/src/fx_media_cache_invalidate.o \
./common/src/fx_media_check.o \
./common/src/fx_media_check_FAT_chain_check.o \
./common/src/fx_media_check_lost_cluster_check.o \
./common/src/fx_media_close.o \
./common/src/fx_media_close_notify_set.o \
./common/src/fx_media_exFAT_format.o \
./common/src/fx_media_extended_space_available.o \
./common/src/fx_media_flush.o \
./common/src/fx_media_format.o \
./common/src/fx_media_format_oem_name_set.o \
./common/src/fx_media_format_type_set.o \
./common/src/fx_media_format_volume_id_set.o \
./common/src/fx_media_open.o \
./common/src/fx_media_open_notify_set.o \
./common/src/fx_media_read.o \
./common/src/fx_media_space_available.o \
./common/src/fx_media_volume_get.o \
./common/src/fx_media_volume_get_extended.o \
./common/src/fx_media_volume_set.o \
./common/src/fx_media_write.o \
./common/src/fx_partition_offset_calculate.o \
./common/src/fx_ram_driver.o \
./common/src/fx_system_date_get.o \
./common/src/fx_system_date_set.o \
./common/src/fx_system_initialize.o \
./common/src/fx_system_time_get.o \
./common/src/fx_system_time_set.o \
./common/src/fx_system_timer_entry.o \
./common/src/fx_trace_event_insert.o \
./common/src/fx_trace_event_update.o \
./common/src/fx_trace_object_register.o \
./common/src/fx_trace_object_unregister.o \
./common/src/fx_unicode_directory_create.o \
./common/src/fx_unicode_directory_entry_change.o \
./common/src/fx_unicode_directory_entry_read.o \
./common/src/fx_unicode_directory_rename.o \
./common/src/fx_unicode_directory_search.o \
./common/src/fx_unicode_file_create.o \
./common/src/fx_unicode_file_rename.o \
./common/src/fx_unicode_length_get.o \
./common/src/fx_unicode_length_get_extended.o \
./common/src/fx_unicode_name_get.o \
./common/src/fx_unicode_name_get_extended.o \
./common/src/fx_unicode_short_name_get.o \
./common/src/fx_unicode_short_name_get_extended.o \
./common/src/fx_utility_16_unsigned_read.o \
./common/src/fx_utility_16_unsigned_write.o \
./common/src/fx_utility_32_unsigned_read.o \
./common/src/fx_utility_32_unsigned_write.o \
./common/src/fx_utility_64_unsigned_read.o \
./common/src/fx_utility_64_unsigned_write.o \
./common/src/fx_utility_FAT_entry_read.o \
./common/src/fx_utility_FAT_entry_write.o \
./common/src/fx_utility_FAT_flush.o \
./common/src/fx_utility_FAT_map_flush.o \
./common/src/fx_utility_FAT_sector_get.o \
./common/src/fx_utility_absolute_path_get.o \
./common/src/fx_utility_exFAT_allocate_new_cluster.o \
./common/src/fx_utility_exFAT_bitmap_cache_prepare.o \
./common/src/fx_utility_exFAT_bitmap_cache_update.o \
./common/src/fx_utility_exFAT_bitmap_flush.o \
./common/src/fx_utility_exFAT_bitmap_free_cluster_find.o \
./common/src/fx_utility_exFAT_bitmap_initialize.o \
./common/src/fx_utility_exFAT_bitmap_start_sector_get.o \
./common/src/fx_utility_exFAT_cluster_free.o \
./common/src/fx_utility_exFAT_cluster_state_get.o \
./common/src/fx_utility_exFAT_cluster_state_set.o \
./common/src/fx_utility_exFAT_geometry_check.o \
./common/src/fx_utility_exFAT_name_hash_get.o \
./common/src/fx_utility_exFAT_size_calculate.o \
./common/src/fx_utility_exFAT_system_area_checksum_verify.o \
./common/src/fx_utility_exFAT_system_area_checksum_write.o \
./common/src/fx_utility_exFAT_system_area_format.o \
./common/src/fx_utility_exFAT_system_sector_write.o \
./common/src/fx_utility_exFAT_unicode_name_hash_get.o \
./common/src/fx_utility_exFAT_upcase_table.o \
./common/src/fx_utility_logical_sector_cache_entry_read.o \
./common/src/fx_utility_logical_sector_flush.o \
./common/src/fx_utility_logical_sector_read.o \
./common/src/fx_utility_logical_sector_write.o \
./common/src/fx_utility_memory_copy.o \
./common/src/fx_utility_memory_set.o \
./common/src/fx_utility_string_length_get.o \
./common/src/fx_utility_token_length_get.o \
./common/src/fxe_directory_attributes_read.o \
./common/src/fxe_directory_attributes_set.o \
./common/src/fxe_directory_create.o \
./common/src/fxe_directory_default_get.o \
./common/src/fxe_directory_default_get_copy.o \
./common/src/fxe_directory_default_set.o \
./common/src/fxe_directory_delete.o \
./common/src/fxe_directory_first_entry_find.o \
./common/src/fxe_directory_first_full_entry_find.o \
./common/src/fxe_directory_information_get.o \
./common/src/fxe_directory_local_path_clear.o \
./common/src/fxe_directory_local_path_get.o \
./common/src/fxe_directory_local_path_get_copy.o \
./common/src/fxe_directory_local_path_restore.o \
./common/src/fxe_directory_local_path_set.o \
./common/src/fxe_directory_long_name_get.o \
./common/src/fxe_directory_long_name_get_extended.o \
./common/src/fxe_directory_name_test.o \
./common/src/fxe_directory_next_entry_find.o \
./common/src/fxe_directory_next_full_entry_find.o \
./common/src/fxe_directory_rename.o \
./common/src/fxe_directory_short_name_get.o \
./common/src/fxe_directory_short_name_get_extended.o \
./common/src/fxe_fault_tolerant_enable.o \
./common/src/fxe_file_allocate.o \
./common/src/fxe_file_attributes_read.o \
./common/src/fxe_file_attributes_set.o \
./common/src/fxe_file_best_effort_allocate.o \
./common/src/fxe_file_close.o \
./common/src/fxe_file_create.o \
./common/src/fxe_file_date_time_set.o \
./common/src/fxe_file_delete.o \
./common/src/fxe_file_extended_allocate.o \
./common/src/fxe_file_extended_best_effort_allocate.o \
./common/src/fxe_file_extended_relative_seek.o \
./common/src/fxe_file_extended_seek.o \
./common/src/fxe_file_extended_truncate.o \
./common/src/fxe_file_extended_truncate_release.o \
./common/src/fxe_file_open.o \
./common/src/fxe_file_read.o \
./common/src/fxe_file_relative_seek.o \
./common/src/fxe_file_rename.o \
./common/src/fxe_file_seek.o \
./common/src/fxe_file_truncate.o \
./common/src/fxe_file_truncate_release.o \
./common/src/fxe_file_write.o \
./common/src/fxe_file_write_notify_set.o \
./common/src/fxe_media_abort.o \
./common/src/fxe_media_cache_invalidate.o \
./common/src/fxe_media_check.o \
./common/src/fxe_media_close.o \
./common/src/fxe_media_close_notify_set.o \
./common/src/fxe_media_exFAT_format.o \
./common/src/fxe_media_extended_space_available.o \
./common/src/fxe_media_flush.o \
./common/src/fxe_media_format.o \
./common/src/fxe_media_open.o \
./common/src/fxe_media_open_notify_set.o \
./common/src/fxe_media_read.o \
./common/src/fxe_media_space_available.o \
./common/src/fxe_media_volume_get.o \
./common/src/fxe_media_volume_get_extended.o \
./common/src/fxe_media_volume_set.o \
./common/src/fxe_media_write.o \
./common/src/fxe_system_date_get.o \
./common/src/fxe_system_date_set.o \
./common/src/fxe_system_time_get.o \
./common/src/fxe_system_time_set.o \
./common/src/fxe_unicode_directory_create.o \
./common/src/fxe_unicode_directory_rename.o \
./common/src/fxe_unicode_file_create.o \
./common/src/fxe_unicode_file_rename.o \
./common/src/fxe_unicode_name_get.o \
./common/src/fxe_unicode_name_get_extended.o \
./common/src/fxe_unicode_short_name_get.o \
./common/src/fxe_unicode_short_name_get_extended.o 


# Each subdirectory must supply rules for building sources it contributes
common/src/%.o: ../common/src/%.c
	arm-none-eabi-gcc -mcpu=cortex-a9 -march=armv7-a -marm -mlittle-endian -mfloat-abi=hard -mfpu=vfpv3-d16 -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wnull-dereference -g -Wstack-usage=1000 -I"C:\workspace\RZA1\rza1h_azure_gcc\e2studio\threadx\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\e2studio\threadx\ports\cortex_a9\gnu\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\e2studio\filex\common\inc" -I"C:\workspace\RZA1\rza1h_azure_gcc\e2studio\filex\ports\cortex_a9\gnu\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
