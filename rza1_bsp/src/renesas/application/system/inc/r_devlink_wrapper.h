/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is  protected
* under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR  A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE  EXPRESSLY
* DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_devlink_wrapper.h
* Version      : 2.00
* Description  : Functions to link devices into the C standard IO run time
*                library
*******************************************************************************
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


/*********************************************************************************************************************
 * @ingroup Interface_Library
 * @defgroup DS_DEVLINK_BASE STDIO Driver Interface Definition
 * @brief Defines the interface for an application to interact with a driver.
 *
 * @anchor RZA1H_DS_DEVLINK_API_EX
 * @par Examples
 *
 * @anchor RZA1H_DS_DEVLINK_API_LIMITATIONS
 * @par Known Limitations
 * NONE
 *
 * @anchor RZA1H_DS_DEVLINK_API_INSTANCES
 * @par Known Implementations
 * @ref NONE_YET
 *
 * @anchor RZA1H_DS_DEVLINK_API_RELATED
 * @par Related modules
 * @{
 **********************************************************************************************************************/


#include "r_typedefs.h"
#include "application_cfg.h"
#include "control.h"
#include "console.h"

#ifndef DEVLINK_H_INCLUDED
#define DEVLINK_H_INCLUDED

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/


/** Define the maximum number of devices that are expected to be dynamically
   attached. The USB on chip peripheral can only support 10 devices */
#define DEVICE_LINK_MAX_DEVICES     (10)

/** Define the maximum number of dynamic devices that are expected to be dynamically
   attached. The USB on chip peripheral can only support 10 devices */
#define DRIVER_LINK_TABLE_SIZE      (6)

/** The first file direct descriptor available to the user note 0,1,2 are reserved */
#define FIRST_DIRECT_FILE           (3)

/** The first file descriptor available to the user */
#define FIRST_FILE                  (0)

/** The maximum number of driver descriptor available to the user */
#define DEV_MAX_DRIVER_DEVICES      (64)

/** Define the maximum string size for the device information strings */
#define DEVICE_MAX_STRING_SIZE      (128)

/** Define the size of the devlink table, if using posix stdio this size is determined by FOPEN_MAX
 * otherwise the linited is defined in this file */
#ifndef FOPEN_MAX

/** <stdio.h> is not included so define the limit here */
#define IOSTREAM (DEV_MAX_DRIVER_DEVICES)

#else

/** FOPEN_MAX should have been defined in <stdio.h> */
#define IOSTREAM (FOPEN_MAX + DEV_MAX_DRIVER_DEVICES)

#endif


/******************************************************************************
Typedef definitions
******************************************************************************/
#pragma pack(1)

/** Define the structure of the information that describes a device */
typedef struct _DEVINFO
{
    /** The device symbolic link name */
    char      pszDeviceLinkName[DEVICE_MAX_STRING_SIZE];

    /** The manufacturer string */
    char      pszManufacturer[DEVICE_MAX_STRING_SIZE];

    /** The product string */
    char      pszProduct[DEVICE_MAX_STRING_SIZE];

    /** The serial number string */
    char      pszSerialNumber[DEVICE_MAX_STRING_SIZE];

    /** The device vendor ID */
    uint16_t  wVID;

    /** The device product ID */
    uint16_t  wPID;

    /** The device version in BCD */
    uint16_t  wbcdVersion;

    /** The device class code */
    uint8_t   byDeviceClass;

    /** The device sub class code */
    uint8_t   byDeviceSubClass;

    /** The device protocol code */
    uint8_t   byProtocolCode;
} DEVINFO,
*PDEVINFO;

/* copy from other devlink.h */

typedef struct _DEVICE *PDEVICE;

typedef struct st_def_driver_t *st_driver_ptr_t;

typedef struct st_def_stream_t *st_stream_ptr_t;

