
/**************************************************************************/
/**                                                                       */
/** Mass storage transfer size                                            */
/**                                                                       */
/**   usb_size.h                                                          */
/**                                                                       */
/**************************************************************************/

/* Although this macro is defined in  ux_host_class_storage.h,
 * we redefine here to change value.
 * After changing, not only this project but also UsbX must be rebuild.
 */

#define UX_HOST_CLASS_STORAGE_MAX_TRANSFER_SIZE (1024)
