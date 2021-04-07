/**************************************************************************/
/**                                                                       */ 
/** USBX Device Mass Storage Example for the Renesas RZ RSK Board         */ 
/**                                                                       */
/** This example illustrates USBX Device Mass Storage operation, making   */ 
/** the RZ RSK board appear to the host machine as a USB Flash drive.     */ 
/**                                                                       */ 
/**************************************************************************/

/* Include necessary system files.  */

#include "tx_api.h"
#include "ux_api.h"
#include "fx_api.h"
#include "ux_dcd_rz.h"


/* Define constants.  */

#define UX_DEMO_STACK_SIZE          4096
#define UX_RAM_DISK_SIZE            (6 * 1024 * 1024 - 512)
#define UX_RAM_DISK_LAST_LBA        ((UX_RAM_DISK_SIZE / 512) -1)


/* Define global data structures.   */

TX_THREAD                           demo_thread;
ULONG                               demo_thread_counter;
ULONG                               demo_error_counter;
ULONG                               demo_media_status_counter;
ULONG                               demo_media_read_counter;
ULONG                               demo_media_write_counter;


/* Buffer for FileX RAM media initialization.  */

FX_MEDIA                            *media;
unsigned char                       buffer[512];
FX_MEDIA                            ram_disk;
CHAR                                ram_disk_memory[UX_RAM_DISK_SIZE];


/* Define local function prototypes.  */

void  demo_thread_entry(ULONG);
UINT  demo_device_state_change(ULONG event);
void  demo_thread_entry(ULONG);
UINT  demo_media_read(VOID *storage, ULONG lun, UCHAR * data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status);
UINT  demo_media_write(VOID *storage, ULONG lun, UCHAR * data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status);
UINT  demo_media_status(VOID *storage, ULONG lun, ULONG media_id, ULONG *media_status);

UINT  ux_rz_dcd_mass_storage_setup(UINT (*dcd_initialize)(ULONG dcd_io),
                            UINT  media_lba,
                            UINT  (*media_read)(VOID *storage, ULONG lun, UCHAR * data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status),
                            UINT  (*media_write)(VOID *storage, ULONG lun, UCHAR * data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status),
                            UINT  (*media_status)(VOID *storage, ULONG lun, ULONG media_id, ULONG *media_status));
VOID  _fx_ram_driver(FX_MEDIA *media_ptr);
void  hwsetup(void);


/* Define main entry point.  */

int main(void)
{

    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}



/* Define what the initial system looks like.  */

void    tx_application_define(void *first_unused_memory)
{

UCHAR   *memory_pointer;


    /* Initialize the free memory pointer */
    memory_pointer =  (UCHAR *)  first_unused_memory;

    /* Initialize FileX.  */
    fx_system_initialize();

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "USBX Mass Storage Demo", demo_thread_entry, 0,  
            memory_pointer, UX_DEMO_STACK_SIZE, 
            20, 20, 1, TX_AUTO_START);
}


void  demo_thread_entry(ULONG arg)
{

UINT    status;
    
    
    /* Format the ram drive. */
    status =  fx_media_format(&ram_disk, _fx_ram_driver, ram_disk_memory, buffer, 512, "RAM DISK", 2, 512, 0, UX_RAM_DISK_SIZE / 512, 512, 4, 1, 1);

    /* Check the media format status.  */
    if (status != FX_SUCCESS)
    {

        /* Increment error counter.  */
        demo_error_counter++;

        /* Error opening media.  Return to caller.  */
        return;
    }

    /* Open the ram_disk.  */
    status =  fx_media_open(&ram_disk, "RAM DISK", _fx_ram_driver, ram_disk_memory, buffer, 512);

    /* Check the media open status.  */
    if (status != FX_SUCCESS)
    {

        /* Increment error counter.  */
        demo_error_counter++;

        /* Error opening media.  Return to caller.  */
        return;
    }

    /* Setup USBX Device for Mass Storage operation.  */
    status =  ux_rz_dcd_mass_storage_setup(ux_dcd_rz_initialize, UX_RAM_DISK_LAST_LBA, demo_media_read, demo_media_write, demo_media_status);

    /* Check the USBX Device Mass Storage status.  */
    if (status != UX_SUCCESS)
    {

        /* Increment error counter.  */
        demo_error_counter++;

        /* Error setting up USBX Device Mass Storage.  Return to caller.  */
        return;
    }

    /* Now loop here and just increment a counter periodically.  */
    while(1)
    {

        /* Increment thread counter.   */
        demo_thread_counter++;
        
        /* Sleep.  */
        tx_thread_sleep(100);
    }
}


UINT    demo_media_status(VOID *storage, ULONG lun, ULONG media_id, ULONG *media_status)
{

    /* Increment the status request count.  */
    demo_media_status_counter++;

    /* Since the ATA drive never fails, this is just for demo only !!!! */
    return(UX_SUCCESS);
}


UINT    demo_media_read(VOID *storage, ULONG lun, UCHAR * data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status)
{

UINT    status;


    /* Increment the read request count.  */
    demo_media_read_counter++;

    /* Loop to read all blocks.  */
    while(number_blocks--)
    {
        status =  fx_media_read(&ram_disk, lba, data_pointer);
        data_pointer+=512;
        lba++;
    }

    /* Return status.  */
    return(status);
}

UINT    demo_media_write(VOID *storage, ULONG lun, UCHAR * data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status)
{

UINT    status;


    /* Increment the write request count.  */
    demo_media_write_counter++;

    /* Loop to write all blocks.  */
    while(number_blocks--)
    {
        status =  fx_media_write(&ram_disk,lba,data_pointer);
        data_pointer+=512;
        lba++;
    }        

    /* Return status.  */
    return(status);
}