/** @struct st_def_stdio_device_t
 * @brief This reference provides a demonstration of using High Level drivers
 * via POSIX/STDIO and also by direct call, using the OSTM driver as a specific example.
 * @par ANSI STDIO Call Example open
<pre>
 * // DEVICE_INDENTIFIER          "\\\\.\\"
 * static int_t gs_handle = DRV_ERROR;
 * // Open the ostm driver
 * gs_handle  = open(DEVICE_INDENTIFIER "ostm", 0);
 * // Only continue if the drive has successfully been opened
 * configASSERT((DRV_ERROR != gs_handle));
</pre>
 * @par Direct Call Example open
<pre>
 * static int_t gs_handle = DRV_ERROR;
 * // Open the ostm driver
 * gs_handle = g_stdio_driver.p_device.open(NULL);
 * // Only continue if the drive has successfully been opened
 * configASSERT((DRV_ERROR != gs_handle));
</pre>
 *
 * @par ANSI STDIO Call Example close
<pre>
 * // gs_handle <b>MUST</b> have been opened
 * // Close the ostm driver
 * close(gs_handle);
</pre>
 * @par Direct Call Example close
<pre>
 * // gs_handle <b>MUST</b> have been opened
 * // Close the ostm driver
 * gs_handle = g_stdio_driver.p_device.close(gs_handle);
</pre>
 *
 * @par  ANSI STDIO Call Example read
<pre>
 * uint32_t ulValueNow;
 * // gs_handle <b>MUST</b> have been opened
 * // Read data
 * read((st_stream_ptr_t)gs_handle, (uint8_t *)&ulValueNow, ((sizeof(ulValueNow))/(sizeof(uint8_t))));
</pre>
 * @par  Direct Call Example read
<pre>
 * uint32_t ulValueNow;
 * // gs_handle <b>MUST</b> have been opened
 * // Read data
 * g_stdio_driver.p_device.read((st_stream_ptr_t)gs_handle, (uint8_t *)&ulValueNow, ((sizeof(ulValueNow))/(sizeof(uint8_t))));
</pre>
* @par Call Example write
<pre>
not in use
</pre>
 * @par  ANSI STDIO Call Example control
<pre>
 * // gs_handle <b>MUST</b> have been opened
 * // Only continue if the drive has been successfully created
 * configASSERT(DRV_ERROR !=
 *       control ((st_stream_ptr_t)gs_handle, CTL_GET_LAST_ERROR, &config));
</pre>
* @par Direct Call Example control
<pre>
 * // gs_handle <b>MUST</b> have been opened
 * // Only continue if the drive has been successfully created
 *   configASSERT(DRV_ERROR !=
 *       g_stdio_driver.p_device.control ((st_stream_ptr_t)gs_handle, CTL_GET_LAST_ERROR, &config));
</pre>
 * @par  ANSI STDIO Call Example get_version
<pre>
 * // gs_handle <b>DOES NOT NEED</b> to be open
 * get_version((st_stream_ptr_t)gs_handle, &drv_info);
</pre>
 * @par Direct Call Example get_version
<pre>
 * // gs_handle <b>DOES NOT NEED</b> to be open
 * pdriver->p_info.get_version((st_stream_ptr_t)gs_handle, &drv_info);
</pre>
 */

/** Define the structure of the functions to access the device driver */
typedef struct st_r_driver_intern_t
{
    /**< The driver name */
    const char   *const p_device_name;

    /**< The driver open function */
    int_t (*const open)(st_stream_ptr_t pStream);

    /**< The driver close function */
    void (*const close)(st_stream_ptr_t pStream);

    /**< The driver read function */
    int_t (*const read)(st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);

    /**< The driver write function */
    int_t (*const write)(st_stream_ptr_t pStream, uint8_t*pbyBuffer, uint32_t uiCount);

    /**< A driver control function to handle all other driver specific
       functions */
    int_t (*const control)(st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct);

    /**< The driver version (updated by developer) */
    int_t (*const get_version)(st_stream_ptr_t pStream, st_ver_info_t *pVerInfo);

} st_r_driver_t;


/** Define the structure of the table to hold a pointer to the device driver
   functions, pointer to any driver specific data, pointer to the file and
   access options */
typedef struct st_def_stream_t
{
    st_r_driver_t * p_device_driver;
    st_r_driver_t * p_driver;
    char_t  *p_stream_name;
    void    *p_extension;
#if defined( __RX )
    long    iFileNumber;
    long    iFileMode;
    long    iFileFlag;
#else
    int_t   file_number;
    int_t   file_mode;
    int_t   file_flag;
#endif
    e_sc_id_t sc_config_index;
} st_stream_t;

typedef struct _devlink_table_t
{
    /* The link name passed to the open function */
    const char * p_class_link_name;

    /* The p_device_name member in the DEVICE structure */
    const char * p_driver_name;

} st_devlink_table_t;

/** Define the structure of a device which can be loaded at run time */
typedef struct _DYNDEV
{
    /** Pointer to the next on the list */
    struct _DYNDEV *pNext;

    /** Pointer to the device link name */
    char    *pszDeviceLinkName;

    /** Flag to show if the device is allocated */
    bool_t   bfAllocated;

    /** Pointer to functions that open, close, read, write and control the
       device */
    struct st_r_driver_intern_t *p_device_driver;

    /** The information that describes the device */
    DEVINFO Information;
} DYNDEV,
*PDYNDEV;


/** Control structure for CTL_FILE_SEEK */
typedef struct _FILESEEK
{
    long    lOffset;
    int     iBase;
    long    lResult;
} FILESEEK,
*PFILESEEK;

#pragma pack()

/******************************************************************************
Constant Data
******************************************************************************/

/******************************************************************************
Function Prototypes
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
Function Name: devInitDynamicList
Description:   Function to initialise the dynamic device list
Arguments:     none
Return value:  none
******************************************************************************/

extern  void devInitDynamicList(void);


/******************************************************************************
* Function Name: devInsertDeviceLinkTableEntry
* Description  : Function to insert a device driver into the device table list
*                this action must be completed before the driver can be opened
* Arguments    : pszClassLinkName  - link name passed to the open function
*                pszDriverName - pszDeviceName member in the st_r_driver_t
*                structure
* Return Value : >= 0   - location of newly added driver in the mount table
*                -1     - error pszClassLinkName in use
*                -2     - error pszDriverName in use
*                -3     - error table full
******************************************************************************/
extern int_t R_DEVLINK_InsertDeviceLinkTableEntry(const char *pszClassLinkName, const char *pszDriverName);


