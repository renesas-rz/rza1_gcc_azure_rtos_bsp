/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : devlink.c
* Version      : 2.00
* Description  : Functions to link devices into the C standard IO run time
*                library
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 05.08.2010 1.00    First Release
*         : 14.12.2010 1.01    Added devGetFromList

*         : 25.02.2018 2.00    Revised dwvlink layer to support stdio & direct
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include <string.h>
#include <fcntl.h>

#include "control.h"
#include "r_devlink_wrapper.h"
#include "r_os_abstraction_api.h"
#include "r_devlink_wrapper_cfg.h"
#include "application_cfg.h"

/******************************************************************************
Macro Definitions
******************************************************************************/

/* Define the maximum number of devices that are expected to be dynamically attached.
 * gs_device_link_table uses this define to control its size, currenlty the USB subsystem uses this table.
 * The USB on chip peripheral can only support 10 devices */
#define DEVICE_LINK_MAX_DEVICES     (10)

/* This macro clears all data set for a dynamic device on removal */
#define DEVLINK_PRV__DEV_FREE(pDynDev)     (memset((pDynDev), 0, sizeof(DYNDEV)))


/******************************************************************************
Private global variables and functions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/* Functions provided by lowsrc.c */
static void close_all(char *p_stream_name);
static bool_t check_open(st_r_driver_t * pDeviceDriver, char *pszLinkName);

/* The string that is used to differentiate a device name from a file name */
static const char * const gsp_dev_ident = DEVICE_INDENTIFIER;

static PDYNDEV dev_alloc_device(void);
static st_r_driver_t * dev_get_class_device(PDYNDEV *ppDeviceListEntry,
                                 char *p_class_link_name);


static const char_t gs_enum_helper_e_sc_id [][7] =
{
        {"R_SC0"},
        {"R_SC1"},
        {"R_SC2"},
        {"R_SC3"},
        {"R_SC4"},
        {"R_SC5"},
        {"R_SC6"},
        {"R_SC7"},
        {"R_SC8"},
        {"R_SC9"},
        {"R_SC10"},
        {"R_SC11"},
        {"R_SC12"},
        {"R_SC13"},
        {"R_SC14"},
        {"R_SC15"},
        {"R_SC16"},
        {"R_SC17"},
        {"R_SC18"},
        {"R_SC19"},
        {"R_SC20"},
        {"R_SC21"},
        {"R_SC22"},
        {"R_SC23"},
        {"R_SC24"},
        {"R_SC25"},
        {"R_SC26"},
        {"R_SC27"},
        {"R_SC28"},
        {"R_SC29"},
        {"R_SC30"},
        {"R_SC31"}
};

/* Define a driver function table for no device */
static st_r_driver_t gs_no_driver =
{
    "No Device Driver",
    no_dev_open,
    no_dev_close,
    no_dev_io,
    no_dev_io,
    no_dev_control,
    no_dev_get_version,
};

static st_devlink_table_t gs_device_link_table[DRIVER_LINK_TABLE_SIZE] =
{
    /* Add in other supported class drivers here */
};

/* The memory for the dynamic devices */
static DYNDEV gs_devices[DEVICE_LINK_MAX_DEVICES];

/* The dynamic device list pointer */
static PDYNDEV gs_pdynamic_device_list = NULL;

/*
 * Interface between devlink and lowsrc
 * Store the lookup table in devlink to allow both direct and posix interfaces to share the same table
 */
static st_stream_t gs_filestream[IOSTREAM];

/***********************************************************************************************************************
 * Function Name: init_gs_mount_table
 * Description  : Initialises the driver entries in the gs_mount_table (above)
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
static void init_gs_mount_table(void)
{
    //int_t index = 2;

     /* Modified by SC do not manually edit */

     /* End of SC modification */

     /* Modified by user, drivers that are not under the control of sc added here */

    /* End of user modification */
}
/***********************************************************************************************************************
 End of function init_gs_mount_table
 **********************************************************************************************************************/


/**********************************************************************************************************************
Private global variables and functions
**********************************************************************************************************************/
/* Dummy driver configuration */

/**********************************************************************************************************************
* Function Name: dev_alloc_device
* Description  : Function to allocate a dynamic device
* Arguments    : none
* Return Value : Pointer to the device or NULL on error
**********************************************************************************************************************/
static PDYNDEV dev_alloc_device(void)
{
    /* NULL signifies that the driver is not loaded */
    PDYNDEV pdynamicdevice = NULL;
    int_t index;

    for (index = 0; index < DEVICE_LINK_MAX_DEVICES; index++)
    {
        if (!gs_devices[index].bfAllocated)
        {
            gs_devices[index].bfAllocated = true;
            pdynamicdevice = &gs_devices[index];
            break;
        }
    }

    return (pdynamicdevice);
}
/**********************************************************************************************************************
End of function  dev_alloc_device
**********************************************************************************************************************/


/**********************************************************************************************************************
Function Name: R_DEVLINK_DevGetFromList
Description:   Function to check the device list for a device
Arguments:     IN  pszLinkName - Pointer to the device link name
Return value:  Pointer to the device or NULL if not found
**********************************************************************************************************************/
static PDYNDEV R_DEVLINK_DevGetFromList(char *pszLinkName)
{
    /* NULL signifies that the driver is not loaded */
    PDYNDEV pdynamicdevice = gs_pdynamic_device_list;
    uint32_t cmp = 0;
    PDYNDEV ret = NULL;

    /* Now search through all the dynamic devices in the device list */
    while (pdynamicdevice)
    {
        cmp = (uint32_t)strcmp(pdynamicdevice->pszDeviceLinkName, pszLinkName);
        if (cmp)
        {
            pdynamicdevice = pdynamicdevice->pNext;
        }
        else
        {
            ret = pdynamicdevice;
            break;
        }
    }

    /* NULL signifies that the driver is not found */
    return (ret);
}
/**********************************************************************************************************************
End of function  R_DEVLINK_DevGetFromList
**********************************************************************************************************************/


