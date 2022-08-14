/**************************************************************************/
/**                                                                       */ 
/** USBX Host Mass Storage Example for the Renesas RZ RSK CPU Board       */ 
/**                                                                       */
/** This example illustrates USBX Host Mass Storage operation. A USB      */ 
/** flash drive can be and this demo will cycle through the directory     */ 
/** entries, reading the contents of any file in the root directory.      */ 
/**                                                                       */ 
/** Grape adds File reading and writing function using FileX functions.    */
/**                                                                       */
/**************************************************************************/

/* Include necessary system files.  */
#if 0 /* grape */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "compiler_settings.h"
#include "iodefine_cfg.h"
#include "control.h"
#endif

#include "tx_api.h"
#include "ux_api.h"
#include "fx_api.h"
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_host_class_storage.h"
#include "ux_hcd_rz.h"

#if 1	/* grape */
#include "usb_misc.h"
#include "usb_serial.h"
#include "filex_test.h"
#include "usb_mem.h"
#include "usb_dma.h"
#include "sample_connect.h"
#include "usb_size.h"	/* UX_HOST_CLASS_STORAGE_MAX_TRANSFER_SIZE */
#endif

/* Define constants.  */

#define UX_DEMO_STACK_SIZE      2048
#define UX_DEMO_BUFFER_SIZE     512         
#define UX_RZ_HC_USB_BASE       0x0

#if 1	/* grape */
/* If defined, mode for measuring reading/writing time.
 * If Not defined, mode for Simple demo to enumerate directories and files
 * reside in root directory.
 */
#if 1
#define SPEED_TEST
#endif
#endif

/* Define global data structures.   */

TX_THREAD                       demo_thread;
UX_HOST_CLASS                   *class_driver;
UX_HOST_CLASS_STORAGE           *storage;
UX_HOST_CLASS_STORAGE_MEDIA     *storage_media;
FX_MEDIA                        *media;
UINT                            status;
UINT                            transfer_completed;
ULONG                           requested_length;
FX_FILE                         my_file;
CHAR                            file_name[64];
UCHAR                           local_buffer[UX_DEMO_BUFFER_SIZE];


/* Define local function prototypes.  */

void  demo_thread_entry(ULONG);
UINT  _ux_hcd_rx_initialize(UX_HCD *hcd);
UINT  demo_host_change_function(ULONG event, UX_HOST_CLASS * class, VOID * instance);

void  hwsetup(void);


/******************************************************************************
* Function Name: void entry_find()
* Description  : Enumulate directories and files on root directory.
* Arguments    : None
* Return Value : None
******************************************************************************/
void entry_find();


/* Define main entry point.  */

int main(void)
{

	printf( "----------------------------------------\n");
	printf( "rw_memory address= 0x%08x\n", test_usb_rw_buff);
	printf( "sizeof(rw_memory)= %d bytes.\n", FX_MEM_SIZE);

#ifdef UX_RZ_HCD_USE_DMA
	printf( "transfer mode= DMA single\n" );
#else
	printf( "transfer mode= PIO\n" );
#endif

	printf( "STORAGE_TRANSFER_SIZE= %dB\n", UX_HOST_CLASS_STORAGE_MAX_TRANSFER_SIZE );

	/* Setup the RZ board.  */
//    hwsetup();

#ifdef UX_RZ_HCD_USE_DMA
    usb_dma_init();	/* grape */
#endif
    
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}


/* Define what the initial system looks like.  */

void    tx_application_define(void *first_unused_memory)
{

CHAR    *stack_pointer;
CHAR    *memory_pointer;
UINT    status;


    /* Initialize the free memory pointer */
    stack_pointer= (CHAR *)first_unused_memory;
    memory_pointer=stack_pointer + UX_DEMO_STACK_SIZE;

    /* Initialize FileX.  */
    fx_system_initialize();

    /* Initialize USBX memory.  */
    ux_system_initialize(memory_pointer, 0x10000, UX_NULL, 0);

    /* The code below is required for installing the host portion of USBX */
    status =  ux_host_stack_initialize(demo_host_change_function);
    if (status != UX_SUCCESS)
        return;

    /* Register all the host class drivers for this USBX implementation.  */
    status =  ux_host_stack_class_register(_ux_system_host_class_storage_name, _ux_host_class_storage_entry);
    if(status != UX_SUCCESS)
        return;

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "USBX Mass Storage Demo", demo_thread_entry, 0,  
            stack_pointer, UX_DEMO_STACK_SIZE, 
            20, 20, 1, TX_AUTO_START);
}