/******************************************************************************
* Function Name: R_DEVLINK_RemoveDeviceLinkTableEntry
* Description  : Function to remove a device driver from the device table list
* Arguments    : pszClassLinkName  - link name passed to the open function
*                pszDriverName - pszDeviceName member in the st_r_driver_t
*                structure
* Return Value : >= 0   - success
*                -1     - error pszClassLinkName not in use
*                -2     - error pszDriverName not in use
*                -3     - error table empty
******************************************************************************/
extern int_t R_DEVLINK_RemoveDeviceLinkTableEntry(const char *pszClassLinkName, const char *pszDriverName);


/******************************************************************************
Function Name: R_DEVLINK_DevAdd
Description:   Function to add a device to the device list
Arguments:     IN  pszLinkName - Pointer to the device link name, this is used
                                 to open the device
               IN  p_device_driver - Pointer to the functions to open, close,
                                   read, write and control the device
               IN  pInformation - Pointer to the information that describes the
                                  device
Return value:  true for success
******************************************************************************/

extern bool_t R_DEVLINK_DevAdd(char *pszLinkName, st_r_driver_t * p_device_driver, PDEVINFO pInformation);

/******************************************************************************
Function Name: R_DEVLINK_DevRemove
Description:   Function to remove a device from the list
Arguments:     IN  pszLinkName - Pointer to the device link name, this is used
                                 to open the device
Return value:  true for success
******************************************************************************/

extern  bool_t R_DEVLINK_DevRemove(char *pszLinkName);

/******************************************************************************
Function Name: R_DEVLINK_DevList
Description:   Function to list the link names and details of the devices
               attached
Arguments:     IN  pFile - Pointer to the file stream to print the details to
Return value:  The number of devices attached
******************************************************************************/

extern  int_t R_DEVLINK_DevList(FILE *p_out);

/*****************************************************************************
Function Name: R_DEVLINK_DevInfo
Description:   Function get the information about a dynamic device
Arguments:     OUT pDynDev - Pointer to the destination dynamic device info
               IN  iIndex - The index of the device
Return value:  true if a device was found
*****************************************************************************/

extern  bool_t R_DEVLINK_DevInfo(PDYNDEV pDynDev, int iIndex);

extern int_t R_DEVLINK_FilePtrDescriptor(FILE *pFile);

extern  int no_dev_open(st_stream_ptr_t pStream);
extern  void no_dev_close(st_stream_ptr_t pStream);
extern  int_t no_dev_io(st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
extern  int_t no_dev_read(st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
extern  int_t no_dev_write(st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
extern  int_t no_dev_control(st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct);
extern  int_t no_dev_get_version(st_stream_ptr_t pStream, st_ver_info_ptr_t info_ptr);



/******************************************************************************
Function Name: devGetFromList
Description:   Function to check the device list for a device
Arguments:     IN  pszLinkName - Pointer to the device link name
Return value:  Pointer to the device or NULL if not found
******************************************************************************/


/******************************************************************************
* Function Name: R_SYS_InitIolib
* Description  : ANSI library low level initialisation function
* Arguments    : none
* Return Value : none
******************************************************************************/

extern void  R_DEVLINK_Init(void);

extern int_t R_DEVLINK_ListDevlinkTblContent(bool_t use_console, pst_comset_t pCom);

extern int_t R_DEVLINK_ListDeviceOpenHandles(bool_t use_console, pst_comset_t pCom);


/* Direct Interface */

extern int_t direct_open(char *pszFileName, int_t param);

extern int_t direct_close(int_t handle);

extern int_t direct_read(int handle, uint8_t *buff_ptr, uint32_t count);

extern int_t direct_write(int handle, uint8_t *buff_ptr, uint32_t count);

extern int_t direct_control(int handle, uint32_t ctlCode, void *pCtlStruct);

extern int_t direct_get_version(const char *p_driver_name, st_ver_info_t *info);

#if R_USE_ANSI_STDIO_MODE_CFG
/* Enable STDIO interface */

extern int_t low_open(char *pszFileName, int_t param);

extern int_t low_close(int_t handle);

extern int_t low_read(int handle, uint8_t *buff_ptr, uint32_t count);

extern int_t low_write(int handle, uint8_t *buff_ptr, uint32_t count);

extern int_t control(int handle, uint32_t ctlCode, void *pCtlStruct);

extern int_t get_version(const char *p_driver_name, st_ver_info_t *info);

extern int_t low_lseek(int iFileNumber, uint32_t lOffset, int iBase);

#endif

#ifdef __cplusplus
}
#endif

#endif /* DEVLINK_H_INCLUDED */

/*******************************************************************************************************************//**
 * @} (end addtogroup DS_DEVLINK_BASE)
 ***********************************************************************************************************************/


/******************************************************************************
End  Of File
******************************************************************************/