/**********************************************************************************************************************
* Function Name: get_first_free_direct_handle
* Description  : Function to get the first free handle from the list maintained
*                in the array gs_filestream.
* Arguments    : IN pszFileName - Pointer to the file name. This is to reserve the file descriptors for the standard
*                streams defined in stdio.h, stdin, stdout and stderr
* Return Value : File number or -1 on error
**********************************************************************************************************************/
static int_t get_first_free_direct_handle(const char *pszFileName)
{
    UNUSED_PARAM(pszFileName);

    /* Allocate all other file descriptors dynamically */
    int_t handle = -1;
    int_t findex;

    for (findex = FIRST_DIRECT_FILE; findex < IOSTREAM; findex++)
    {
        /* NULL signifies that the driver is not loaded */
        if (NULL == gs_filestream[findex].p_device_driver)
        {
            /* First free entry found */
            handle = findex;

            /* force the loop to terminate */
            findex = IOSTREAM+1;
        }
    }
    return handle;
}
/***********************************************************************************************************************
 End of function get_first_free_direct_handle
 **********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: get_first_free_call_stdio_handle
* Description  : Function to get the first free handle from the list maintained
*                in the array gs_filestream.
* Arguments    : IN pszFileName - Pointer to the file name. This is to reserve the file descriptors for the standard
*                streams defined in stdio.h, stdin, stdout and stderr
* Return Value : File number or -1 on error
**********************************************************************************************************************/
static int_t get_first_free_call_stdio_handle(const char *pszFileName)
{
    UNUSED_PARAM(pszFileName);

    /* Allocate all other file descriptors dynamically */
    int_t handle = -1;
    int_t findex;

    for (findex = FIRST_FILE; findex < IOSTREAM; findex++)
    {
        /* NULL signifies that the driver is not loaded */
        if (NULL == gs_filestream[findex].p_device_driver)
        {
            /* First free entry found */
            handle = findex;

            /* force the loop to terminate */
            findex = IOSTREAM+1;
        }
    }
    return handle;
}
/***********************************************************************************************************************
 End of function get_first_free_call_stdio_handle
 **********************************************************************************************************************/

/**********************************************************************************************************************
Function Name: dev_get_class_device
Description:   Function to get a device
Arguments:     OUT ppDeviceListEntry - Pointer to the entry of the device on
                                       the list
               IN  p_class_link_name - Pointer to the class link name
Return value:  Pointer to the first available device driver
               NULL is returned if none available
**********************************************************************************************************************/
static st_r_driver_t * dev_get_class_device(PDYNDEV *ppDeviceListEntry,
                                 char    *p_class_link_name)
{
    PDYNDEV pdynamic_device = gs_pdynamic_device_list;
    int_t compresult = -1;
    bool_t isopen;

    /* Now search through all the dynamic devices in the device list */
    while (pdynamic_device)
    {
        /* Check if the device is already open */
        isopen = check_open(pdynamic_device->p_device_driver,
                pdynamic_device->pszDeviceLinkName);

        /* If there is an available driver, see if there is a device name match */
        if (NULL != pdynamic_device->p_device_driver)
        {
            compresult = strcmp(pdynamic_device->p_device_driver->p_device_name,
                                            p_class_link_name);
        }

        /* If the device is already open or the device name does not
         * match the link name, move on to the next device */
        if (isopen || compresult)
        {
            pdynamic_device = pdynamic_device->pNext;
        }
        else
        {
            if (ppDeviceListEntry)
            {
                *ppDeviceListEntry = pdynamic_device;
            }
            return pdynamic_device->p_device_driver;
        }
    }

    /* NULL signifies that the driver is not loaded */
    return NULL;
}
/**********************************************************************************************************************
End of function  dev_get_class_device
**********************************************************************************************************************/


/**********************************************************************************************************************
* Function Name: check_open
* Description  : Function to find out if a device driver is open
* Arguments    : IN  pDeviceDriver - Pointer to the device driver
*                IN  pszLinkName - Pointer to the link name
* Return Value : true if the device is open
**********************************************************************************************************************/
static bool_t check_open(st_r_driver_t * pDeviceDriver, char *pszLinkName)
{
    int_t index = FOPEN_MAX;

    /* For each file stream */
    while (index--)
    {
        if (gs_filestream[index].p_device_driver == pDeviceDriver)
        {
            if (strcmp(gs_filestream[index].p_stream_name, pszLinkName) == 0)
            {
                return true;
            }
        }
    }
    return false;
}
/**********************************************************************************************************************
End of function  check_open
**********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: close_all
* Description  : Function to close all files of a given name
* Arguments    : IN  pszStreamName - Pointer to the stream name to close
* Return Value : none
**********************************************************************************************************************/
static void close_all(char *pszStreamName)
{
    int_t index = FOPEN_MAX;

    /* For each file stream */
    while (index--)
    {
        /* If the device is open */
        if ((gs_filestream[index].p_device_driver)

        /* Check the link name */
        &&  (strcmp(pszStreamName, gs_filestream[index].p_stream_name) == 0))
        {
            /* Previously this directly hit the low
             * level close function, but left the OS level file
             * open creating a synchronising issue. Modified 020615 to call
             * the OS level close function
             * Previous code line: low_close(filestream[iindex].file_number);*/

            close(gs_filestream[index].file_number);
        }
    }
}
/**********************************************************************************************************************
End of function  close_all
**********************************************************************************************************************/



///***********************************************************************************************************************
// * Function Name: get_filestream
// * Description  : Does an example task. Making this longer just to see how it wraps. Making this long just to see how it
// *                wraps.
// * Arguments    : index -
// *                    Where to start looking
// *                p_output -
// *                    Pointer of where to put the output data
// * Return Value : count -
// *                    How many entries were found
// **********************************************************************************************************************/
//static st_stream_t * get_filestream(int_t driver)
//{
//    /* May not need parameter checking */
//    return &gs_filestream[driver];
//}
///***********************************************************************************************************************
// End of function get_filestream
// **********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: dev_get_pointer
* Description  : Function to get a pointer to the open, close, read, write, control and get_version functions
* Arguments    : IN  pszLinkName - Pointer to the link name
*                OUT p_stream_name - Pointer to the device link name
* Return Value : Pointer to the device or NULL on error
***********************************************************************************************************************/
static st_r_driver_t *dev_get_pointer(char *pszLinkName, char **p_stream_name, uint32_t *sc_index_ptr)
{
    /* NULL signifies that the driver is not loaded */
    st_r_driver_t * p_device_driver = NULL;
    int_t device = (sizeof(gs_mount_table)) / sizeof(struct _mount_table_t);

    /* First, search through all the static devices in the mount table */
    while (device--)
    {
        if (strcmp(pszLinkName, gs_mount_table[device].p_stream_name) == 0)
        {
            /* remove the const is necessary for assignment */
            *p_stream_name = (char*)gs_mount_table[device].p_stream_name;
            *sc_index_ptr  = gs_mount_table[device].sc_config_index;

            /* const needs removing */
            return (st_r_driver_t *)gs_mount_table[device].p_device_driver;
        }
    }

    /* If pdevice_driver is not set, search through all the supported class devices */
    if (NULL == p_device_driver)
    {
        int_t count = (sizeof(gs_device_link_table)) / sizeof(struct _devlink_table_t);

        while (count--)
        {
            /* Is this device a request for a class */
            if (strcmp(pszLinkName,
                       gs_device_link_table[count].p_class_link_name) == 0)
            {
                PDYNDEV pdynamic_device;

                /* remove the const is necessary for assignment */
                p_device_driver = dev_get_class_device(&pdynamic_device, (char*)gs_device_link_table[count].p_driver_name);

                if (p_device_driver)
                {
                    *p_stream_name = pdynamic_device->pszDeviceLinkName;
                    return p_device_driver;
                }
            }
        }
    }

    /* Check for any dynamic device which matches the name */
    {
        /* See if there are any dynamic devices of this name */
        PDYNDEV pdynamic_device = R_DEVLINK_DevGetFromList(pszLinkName);
        if (pdynamic_device)
        {
            *p_stream_name = pdynamic_device->pszDeviceLinkName;
            return pdynamic_device->p_device_driver;
        }
    }

    /* Device not found */
    return (p_device_driver);
}
/***********************************************************************************************************************
End of function dev_get_pointer
***********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: get_device_name
* Description  : Function to see if the file name supplied is for a device
* Arguments    : IN  pszFileName - Pointer to the file name
* Return Value : Pointer to the device link name or NULL on failure
**********************************************************************************************************************/
static char_t *get_device_name(const char *pszFileName)
{
    /* NULL signifies that the driver name is not found */
    char *p_name = NULL;

    if (strstr(pszFileName, gsp_dev_ident) == pszFileName)
    {
        /* NULL signifies that the driver name is not found */
        p_name = (char_t *)(pszFileName += (sizeof(gsp_dev_ident)));
    }

    return (p_name);
}
/**********************************************************************************************************************
End of function  get_device_name
**********************************************************************************************************************/