UINT demo_host_change_function(ULONG event, UX_HOST_CLASS * class, VOID * instance)
{

UINT            status;
UX_HOST_CLASS   *storage_class;


    /* Check if there is a device insertion.  */
    if (event == UX_DEVICE_INSERTION)
    {

        /* Get the storage class.  */
        status =  ux_host_stack_class_get(_ux_system_host_class_storage_name, &storage_class);
        if (status != UX_SUCCESS)
            return(status);

        /* Check if we got a storage class device.  */
        if (storage_class == class)
        {

            /* We only get the first media attached to the class container.  */
            if (media == UX_NULL)
            {

                storage = instance;
                storage_media = class -> ux_host_class_media;
                media = &storage_media -> ux_host_class_storage_media;
            }
        }
    }
    /* Check if some device is removed.  */
    else if(event == UX_DEVICE_REMOVAL)
    {

        /*  Check if the storage device is removed.  */
        if (instance == storage)
        {

            /* Set pointers to null, so that the demo thread will not try to access the media any more.  */
            media = UX_NULL;
            storage_media = UX_NULL;
            storage = UX_NULL;
        }
    }

    return(UX_SUCCESS);
}

void  demo_thread_entry(ULONG arg)
{
	int connect_count = 0;
	UINT status;

	printf("\nRZA1H USBx Host Mass Storage Demo\n");
	printf("tick is %d(ms).\n", TICK_MS);
    
    /* Register all the USB host controllers available in this system */
    status =  ux_host_stack_hcd_register("RZ-USB0", _ux_hcd_rz_initialize, UX_RZ_HC_USB_BASE,UX_RZ_CONTROLLER_RZ);
    if (status != UX_SUCCESS)
        while(1);


	while(1)
    {

#if 1	/* grape */
        printf( "\nwaiting USB memory is connected.\n");
    	wait_connect();
#else
        /* Check if we have an available media instance.  */
        while (media == UX_NULL)
        {

            tx_thread_relinquish();
        }
#endif

    	printf("\n--- %dth connection -------------\n", connect_count++);

#ifdef SPEED_TEST
    	printf("%dMB write read verify [speed_test]\n", MB(FX_MEM_SIZE));
    	status = speed_test();
    	if ( FX_SUCCESS != status) {
    		goto UNM;
    	}
#else
        entry_find();
#endif

UNM:
        printf( "\nwaiting USB memory is disconnected.\n");
    	wait_disconnect();
    }
}

void entry_find()
{
	FX_FILE     my_file;
	ULONG       files_count = 0;
	UINT        file_attribute;
	ULONG       error_count = 0;

	/* Find first file on media.  */
	status = fx_directory_first_entry_find(media, file_name);
	if (status != UX_SUCCESS) {
		printf("[%s] fx_directory_first_entry_find error, status= %s\n",
				__func__, get_fx_error_string(status));
		return;
	}

	/* We get here if there is at least a file in the directory.  */
	do
	{
		printf("File/Directory Found Named %s\n", file_name);
		/* Loop through the files in the root directory.  */

		/* Increment file count.  */
		files_count++;

		/* Reset file attributes.  */
		file_attribute = 0;

		/* Try to read the file attributes.  */
		status = fx_file_attributes_read(media, file_name, &file_attribute);

		/* If this is a directory, pass.  */
		if(!(file_attribute & 0x18) && (status == UX_SUCCESS))
		{

			/* A file is present, attempt to read its contents.  */

			/* Try to open the file.  */
			status = fx_file_open(media, &my_file, file_name, FX_OPEN_FOR_READ);
			if (status != UX_SUCCESS)
				break;

			while (status == UX_SUCCESS) 
			{

				/* Read the file in blocks */
				status = fx_file_read(&my_file, local_buffer, UX_DEMO_BUFFER_SIZE, &requested_length);

				/* Check if status OK.  */
				if (status != UX_SUCCESS)
				{
					error_count++;
					break;
				}

				/* Check if end of file.  */
				if (requested_length != UX_DEMO_BUFFER_SIZE)
					break;

			}

			/* Finished reading file either at the end or because of error. */
			fx_file_close(&my_file);
		}

		/* Find the next file.  */
		status = fx_directory_next_entry_find(media, file_name);

	} while (status == UX_SUCCESS);
}
