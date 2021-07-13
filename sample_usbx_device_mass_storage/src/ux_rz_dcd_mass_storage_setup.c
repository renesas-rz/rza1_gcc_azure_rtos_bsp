/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Device Mass Storage Setup for RZ                                 */ 
/**                                                                       */
/** This initialization function is for USBX Device Mass Storage          */ 
/** operation on the Renesas RZ processor.                                */
/**                                                                       */
/** This template assumes basic operation, any special configuration will */ 
/** require user modification or replacement of this file.                */ 
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

/* Include necessary system files.  */

#include "tx_api.h"
#include "ux_api.h"
#include "ux_device_class_storage.h"
#include "ux_dcd_rz.h"


UX_SLAVE_CLASS_STORAGE_PARAMETER    storage_parameter;


#define DEVICE_FRAMEWORK_LENGTH_FULL_SPEED 50
UCHAR device_framework_full_speed[] = { 

    /* Device descriptor */
        0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x08,
        0x81, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
        0x03, 0x01,                                      

    /* Configuration descriptor */
        0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0xc0,
        0x32, 

    /* Interface descriptor */
        0x09, 0x04, 0x00, 0x00, 0x02, 0x08, 0x06, 0x50,
        0x00,

    /* Endpoint descriptor (Bulk In) */
        0x07, 0x05, 0x81, 0x02, 0x40, 0x00, 0x00, 

    /* Endpoint descriptor (Bulk Out) */
        0x07, 0x05, 0x02, 0x02, 0x40, 0x00, 0x00

    };
    
    
#define DEVICE_FRAMEWORK_LENGTH_HIGH_SPEED 60
UCHAR device_framework_high_speed[] = { 

    /* Device descriptor */
        0x12, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40,
        0x81, 0x07, 0x00, 0x00, 0x01, 0x00, 0x01, 0x02,
        0x03, 0x01,                                      

    /* Device qualifier descriptor */
        0x0a, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40,
        0x01, 0x00,

    /* Configuration descriptor */
        0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0xc0,
        0x32, 

    /* Interface descriptor */
        0x09, 0x04, 0x00, 0x00, 0x02, 0x08, 0x06, 0x50,
        0x00,

    /* Endpoint descriptor (Bulk In) */
        0x07, 0x05, 0x81, 0x02, 0x00, 0x02, 0x00, 

    /* Endpoint descriptor (Bulk Out) */
        0x07, 0x05, 0x02, 0x02, 0x00, 0x02, 0x00

    };
    

    /* String Device Framework :
     Byte 0 and 1 : Word containing the language ID : 0x0904 for US
     Byte 2       : Byte containing the index of the descriptor
     Byte 3       : Byte containing the length of the descriptor string
    */
   
#define STRING_FRAMEWORK_LENGTH 38
UCHAR string_framework[] = { 

    /* Manufacturer string descriptor : Index 1 */
        0x09, 0x04, 0x01, 0x0c, 
        0x45, 0x78, 0x70, 0x72,0x65, 0x73, 0x20, 0x4c, 
        0x6f, 0x67, 0x69, 0x63,

    /* Product string descriptor : Index 2 */
        0x09, 0x04, 0x02, 0x0a, 
        0x46, 0x6c, 0x61, 0x73, 0x68, 0x20, 0x44, 0x69, 
        0x73, 0x6b,             

    /* Serial Number string descriptor : Index 3 */
        0x09, 0x04, 0x03, 0x04, 
        0x30, 0x30, 0x30, 0x31
    };


    /* Multiple languages are supported on the device, to add
       a language besides english, the unicode language code must
       be appended to the language_id_framework array and the length
       adjusted accordingly. */
#define LANGUAGE_ID_FRAMEWORK_LENGTH 2
UCHAR language_id_framework[] = { 

    /* English. */
        0x09, 0x04
    };


/* Define the memory area needed for USBX Device Mass Storage demo.  */

//UCHAR  _ux_rz_dcd_memory_area[64 * 1024] __attribute__ ((section(".RAM_regionCache")));     /* FIXME: This should be tuned based on memory used during testing.  */
UCHAR  _ux_rz_dcd_memory_area[64 * 1024];

/* Define the basic USBX Device Mass Storage initialization.  Must be called from a thread context.  */

UINT  ux_rz_dcd_mass_storage_setup(UINT (*dcd_initialize)(ULONG dcd_io),
                            UINT  media_lba,
                            UINT  (*media_read)(VOID *storage, ULONG lun, UCHAR * data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status),
                            UINT  (*media_write)(VOID *storage, ULONG lun, UCHAR * data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status),
                            UINT  (*media_status)(VOID *storage, ULONG lun, ULONG media_id, ULONG *media_status))
{

UINT    status;


    /* Initialize USBX memory.  */
    ux_system_initialize(_ux_rz_dcd_memory_area, sizeof(_ux_rz_dcd_memory_area), UX_NULL, 0);

    /* The code below is required for installing the device portion of USBX. 
       In this demo, DFU is possible and we have a call back for state change. */
    status =  _ux_device_stack_initialize(device_framework_high_speed, DEVICE_FRAMEWORK_LENGTH_HIGH_SPEED,
                                       device_framework_full_speed, DEVICE_FRAMEWORK_LENGTH_FULL_SPEED,
                                       string_framework, STRING_FRAMEWORK_LENGTH,
                                       language_id_framework, LANGUAGE_ID_FRAMEWORK_LENGTH,UX_NULL);
    if(status!=UX_SUCCESS)
        return(status);

    /* Store the number of LUN in this device storage instance.  */
    storage_parameter.ux_slave_class_storage_parameter_number_lun =  1;

    /* Initialize the storage class parameters for reading/writing to the Flash Disk.  */
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_last_lba        =  media_lba;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_block_length    =  512;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_type            =  0;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_removable_flag  =  0x80;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_read            =  media_read;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_write           =  media_write; 
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_status          =  media_status;

    /* Initilize the device storage class. The class is connected with interface 0 on configuration 1. */
    status =  _ux_device_stack_class_register(_ux_system_slave_class_storage_name, _ux_device_class_storage_entry, 
                                                1, 0, (VOID *)&storage_parameter);

    /* Check completion status.  */
    if (status != UX_SUCCESS)
        return(status);

    /* Initialize/start the USBX device driver.  */
    status =  (dcd_initialize)(UX_RZ_USB0_BASE);
    
    /* Check status.  */
    if (status != UX_SUCCESS)
        while(1);

    /* Return status.  */
    return(status);
}