///**********************************************************************************************************************
//* Function Name: get_device_stream
//* Description  : Function to see if the file name supplied is for a device
//* Arguments    : [IN]  pszFileName - Pointer to the file name
//* Return Value : Pointer to the device link name or NULL on failure
//**********************************************************************************************************************/
//static st_stream_ptr_t get_device_stream(const char *pszFileName)
//{
//    /* NULL signifies that the driver is not loaded */
//    st_stream_ptr_t pstream = NULL;
//    int_t findex;
//
//    for (findex = FIRST_FILE; findex < IOSTREAM; findex++)
//    {
//        if (strcmp(gs_filestream[findex].p_stream_name, pszFileName) == 0)
//        {
//            /* Driver entry found */
//            pstream = &gs_filestream[findex];
//
//            /* force the loop to terminate */
//            findex = IOSTREAM+1;
//        }
//    }
//    return pstream;
//}
///**********************************************************************************************************************
//End of function  get_device_stream
//**********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: get_device_stream_from_handle
 * Description  : Function to see if the file name supplied is for a device
 * Arguments    : [IN] handle - The index to the driver to check taken from the gs_filestream
 * Return Value : DRV_SUCCESS - Hand is valid and in use
 *                DRV_ERROR - handle not in use
 **********************************************************************************************************************/
static st_stream_ptr_t get_device_stream_from_handle(const int_t h)
{
    /* NULL signifies that the driver is not loaded */
    st_stream_ptr_t pstream = NULL;

    if((h >= 0) && (h < IOSTREAM))
    {
        /* NULL signifies that the driver is not loaded */
        if (NULL != gs_filestream[h].p_device_driver)
        {
            /* handle found */
            pstream = &gs_filestream[h];
        }
    }
    return (pstream);
}
/***********************************************************************************************************************
 End of function get_device_stream_from_handle
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: check_valid_handle
 * Description  : Function to ensure that the handle that is presented is in use in the gs_filestream table
 * Arguments    : [IN] handle - The index to the driver to check taken from the gs_filestream
 * Return Value : DRV_SUCCESS - Hand is valid and in use
 *                DRV_ERROR - handle not in use
 **********************************************************************************************************************/
static int_t check_valid_handle(const int_t h)
{
    int_t res = (-1);

    if((h >= 0) && (h < IOSTREAM))
    {
        /* NULL signifies that the driver is not loaded */
        if (NULL != gs_filestream[h].p_device_driver)
        {
            /* handle found */
            res = DRV_SUCCESS;
        }
    }
    return (res);
}
/***********************************************************************************************************************
 End of function check_valid_handle
 **********************************************************************************************************************/


/***********************************************************************************************************************
 Public Functions
**********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: get_no_driver
 * Description  : Returns address of dummy driver configuration, used
 * Arguments    : none
 * Return Value : dummy driver configuration
 **********************************************************************************************************************/
st_r_driver_t * get_no_driver(void)
{
    return (&gs_no_driver);
}
/***********************************************************************************************************************
 End of function get_no_driver
 **********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: no_dev_open
* Description  : Function open stub
* Arguments    : IN  pStream - Pointer to the file stream
* Return Value : 0 for success otherwise -1
***********************************************************************************************************************/
int no_dev_open(st_stream_ptr_t pStream)
{
    /* avoid unused parameter warning */
    (void)pStream;

    return (-1);
}
/***********************************************************************************************************************
End of function  no_dev_open
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: no_dev_close
* Description  : Function close stub
* Arguments    : IN  pStream - Pointer to the file stream
* Return Value : none
***********************************************************************************************************************/
void no_dev_close(st_stream_ptr_t pStream)
{
    /* avoid unused parameter warning */
    (void)pStream;
}
/***********************************************************************************************************************
End of function  no_dev_close
***********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: no_dev_io
* Description  : Function read / write stub
* Arguments    : IN  pStream - Pointer to the file stream
*                IN  pbyBuffer - Pointer to the destination memory
*                IN  uiCount - The number of bytes to read
* Return Value : 0 for success -1 on error
**********************************************************************************************************************/
int_t no_dev_io(st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
{
    /* avoid unused parameter warning */
    (void)pStream;

    /* avoid unused parameter warning */
    (void)pbyBuffer;

    /* avoid unused parameter warning */
    (void)uiCount;

    return (-1);
}
/**********************************************************************************************************************
End of function  no_dev_io
**********************************************************************************************************************/



/**********************************************************************************************************************
* Function Name: no_dev_read
* Description  : Function read stub
* Arguments    : IN  pStream - Pointer to the file stream
*                IN  pbyBuffer - Pointer to the destination memory
*                IN  uiCount - The number of bytes to read
* Return Value : 0 for success -1 on error
**********************************************************************************************************************/
int_t no_dev_read(st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
{
    /* avoid unused parameter warning */
    (void)pStream;

    /* avoid unused parameter warning */
    (void)pbyBuffer;

    /* avoid unused parameter warning */
    (void)uiCount;

    return (-1);
}
/**********************************************************************************************************************
End of function  no_dev_read
**********************************************************************************************************************/


/**********************************************************************************************************************
* Function Name: no_dev_write
* Description  : Function write stub
* Arguments    : IN  pStream - Pointer to the file stream
*                IN  pbyBuffer - Pointer to the destination memory
*                IN  uiCount - The number of bytes to read
* Return Value : 0 for success -1 on error
**********************************************************************************************************************/
int_t no_dev_write(st_stream_ptr_t pStream, const uint8_t *pbyBuffer, uint32_t uiCount)
{
    /* avoid unused parameter warning */
    (void)pStream;

    /* avoid unused parameter warning */
    (void)pbyBuffer;

    /* avoid unused parameter warning */
    (void)uiCount;

    return (-1);
}
/**********************************************************************************************************************
End of function  no_dev_write
**********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: no_dev_control
* Description  : Function to control stub
* Arguments    : IN  pStream - Pointer to the file stream
*                IN  ctlCode - The custom control code
*                IN  pCtlStruct - Pointer to the custorm control structure
* Return Value : 0 for success -1 on error
***********************************************************************************************************************/
int_t no_dev_control(st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct)
{
    /* avoid unused parameter warning */
    (void)pStream;

    /* avoid unused parameter warning */
    (void)ctlCode;

    /* avoid unused parameter warning */
    (void)pCtlStruct;

    return (-1);
}
/***********************************************************************************************************************
End of function  no_dev_control
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: no_dev_get_version
* Description  : Function to version control stub
* Arguments    :
*
*  IN  pStream - Pointer to the file stream
*                IN  st_ver_info_t - The custom control code
*                IN  pCtlStruct - Pointer to the custorm control structure
*
*                st_ver_info_t
* Return Value : 0 for success -1 on error
***********************************************************************************************************************/
int_t no_dev_get_version(st_stream_ptr_t pStream, st_ver_info_ptr_t info_ptr)
{
    /* avoid unused parameter warning */
    (void)pStream;

    /* avoid unused parameter warning */
    (void)info_ptr;

    return -1;
}
/***********************************************************************************************************************
 End of function no_dev_get_version
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: direct_open
 * Description  : devlink direct api open driver function, opens the named driver assuming the name is found
 *                and a handle is free.
 * Arguments    : [IN] p_driver_name - Pointer to the file name (NULL terminated)
 *                [IN] param - the file parameter which is ignored for devices and is
 *                only included here to maitain compatibility with the stdio version of this function
 * Return Value : 0 for success -1 on error
 **********************************************************************************************************************/
int_t direct_open(char *p_driver_name, int_t param)
{
    int_t handle = -1;
    int_t local_handle = -1;
    int_t       open_result = -1;
            
     /* only supplied to match the stdio interface function open */
    (void)param;

    R_OS_SysWaitAccess();

    local_handle = get_first_free_direct_handle(p_driver_name);

    if (local_handle >= 0)
    {
        char link_name[32];

        /* pre-fix the driver ID onto the device_name allowing direct and stdio calls to share code */
        sprintf(link_name, "%s%s", DEVICE_INDENTIFIER, p_driver_name);

        char *p_link_name = get_device_name(link_name);

        /* check to see if this is a driver NULL signifies driver name not found */
        if (p_link_name)
        {
            char      *p_stream_name;
            uint32_t  sc_index;

            st_r_driver_t * p_device_driver = dev_get_pointer(p_link_name, &p_stream_name, &sc_index );

            gs_filestream[local_handle].p_device_driver = &gs_no_driver;

            if (p_device_driver)
            {
                gs_filestream[local_handle].p_stream_name   = p_stream_name;
                gs_filestream[local_handle].p_device_driver = p_device_driver;
                gs_filestream[local_handle].sc_config_index = sc_index;
            }
            else
            {
                open_result = -1;
                handle = -1;
            }

            /* If we found a device or a file then open it */
            if ((gs_filestream[local_handle].p_device_driver) &&
                ((&gs_no_driver) != gs_filestream[local_handle].p_device_driver))
            {
                st_stream_ptr_t pstream = &gs_filestream[local_handle];

                pstream->file_number = local_handle;

                /* drivers ONLY support one mode */
                pstream->file_flag = O_RDWR;

                open_result = pstream->p_device_driver->open(pstream);
            }
            if ((-1) == open_result)
            {
                /* Device failed to open */
                memset(&gs_filestream[local_handle], 0x00, sizeof(st_stream_t));

                /* NULL signifies that the driver is not loaded */
                gs_filestream[local_handle].p_device_driver = NULL;
            }
            else
            {
                /* Device opened successfully assign the handle */
                handle = local_handle;
            }
        }
    }

    R_OS_SysReleaseAccess();

    return (handle);
}
/***********************************************************************************************************************
 End of function direct_open
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: direct_close
 * Description  : devlink direct api close driver function, closes the driver and frees any resources
 * Arguments    : [IN] handle - The index to the driver to close taken from the gs_filestream
 * Return Value : 0 for success, DRV_ERROR on error
 **********************************************************************************************************************/
int_t direct_close(int_t handle)
{
    int_t res = -1;

    res = check_valid_handle(handle);

    if((-1) != res)
    {
        st_stream_ptr_t pstream = get_device_stream_from_handle(handle);

        /* driver has closed successfully, release the resource in the devlink (st_stream_t) table */
        gs_filestream[handle].p_device_driver->close(pstream);
        memset(&gs_filestream[handle], 0x00, sizeof(st_stream_t));

        /* Table entry set to NULL to signify no driver loaded */
        gs_filestream[handle].p_device_driver = NULL;
    }
    return (res);
}
/***********************************************************************************************************************
 End of function direct_close
 **********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: direct_read
* Description  : devlink direct api read data driver function, reads from the driver
* Arguments    : [IN]  handle - The file index
*                [IN]  count - The number of characters to read
*                [OUT] buff_ptr - The pointer to the buffer
* Return Value : Number of characters read, -1 on error
***********************************************************************************************************************/
int_t direct_read(int handle, uint8_t *buff_ptr, uint32_t count)
{
    int_t res = -1;

    res = check_valid_handle(handle);

    if((-1) != res)
    {
        /* File open check */
        if (gs_filestream[handle].p_device_driver)
        {
            st_stream_t * p_stream = &gs_filestream[handle];

            /* File access rights check */
            if ((p_stream->file_flag == O_RDONLY) | (p_stream->file_flag & O_RDWR))
            {
                res = p_stream->p_device_driver->read(p_stream, buff_ptr, count);
            }
        }
    }
    return (res);
}
/***********************************************************************************************************************
End of function direct_read
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: direct_write
* Description  : devlink direct api write data driver function, writes to the driver
* Arguments    : [IN]  handle - The file index
*                [IN]  buff_ptr - The pointer to the buffer
*                [IN]  count - The number of characters to read
* Return Value : Number of characters read, -1 on error
***********************************************************************************************************************/
int_t direct_write(int handle, uint8_t *buff_ptr, uint32_t count)
{
    int_t res = -1;

    res = check_valid_handle(handle);

    if((-1) != res)
    {
        /* File open check */
        if (gs_filestream[handle].p_device_driver)
        {
            st_stream_t *p_stream = &gs_filestream[handle];

            /* File access rights check */
            if ((p_stream->file_flag & O_WRONLY) | (p_stream->file_flag & O_RDWR))
            {
                res = p_stream->p_device_driver->write(p_stream, buff_ptr, count);
            }
        }
    }
    return (res);
}
/***********************************************************************************************************************
 End of function direct_write
 **********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: direct_control
* Description  : devlink direct api write data driver function, writes to the driver
* Arguments    : [IN]  handle - The file index
*                [IN]  ctlCode - The control code selected
*                [IN]  pCtlStruct - The pointer to the control structures associated with the control code
* Return Value : Number of characters read, -1 on error
***********************************************************************************************************************/
int_t direct_control(int handle, uint32_t ctlCode, void *pCtlStruct)
{
    int_t res = -1;

    res = check_valid_handle(handle);

    if((-1) != res)
    {
        /* File open check */
        if (gs_filestream[handle].p_device_driver)
        {
            st_stream_t *p_stream = &gs_filestream[handle];
            res = p_stream->p_device_driver->control(p_stream, ctlCode, pCtlStruct);
        }
    }
    return (res);
}
/***********************************************************************************************************************
 End of function direct_control
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: direct_get_version
 * Description  : devlink direct api version function, retrieve the driver version number.
 * Arguments    : [IN]  p_driver_name - Pointer to the file name (NULL terminated)
 *                [OUT] info - Pointer to the version_info structure
 * Return Value : 0 for success -1 on error
 **********************************************************************************************************************/
int_t direct_get_version(const char *p_driver_name, st_ver_info_t *info)
{
    e_err_code_t res = DRV_ERROR;
    char link_name[32];

    /* pre-fix the driver ID onto the device_name allowing direct and stdio calls to share code */
    sprintf(link_name, "%s%s", DEVICE_INDENTIFIER, p_driver_name);

    char      *p_link_name = get_device_name(link_name);

    /* Check to see if this driver is a driver and not something else */
    if (p_link_name)
    {
        char      *p_stream_name;
        uint32_t  sc_index;

        st_r_driver_t *p_device_driver = dev_get_pointer(p_link_name, &p_stream_name, &sc_index);

        /* NULL signifies that the driver is not loaded so do not call */
        if(NULL != p_device_driver)
        {
            R_OS_SysWaitAccess();

            /* Driver may not be open so must cast st_stream_ptr_t to NULL */
            res = p_device_driver->get_version(NULL, info);
            R_OS_SysReleaseAccess();
        }
    }
    return (res);
}
/***********************************************************************************************************************
 End of function direct_get_version
 **********************************************************************************************************************/


/**********************************************************************************************************************
* Function Name: dev_init_dynamic_list
* Description  : Function to initialise the dynamic device list
* Arguments    : none
* Return Value : none
**********************************************************************************************************************/
static void dev_init_dynamic_list(void)
{
    /* */
    gs_pdynamic_device_list = (PDYNDEV)NULL;

}
/**********************************************************************************************************************
 End of function  dev_init_dynamic_list
 **********************************************************************************************************************/



#if R_USE_ANSI_STDIO_MODE_CFG
/* Enable STDIO interface */


/******************************************************************************
* Function Name: low_open
* Description  : ANSI library low level open function
* Arguments    : IN  pszFileName - Pointer to the file name
*                IN  file_flag - Passed on to open function
* Return Value : File number or -1 on error
******************************************************************************/
int_t low_open(char *pszFileName, int file_flag)
{
    int_t handle = -1;
    int_t local_handle = -1;
    int_t open_result = -1;

    R_OS_SysWaitAccess();

    /* Get the fist available file number */
    local_handle = get_first_free_call_stdio_handle(pszFileName);
    if (local_handle >= 0)
    {
        /* First look for a device */
        char *p_link_name = get_device_name(pszFileName);

        /* Initialise the driver pointer */
        gs_filestream[local_handle].p_device_driver = &gs_no_driver;

        /* Check to see if this is a device driver not a file */
        if (p_link_name)
        {
            char      *p_stream_name;
            uint32_t  sc_index;

            st_r_driver_t * p_device_driver = dev_get_pointer(p_link_name, &p_stream_name, &sc_index );

            if (p_device_driver)
            {
                gs_filestream[local_handle].p_stream_name   = p_stream_name;
                gs_filestream[local_handle].p_device_driver = p_device_driver;
                gs_filestream[local_handle].sc_config_index = sc_index;
            }
            else
            {
                open_result = -1;
            }
        }

#if INCLUDE_FILE_SYSTEM
        /* If it was not a device look for a file */
        if ((gs_filestream[local_handle].p_device_driver == NULL)
        ||  (gs_filestream[local_handle].p_device_driver == (&gs_no_driver)))
        {
            st_stream_ptr_t pStream = &gs_filestream[local_handle];
            st_r_driver_t* p_device_driver;

            /* Set the file number for access to _File if required */
            pStream->file_number = local_handle;

            /* Try to load a driver for this file */
            p_device_driver = fileLoadDriver(pStream, pszFileName, file_flag);
            if (p_device_driver)
            {

                /* Set the driver */
                pStream->p_device_driver = p_device_driver;

            }
        }
#endif

        /* If we found a device or a file then open it */
        if ((gs_filestream[local_handle].p_device_driver) &&
            ((&gs_no_driver) != gs_filestream[local_handle].p_device_driver))
        {
            st_stream_ptr_t pstream = &gs_filestream[local_handle];

            pstream->file_number = local_handle;
            if (0 == file_flag)
            {
                /* Always read only */
                pstream->file_mode = __SRD;
            }

            /* Check for read and write */
            if (file_flag & O_RDWR)
            {
                pstream->file_mode = (__SRD | __SWR);
            }

            /* Check for write only */
            else if (file_flag & (O_WRONLY))
            {
                pstream->file_mode = __SWR;
            }
            else
            {
                /* If a open is requested with unrecognised flags
                 * such as 'rb', grant read only to allow  level
                 * of functionality.
                 */
                /* Always grant read only mode */
                pstream->file_mode = __SRD;
            }

            /* Check for create & add write */
            if (file_flag & O_CREAT)
            {
                pstream->file_mode |= __SWR;
            }
            pstream->file_flag = file_flag;

            open_result = pstream->p_device_driver->open(pstream);
        }
    }

    if ((-1) == open_result)
    {
        /* Device failed to open */
        memset(&gs_filestream[local_handle], 0x00, sizeof(st_stream_t));

        /* NULL signifies that the driver is not loaded */
        gs_filestream[local_handle].p_device_driver = NULL;
    }
    else
    {
        /* Device opened successfully assign the handle */
        handle = local_handle;
    }

    R_OS_SysReleaseAccess();

    return (handle);
}
/******************************************************************************
End of function low_open
******************************************************************************/

/******************************************************************************
* Function Name: low_close
* Description  : ANSI library low level close function
* Arguments    : IN  file_number - The file number to close
* Return Value : 0 for success, -1 on error
******************************************************************************/
int_t low_close(int_t file_number)
{
    return direct_close((file_number));
}
/******************************************************************************
End of function low_close
******************************************************************************/

/******************************************************************************
* Function Name: low_read
* Description  : ANSI library low level read function
* Arguments    : IN  file_number - The file index
*                IN  pbyBuffer - The pointer to the buffer
*                IN  iCount - The number of characters to read
* Return Value : Number of characters read, -1 on error
******************************************************************************/
int_t low_read(int file_number, uint8_t *pbyBuffer, uint32_t iCount)
{
    return direct_read((file_number), pbyBuffer, iCount);
}
/******************************************************************************
End of function low_read
******************************************************************************/

/******************************************************************************
* Function Name: low_write
* Description  : ANSI library low level read function
* Arguments    : IN  file_number - The file index
*                IN  pbyBuffer - The pointer to the buffer
*                IN  iCount - The number of characters to write
* Return Value : Number of characters written, -1 on error
******************************************************************************/
int_t low_write(int file_number, uint8_t *pbyBuffer, uint32_t iCount)
{
    return direct_write((file_number), pbyBuffer, iCount);
}
/******************************************************************************
End of function low_write
******************************************************************************/

/******************************************************************************
* Function Name: control (NOT ANSI)
* Description  : Function to perform a device control provided by a control code.
*                This is not a standard ANSI function. It is provided as an extension
*                to the ANSI architecture.
* Arguments    : IN  file_number - The file number to control
*                IN  ctlCode - The device specific control code
*                IN  pCtlStruct - Pointer to the control structure
* Return Value : 0 for success -1 on error
******************************************************************************/
int_t control(int file_number, uint32_t ctlCode, void *pCtlStruct)
{
    return direct_control((file_number), ctlCode, pCtlStruct);
}
/******************************************************************************
End of function  control
******************************************************************************/

/******************************************************************************
* Function Name: get_version (NOT ANSI)
* Description  : Function to retrieve the driver version number.
*                This is not a standard ANSI function. It is provided as an extension
*                to the ANSI architecture.
* Arguments    : IN  file_number - The file number to control
*                IN  pCtlStruct - Pointer to the version_info structure
* Return Value : 0 for success -1 on error
******************************************************************************/
int_t get_version(const char *p_driver_name, st_ver_info_t *info)
{
    return direct_get_version(p_driver_name, info);
}
/******************************************************************************
End of function  get_version
******************************************************************************/

/******************************************************************************
* Function Name: low_lseek
* Description  : ANSI library low level seek function
* Arguments    : IN  file_number - The file index
*                IN  lOffset - The offset
*                IN  iBase - The base
* Return Value : The offset in bytes to the current file position or -1 on error
******************************************************************************/
int_t low_lseek(int file_number, uint32_t lOffset, int iBase)
{
    /* File number range check */
    if ((file_number < 0) || ((FOPEN_MAX - 1) < file_number))
    {
        return (-1L);
    }

    /* File open check */
    if (gs_filestream[file_number].p_device_driver)
    {
        st_stream_ptr_t pstream = &gs_filestream[file_number];
        FILESEEK file_seek;
        file_seek.lOffset = (signed)lOffset;
        file_seek.iBase = iBase;
        file_seek.lResult = 0L;
        if ((pstream->p_device_driver->control(pstream,
                                             CTL_FILE_SEEK, &file_seek))
        &&  (0L == file_seek.lResult))
        {
            file_seek.lResult = (-1L);
        }
        return file_seek.lResult;
    }
    return -1L;
}
/******************************************************************************
End of function low_lseek
******************************************************************************/

#endif /* R_USE_ANSI_STDIO_MODE_CFG */

/***********************************************************************************************************************
 * Function Name: R_DEVLINK_Init
 * Description  : Initialisation function
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void R_DEVLINK_Init(void)
{
    /* Initialise the devlink tables */
    memset(gs_filestream, 0, sizeof(gs_filestream));

    init_gs_mount_table();

    /* Initialise the list of dynamic devices */
    dev_init_dynamic_list();

}
/***********************************************************************************************************************
 End of function R_DEVLINK_Init
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_DEVLINK_ListDevlinkTblContent
 * Description  : Determine the number of entries in the gs_mount_table table.
 *                Optional - Display to stdout console details about the entries in the gs_mount_table table.
 * Arguments    : use_console -
 *                    true  - display output to the stdout console
 *                    false - do not display any output to the console
 * Return Value : active_size -
 *                    Number of open entries in the gs_mount_table table.
 **********************************************************************************************************************/
int_t R_DEVLINK_ListDevlinkTblContent(bool_t use_console, pst_comset_t pCom)
{
    int_t device_count = (sizeof(gs_mount_table)) / sizeof(struct _mount_table_t);
    int_t index = 0;

    if (use_console)
    {
        /* ACQUIRE MUTEX LIST LOCK */
        R_OS_SysLock(NULL);

        fprintf(pCom->p_out, "\r\n");
        fprintf(pCom->p_out, "Query gs_mount_table, currently installed drivers\r\n");
        fprintf(pCom->p_out, "\r\n");

        fprintf(pCom->p_out, " %3s " " %9s "     " %12s "        " %7s "    " %s\r\n", "ID","ADDR","DRV_NAME","CONFIG","NAME");
        fprintf(pCom->p_out, "==== "  "========== " "============= " "======== " "=========================\r\n");

        for(index = 0; index < device_count; index++)
        {
            fprintf(pCom->p_out, " %3d %p %14s %8s %s\r\n",
                    index,
                    gs_mount_table[index].p_device_driver,
                    gs_mount_table[index].p_stream_name,
                    gs_enum_helper_e_sc_id[gs_mount_table[index].sc_config_index],
                    gs_mount_table[index].p_device_driver->p_device_name);
        }

        fprintf(pCom->p_out, "\r\n");
        fprintf(pCom->p_out, "Number of devices available [%d]\r\n", device_count);
        fprintf(pCom->p_out, "Note - devices may not be loaded use handles to list loaded drivers \r\n");
        fprintf(pCom->p_out, "\r\n");

        /* RELEASE MUTEX LIST LOCK */
        R_OS_SysUnlock(NULL, 0);

        R_DEVLINK_DevList(pCom->p_out);


    }

    return device_count;
}
/***********************************************************************************************************************
 End of function R_DEVLINK_ListDevlinkTblContent
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_DEVLINK_ListDeviceOpenHandles
 * Description  : Determine the number of open handles in the gs_filestream table.
 *                Optional - Display to stdout console details about the open devices in the gs_filestream table.
 * Arguments    : use_console -
 *                    true  - display output to the stdout console
 *                    false - do not display any output to the console
 * Return Value : active_size -
 *                    Number of open handles in the gs_filestream table.
 **********************************************************************************************************************/
int_t R_DEVLINK_ListDeviceOpenHandles(bool_t use_console, pst_comset_t pCom)
{
    int_t active_size = 0;
    int_t tbl_index = 0;
    int_t tbl_size = (sizeof(gs_filestream)) / sizeof(struct st_def_stream_t);

    if (use_console)
    {
        fprintf(pCom->p_out, "\r\n");
        fprintf(pCom->p_out, "Query gs_filestream, currently open drivers\r\n");
        fprintf(pCom->p_out, "\r\n");

        fprintf(pCom->p_out, " %7s " " %9s "     " %30s "        " %7s "    " \r\n", "HANDLE","ADDR","DRV_NAME","CONFIG");
        fprintf(pCom->p_out, "======== " "========== " "=============================== " "======== " "\r\n");
    }

    for (tbl_index = 0; tbl_index < tbl_size; tbl_index++)
    {
        /* Table entry set to NULL to signify no driver loaded */
        if (NULL != gs_filestream[tbl_index].p_device_driver)
        {
            active_size++;
            if (use_console)
            {
                if(gs_filestream[tbl_index].sc_config_index > 31)
                {
                    gs_filestream[tbl_index].sc_config_index = 0;
                }
                fprintf(pCom->p_out, " %7d %p %32s %s\r\n",
                        tbl_index,
                        &gs_filestream[tbl_index],
                        gs_filestream[tbl_index].p_stream_name,
                        gs_enum_helper_e_sc_id[gs_filestream[tbl_index].sc_config_index]);

            }
        }
    }

    if (use_console)
    {
        fprintf(pCom->p_out, "\r\n");
        fprintf(pCom->p_out, "Total number of drivers in the gs_filestream table that are open [%d]\r\n", active_size);
        fprintf(pCom->p_out, "\r\n");

    }

    return (active_size);
}
/***********************************************************************************************************************
 End of function R_DEVLINK_ListDeviceOpenHandles
 **********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DEVLINK_InsertDeviceLinkTableEntry
* Description  : Function to insert a device driver into the device table list this action must be completed before the
*                driver can be opened
* Arguments    : pszClassLinkName  - link name passed to the open function
*                pszDriverName - pszDeviceName member in the st_r_driver_t structure
* Return Value : >= 0   - location of newly added driver in the mount table
*                -1     - error pszClassLinkName in use
*                -2     - error pszDriverName in use
*                -3     - error table full
***********************************************************************************************************************/
int_t R_DEVLINK_InsertDeviceLinkTableEntry(const char *pszClassLinkName, const char *pszDriverName)
{
    int_t ret = (-3);
    int_t mt = 0;
    int_t ff = DRIVER_LINK_TABLE_SIZE;

    /* ACQUIRE MUTEX LIST LOCK */
    R_OS_SysLock(NULL);

    while(mt < DRIVER_LINK_TABLE_SIZE)
    {
        if(0 == (uint32_t) gs_device_link_table[mt].p_class_link_name)
        {
            /* Update first free entry index */
            ff = mt;
            mt = DRIVER_LINK_TABLE_SIZE;
        }
        else
        {
            if(strcmp(pszClassLinkName, gs_device_link_table[mt].p_class_link_name) == 0)
            {
                /* Device name is in use */
                ret = (-1);
                mt = DRIVER_LINK_TABLE_SIZE;
            }
        }
        /* increment index */
        mt++;
    }

    /* check space is available for the new driver */
    if(ff < DRIVER_LINK_TABLE_SIZE)
    {
        /* space in table at index ff for new device */
        gs_device_link_table[ff].p_class_link_name = pszClassLinkName;
        gs_device_link_table[ff].p_driver_name = pszDriverName;
        ret = ff;
    }

    /* RELEASE MUTEX LIST LOCK */
    R_OS_SysUnlock(NULL, 0);

    return(ret);
}
/***********************************************************************************************************************
End of function  R_DEVLINK_devInsertDeviceLinkTableEntry
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DEVLINK_RemoveDeviceLinkTableEntry
* Description  : Function to remove a device driver from the device table list this action must be completed before the
*                driver can be opened
* Arguments    : pszClassLinkName  - link name passed to the open function
*                pszDriverName - pszDeviceName member in the st_r_driver_t structure
* Return Value : >= 0   - success
*                -1     - error pszClassLinkName not in use
*                -2     - error pszDriverName not in use
*                -3     - error table empty
***********************************************************************************************************************/
int_t R_DEVLINK_RemoveDeviceLinkTableEntry(const char *pszClassLinkName, const char *pszDriverName)
{
    UNUSED_PARAM(pszDriverName);
    int_t ret = (-3);
    int_t mt = 0;
    int_t ff = DRIVER_LINK_TABLE_SIZE;

    /* ACQUIRE MUTEX LIST LOCK */
    R_OS_SysLock(NULL);

    while(mt < DRIVER_LINK_TABLE_SIZE)
    {
        if(0 == (uint32_t) gs_device_link_table[mt].p_class_link_name)
        {
            /* Skip unused entries */
        }
        else
        {
            if(strcmp(pszClassLinkName, gs_device_link_table[mt].p_class_link_name) == 0)
            {
                /* Device name is in use */
                ff = mt;
                mt = DRIVER_LINK_TABLE_SIZE;
            }
        }
        /* increment index */
        mt++;
    }

    /* check space is available for the new driver */
    if(ff < DRIVER_LINK_TABLE_SIZE)
    {
        /* remove table entry at index ff  */
        gs_device_link_table[ff].p_class_link_name = NULL;
        gs_device_link_table[ff].p_driver_name = NULL;
        ret = ff;
    }

    /* RELEASE MUTEX LIST LOCK */
    R_OS_SysUnlock(NULL, 0);

    return(ret);
}
/***********************************************************************************************************************
End of function  R_DEVLINK_devInsertDeviceLinkTableEntry
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: R_DEVLINK_DevAdd
* Description  : Function to add a device to the device list
* Arguments    : IN  pszLinkName - Pointer to the device link name, this is used to open the device
*                IN  p_device_driver - Pointer to the functions to open, close, read, write, control & get_version
*                IN  pInformation - Pointer to the information that describes the device
* Return Value : true for success
***********************************************************************************************************************/
bool_t R_DEVLINK_DevAdd(char *pszLinkName, st_r_driver_t * p_device_driver, PDEVINFO pInformation)
{
    PDYNDEV pnew_device;

    /* Allocate the memory */
    pnew_device = dev_alloc_device();
    if (pnew_device)
    {
        PDYNDEV *pp_end_of_list = &gs_pdynamic_device_list;

        /* Set the information */
        pnew_device->pszDeviceLinkName = pszLinkName;
        pnew_device->p_device_driver = p_device_driver;
        pnew_device->Information = *pInformation;

        /* ACQUIRE MUTEX LIST LOCK */
        R_OS_SysLock(NULL);

        /* Find the end of the list */
        while (*pp_end_of_list)
        {
            pp_end_of_list = &(*pp_end_of_list)->pNext;
        }

        /* Add the to the list */
        if (pp_end_of_list)
        {
            /* Add to the End of the list */
            *pp_end_of_list = pnew_device;
        }
        else
        {
            /* Add to the top of the list */
            pnew_device->pNext = gs_pdynamic_device_list;
            gs_pdynamic_device_list = pnew_device;
        }

        /* RELEASE MUTEX LIST LOCK */
        R_OS_SysUnlock(NULL, 0);

        return true;
    }
    return false;
}
/***********************************************************************************************************************
End of function  R_DEVLINK_DevAdd
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DEVLINK_DevRemove
* Description  : Function to remove a device from the list
* Arguments    : IN  pszLinkName - Pointer to the device link name, this is
*                                  used to open the device
* Return Value : true for success
***********************************************************************************************************************/
bool_t R_DEVLINK_DevRemove(char *pszLinkName)
{
    PDYNDEV pdynamic_device;
    bool_t   result;
    bool_t   removed = false;

    /* One device can support more than one interface */
    do {
        result = false;

        /* See if the device exists */
        pdynamic_device = R_DEVLINK_DevGetFromList(pszLinkName);
        if (pdynamic_device)
        {
            /* Look for the device on the list */
            PDYNDEV *pp_dynamic_device_ist = &gs_pdynamic_device_list;

            /* ACQUIRE MUTEX LIST LOCK */
            R_OS_SysLock(NULL);

            while ((*pp_dynamic_device_ist)
            &&     (pdynamic_device != (*pp_dynamic_device_ist)))
            {
                pp_dynamic_device_ist = &(*pp_dynamic_device_ist)->pNext;
            }

            /* If it is found */
            if (*pp_dynamic_device_ist)
            {
                /* Close the file stream that use it */
                close_all(pszLinkName);

                /* Remove from the device list */
                *pp_dynamic_device_ist = pdynamic_device->pNext;

                /* Free the device */
                DEVLINK_PRV__DEV_FREE(pdynamic_device);

                result = true;
                removed = true;
            }

            /* RELEASE MUTEX LIST LOCK */
            R_OS_SysUnlock(NULL, 0);
        }

     /* Remove more than one device with the same link name */
    } while (result);

    return removed;
}
/***********************************************************************************************************************
End of function  R_DEVLINK_DevRemove
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_DEVLINK_DevList
* Description  : Function to list the link names and details of the devices attached
* Arguments    : none
* Return Value : The number of devices attached
***********************************************************************************************************************/
int_t R_DEVLINK_DevList(FILE *p_out)
{
    int_t device_count = 0;
    PDYNDEV pdynamic_device = gs_pdynamic_device_list;

    /* ACQUIRE MUTEX LIST LOCK */
    R_OS_SysLock(NULL);
    while (pdynamic_device)
    {
        device_count++;
        fprintf(p_out, "Link name: %s\r\n",pdynamic_device->pszDeviceLinkName);
        if (strlen(pdynamic_device->Information.pszManufacturer))
        {
            fprintf(p_out, "Manufacturer: %s\r\n",
                    pdynamic_device->Information.pszManufacturer);
        }
        else
        {
            fprintf(p_out, "Manufacturer: Unknown\r\n");
        }
        if (strlen(pdynamic_device->Information.pszProduct))
        {
            fprintf(p_out, "Product: %s\r\n",
                    pdynamic_device->Information.pszProduct);
        }
        else
        {
            fprintf(p_out, "Product: Unknown\r\n");
        }
        if (strlen(pdynamic_device->Information.pszSerialNumber))
        {
            fprintf(p_out, "Serial Number: %s\r\n",
                    pdynamic_device->Information.pszSerialNumber);
        }
        else
        {
            fprintf(p_out, "Serial Number: Unknown\r\n");
        }
        fprintf(p_out, "Vendor ID: 0x%.4X\r\n",
                pdynamic_device->Information.wVID);
        fprintf(p_out, "Product ID: 0x%.4X\r\n",
                pdynamic_device->Information.wPID);
        fprintf(p_out, "Device Class: 0x%.2X\r\n",
                pdynamic_device->Information.byDeviceClass);
        fprintf(p_out, "Device Sub Class: 0x%.2X\r\n",
                pdynamic_device->Information.byDeviceSubClass);
        fprintf(p_out, "Device Protocol: 0x%.2X\r\n",
                pdynamic_device->Information.byProtocolCode);
        if (pdynamic_device->p_device_driver)
        {
            fprintf(p_out, "Device Driver \"%s\" available for this device\r\n\r\n",
                    pdynamic_device->p_device_driver->p_device_name);
        }
        else
        {
            fprintf(p_out, "There is no device driver available\r\n\r\n");
        }

        /* Check the next device */
        pdynamic_device = pdynamic_device->pNext;
    }

    /* RELEASE MUTEX LIST LOCK */
    R_OS_SysUnlock(NULL, 0);

    return (device_count);
}
/***********************************************************************************************************************
End of function  R_DEVLINK_DevList
***********************************************************************************************************************/

/***********************************************************************************************************************
Function Name: R_DEVLINK_DevInfo
Description:   Function get the information about a dynamic device
Arguments:     OUT pDynDev - Pointer to the destination dynamic device info
               IN  iIndex - The index of the device
Return value:  true if a device was found
***********************************************************************************************************************/
bool_t R_DEVLINK_DevInfo(PDYNDEV pDynDev, int iIndex)
{
    bool_t result = false;
    int_t  device_count = 0;
    PDYNDEV pdynamic_device = gs_pdynamic_device_list;

    /* ACQUIRE MUTEX LIST LOCK */

    R_OS_SysLock(NULL);
    while (pdynamic_device)
    {
        if (iIndex == device_count)
        {
            *pDynDev = *pdynamic_device;
            result = true;
            break;
        }
        else
        {
            /* Check the next device */
            device_count++;
            pdynamic_device = pdynamic_device->pNext;
        }
    }

    /* RELEASE MUTEX LIST LOCK */
    R_OS_SysUnlock(NULL, 0);

    return result;
}
/***********************************************************************************************************************
End of function  R_DEVLINK_DevInfo
***********************************************************************************************************************/

/***********************************************************************************************************************
Function Name: R_DEVLINK_FilePtrDescriptor
Description:   Function to convert a file pointer into a file descriptor
Arguments:     IN  pFile - Pointer to the file stream;
Return value:  The file descriptor
 **********************************************************************************************************************/
int_t R_DEVLINK_FilePtrDescriptor(FILE *pFile)
{
    int_t result = (-1);
    if (pFile)
    {
        /* defined in reent.h as struct __sFILE */
        struct __sFILE *p_internal = (struct __sFILE *) pFile;

        /* defined in reent.h short _file;    // fileno, if Unix descriptor, else -1 */
        result =  (int_t) p_internal->_file;
    }
    return (result);
}
/***********************************************************************************************************************
 End of function R_DEVLINK_FilePtrDescriptor
 **********************************************************************************************************************/

/***********************************************************************************************************************
End  Of File
 **********************************************************************************************************************/
