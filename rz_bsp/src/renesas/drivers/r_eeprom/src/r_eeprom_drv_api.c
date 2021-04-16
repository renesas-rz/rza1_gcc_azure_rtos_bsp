/******************************************************************************
 * DISCLAIMER

 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized.

 * This software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.

 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
 * REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY
 * DISCLAIMED.

 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
 * FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
 * AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

 * Renesas reserves the right, without notice, to make changes to this
 * software and to discontinue the availability of this software.
 * By using this software, you agree to the additional terms and
 * conditions found by accessing the following link:
 * http://www.renesas.com/disclaimer
 ******************************************************************************/
/* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************
 * File Name    : r_eeprom_drv_api.c
 * Version      : 1.00
 * Device       : GNU
 * Tool-Chain   : GNUARM-NONE-EABI v14.02
 * H/W Platform : RSK+
 * Description  : Hardware interface and device driver for CAT24C04
 ******************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : dd.mm.yyyy 1.00    First Release
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "r_eeprom_drv_api.h"
#include "trace.h"
#include "dev_drv.h"
#include "r_os_abstraction_api.h"
#include "r_riic_drv_sc_cfg.h"

//#include "r_eeprom_driver.h"

#include "mcu_board_select.h"

/******************************************************************************
 Defines
 ******************************************************************************/

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

/* I2C slave address */
#define EEPROM_SLAVE_ADDRESS        (0xA2)

/* The Stream-It! board uses a CAT24C04 4Kbit EEPROM */

/* Define the device size in bytes - issue with IIC driver / peripheral means
 * that the size is reduced by two bytes because the device gets stuck in the transmit
 * state and holds SDA low preventing the host sending the stop condition.
 * This is probably a bug in the driver...
 * TODO: Look at EEPROM and IIC driver
 * */
#define EE_MEMORY_SIZE              (512UL)        // 512 for Stream-It! EEPROM (4 k bit -> 1/2 k byte)

#elif (TARGET_BOARD == TARGET_BOARD_RSK)

/* I2C slave address */
#define EEPROM_SLAVE_ADDRESS        (0xA0)

/* The RSK board uses a R1EX24016ASAS0A 2*8 Kbit EEPROM */

/* Define the device size in bytes - issue with IIC driver / peripheral means
 * that the size is reduced by two bytes because the device gets stuck in the transmit
 * state and holds SDA low preventing the host sending the stop condition.
 * This is probably a bug in the driver...
 * TODO: Look at EEPROM and IIC driver
 * */
#define EE_MEMORY_SIZE              (512UL)        // 512 for RSK EEPROM (2*8 k bit -> 1/2 k byte)

#endif /* TARGET_BOARD */

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
    #undef TRACE
    #define TRACE(x)
#endif

#ifndef MIN
    #define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/*****************************************************************************
 Function Macros
 ******************************************************************************/

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Typedefs
 ******************************************************************************/

/*****************************************************************************
 Function Prototypes
 ******************************************************************************/

static int romOpen (st_stream_ptr_t pStream);
static void romClose (st_stream_ptr_t pStream);
static int romRead (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
static int romWrite (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount);
static int romControl (st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct);
static int_t romGetVersion (st_stream_ptr_t p_stream, st_ver_info_t *p_ver_info);

static e_eeprom_error_t eeprom_write (uint16_t address, const void *data, size_t length);
static e_eeprom_error_t eeprom_read (uint16_t address, void *data, size_t length);

/*****************************************************************************
 Constant Data
 ******************************************************************************/

static const st_drv_info_t gs_hld_info =
{
{ ((STDIO_EEPROM_DRIVER_RZ_HLD_VERSION_MAJOR << 16) + STDIO_EEPROM_DRIVER_RZ_HLD_VERSION_MINOR) },
STDIO_EEPROM_DRIVER_RZ_HLD_BUILD_NUM,
STDIO_EEPROM_DRIVER_RZ_HLD_DRV_NAME };

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
/* Define the driver function table for this device */
const st_r_driver_t g24C04Driver =
{ "CAT24C04 Device Driver", romOpen, romClose, romRead, romWrite, romControl, romGetVersion };
#elif (TARGET_BOARD == TARGET_BOARD_RSK)
/* Define the driver function table for this device */
const st_r_driver_t gR1EX24016Driver =
{ "R1EX24016 Device Driver", romOpen, romClose, romRead, romWrite, romControl, romGetVersion };
#endif /* TARGET_BOARD */

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

static event_t gsp_eeprom_mutex;

/*****************************************************************************
 * Function Name: romOpen
 * Description  : Function to open the EEPROM
 * Arguments    : IN  pStream - Pointer to the file stream
 * Return Value : 0 for success otherwise -1
 ******************************************************************************/
static int romOpen (st_stream_ptr_t pStream)
{
    /* The extension pointer is used as the file index - set to zero */
    pStream->p_extension = NULL;

    /* Create a mutex to prevent simultaneous access to the EEPROM */
    if (NULL == gsp_eeprom_mutex)
    {
        gsp_eeprom_mutex = R_OS_CreateMutex();

        if (NULL == gsp_eeprom_mutex)
        {
            /* Failed to create mutex */
            return -1;
        }
    }

    /* Successful initialisation */

    return 0;
}
/*****************************************************************************
 End of function  romOpen
 ******************************************************************************/

/*****************************************************************************
 * Function Name: romClose
 * Description  : Function to close the EEPROM
 * Arguments    : IN  pStream - Pointer to the IOStream
 * Return Value : none
 ******************************************************************************/
static void romClose (st_stream_ptr_t pStream)
{
    (void) pStream;

    if (NULL != gsp_eeprom_mutex)
    {
        R_OS_DeleteMutex(gsp_eeprom_mutex);
        gsp_eeprom_mutex = NULL;
    }
}
/*****************************************************************************
 End of function  romClose
 ******************************************************************************/

/*****************************************************************************
 * Function Name: romRead
 * Description  : Function to read from the EEPROM
 * Arguments    : IN  pStream - Pointer to the IOStream
 *                IN  pbyBuffer - Pointer to the destination buffer
 *                IN  uiCount - The number of bytes to read
 * Return Value : The number of bytes read, or -1 on error
 ******************************************************************************/
static int romRead (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
{
    size_t stAddress = (size_t) pStream->p_extension;
    int return_value = -1;

    /* check reading is allowed */
    if (O_WRONLY != pStream->file_flag)
    {
        /* Check for the end of file */
        if (stAddress > EE_MEMORY_SIZE)
        {
            return EOF;
        }

        /* Limit the read to the end of memory */
        if ((stAddress + uiCount) > EE_MEMORY_SIZE)
        {
            uiCount = EE_MEMORY_SIZE - stAddress;
        }

        if (eeprom_read((uint16_t) stAddress, pbyBuffer, (size_t) uiCount) == EEPROM_NO_ERROR)
        {
            return_value = (int) uiCount;
        }
    }

    return return_value;
}
/*****************************************************************************
 End of function  romRead
 ******************************************************************************/

/*****************************************************************************
 * Function Name: romWrite
 * Description  : Function to write to the EEPROM
 * Arguments    : IN  pStream - Pointer to the IO stream
 *                IN  pbyBuffer - Pointer to the destination buffer
 *                IN  uiCount - The number of characters to write
 * Return Value : The number of characters written or -1 on error
 ******************************************************************************/
static int romWrite (st_stream_ptr_t pStream, uint8_t *pbyBuffer, uint32_t uiCount)
{
    size_t stAddress = (size_t) pStream->p_extension;
    int return_value = -1;

    /* check writing is allowed */
    if (O_RDONLY != pStream->file_flag)
    {
        /* Limit the write to the end of memory */
        if ((stAddress + uiCount) > EE_MEMORY_SIZE)
        {
            uiCount = EE_MEMORY_SIZE - stAddress;
        }

        if (eeprom_write((uint16_t) stAddress, pbyBuffer, (size_t) uiCount) == EEPROM_NO_ERROR)
        {
            return_value = (int) uiCount;
        }
        else
        {
            return_value = -1;
        }
    }

    return return_value;
}
/*****************************************************************************
 End of function  romWrite
 ******************************************************************************/

/*****************************************************************************
 * Function Name: romControl
 * Description  : Function to handle file specific controls
 * Arguments    : IN  pStream - Pointer to the file stream
 *                IN  ctlCode - The custom control code
 *                IN  pCtlStruct - Pointer to the custom control structure
 * Return Value : 0 for success and -1 on error
 ******************************************************************************/
static int romControl (st_stream_ptr_t pStream, uint32_t ctlCode, void *pCtlStruct)
{
    switch (ctlCode)
    {
        case CTL_FILE_SEEK:
        {
            if (pCtlStruct)
            {
                PFILESEEK pSeek = (PFILESEEK) pCtlStruct;
                if (pSeek->iBase == SEEK_SET)
                {
                    /* Don't set it if it is passed the end of storage */
                    if (pSeek->lOffset <= (signed) EE_MEMORY_SIZE)
                    {
                        /* Set the file file index to the desired offset */
                        pStream->p_extension = (void *) pSeek->lOffset;
                        pSeek->lResult = pSeek->lOffset;
                        return 0;
                    }
                }
                else if (pSeek->iBase == SEEK_CUR)
                {
                    /* Add on the offset to the current position */
                    size_t stPosition = (size_t) pStream->p_extension;
                    stPosition += (size_t) pSeek->lOffset;
                    if (stPosition <= EE_MEMORY_SIZE)
                    {
                        pStream->p_extension = (void *) stPosition;
                        pSeek->lResult = (signed) stPosition;
                        return 0;
                    }
                }
                else if (pSeek->iBase == SEEK_END)
                {
                    size_t stPosition = (size_t) (EE_MEMORY_SIZE + (size_t) pSeek->lOffset);

                    if (stPosition <= EE_MEMORY_SIZE)
                    {
                        /* Set the file pointer to the end of memory */
                        pStream->p_extension = (void *) stPosition;
                        pSeek->lResult = (signed) stPosition;
                        return 0;
                    }
                }
            }
            break;
        }

        case CTL_FILE_SIZE:
        {
            if (pCtlStruct)
            {
                *((uint32_t *) pCtlStruct) = EE_MEMORY_SIZE;
                return 0;
            }
            break;
        }

        default:
        {
            return -1;
        }
    }

    return -1;
}
/*****************************************************************************
 End of function  romControl
 ******************************************************************************/

/*******************************************************************************
 * Function Name: fileGetVersion
 * Description  : Provides build information even if driver fails to open
 *                version information is updated by developer
 * Arguments    : none
 * Return Value : DEVDRV_SUCCESS (never fails)
 ******************************************************************************/
static int_t romGetVersion (st_stream_ptr_t p_stream, st_ver_info_t *p_ver_info)
{
    (void) p_stream;

    p_ver_info->hld.version.sub.major = gs_hld_info.version.sub.major;
    p_ver_info->hld.version.sub.minor = gs_hld_info.version.sub.minor;
    p_ver_info->hld.build = gs_hld_info.build;
    p_ver_info->hld.p_szdriver_name = gs_hld_info.p_szdriver_name;

    return (DEVDRV_SUCCESS);
}
/*******************************************************************************
 End of function fileGetVersion
 ******************************************************************************/

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)

/***********************************************************************************************************************
 * Function Name: eeprom_write
 * Description  : Write data to EEPROM memory
 * Arguments    : uint8_t address - memory location where to write data
 *              : const void *data - pointer to the data to be written
 *              : size_t length - number of data bytes
 * Return Value : error_t - error code
 ***********************************************************************************************************************/
static e_eeprom_error_t eeprom_write (uint16_t address, const void *data, size_t length)
{
    e_eeprom_error_t error = EEPROM_ERROR;
    size_t block_length;
    const uint8_t *p_data;
    int_t iic0_handle = ( -1);
    st_r_drv_riic_create_t riic_clock;

    /* setup riic configuration */
    riic_clock.frequency = RIIC_FREQUENCY_100KHZ;

    if (NULL != gsp_eeprom_mutex)
    {
        /* Acquire exclusive access to the EEPROM */
        R_OS_AcquireMutex(gsp_eeprom_mutex);

        /* Cast data pointer */
        p_data = (const uint8_t *) data;

        /* open the I2C channel0 driver */
        iic0_handle = open(DEVICE_INDENTIFIER "iic0", O_RDWR);

        /* Test if riic channel was opened successfully */
        if (0 <= iic0_handle)
        {
            /* Configure the riic channel */
            if (0 == control(iic0_handle, CTL_RIIC_CREATE, &riic_clock))
            {
                /* i2c access control structure */
                st_r_drv_riic_config_t i2c_write;

                /* configure i2c device address on channel */
                i2c_write.device_address = EEPROM_SLAVE_ADDRESS;

                /* riic successfully created and configured */
                error = EEPROM_NO_ERROR;

                /* Write data */
                while ((length > 0) && (EEPROM_NO_ERROR == error))
                {

                    /* Prevent page write operations that would attempt to cross a page boundary */
                    block_length = (uint8_t) (8 - (address % 8));

                    /* Limit the number of bytes to write */
                    block_length = MIN(block_length, length);

                    /* configure eeprom memory address and number of bytes */
                    i2c_write.sub_address = address;
                    i2c_write.number_of_bytes = block_length;

                    /* buffer is not declared as const, so cast to char * to avoid warning */
                    i2c_write.p_data_buffer = (uint8_t *) p_data;

                    if (0 != control(iic0_handle, CTL_RIIC_WRITE, &i2c_write))
                    {
                        error = EEPROM_ERROR;
                    }
                    else
                    {
                        /* Advance data pointer */
                        p_data += block_length;

                        /* Increment word address */
                        address = (uint8_t) (address + block_length);

                        /* Remaining bytes to be written */
                        length -= block_length;

                        /* allow some time for EEPROM to complete write process */
                        R_OS_TaskSleep(5);
                    }
                }
            }
        }

        /* close the I2C channel0 driver */
        close(iic0_handle);

        /* Release exclusive access to the EEPROM */
        R_OS_ReleaseMutex(gsp_eeprom_mutex);
    }

    /* Return status code */
    return (error);
}
/*****************************************************************************
 End of function eeprom_write
 *****************************************************************************/

/***********************************************************************************************************************
 * Function Name: eeprom_read
 * Description  : Read data from EEPROM memory
 * Arguments    : uint8_t address - memory location where to read data
 *              : const void *data - pointer to the buffer where to copy the data
 *              : size_t length - number of data bytes to read
 * Return Value : error_t - error code
 ***********************************************************************************************************************/
static e_eeprom_error_t eeprom_read (uint16_t address, void *data, size_t length)
{
    e_eeprom_error_t error = EEPROM_ERROR;
    uint8_t *p_data;
    int_t iic0_handle = ( -1);
    st_r_drv_riic_create_t riic_clock;

    /* setup riic configuration */
    riic_clock.frequency = RIIC_FREQUENCY_100KHZ;

    if (NULL != gsp_eeprom_mutex)
    {
        /* i2c access control structure */
        st_r_drv_riic_config_t i2c_read;

        /* configure eeprom device address on i2c channel */
        i2c_read.device_address = EEPROM_SLAVE_ADDRESS;

        /* Acquire exclusive access to the EEPROM */
        R_OS_AcquireMutex(gsp_eeprom_mutex);

        /* Cast data pointer */
        p_data = (uint8_t *) data;

        /* configure eeprom memory address and number of bytes */
        i2c_read.sub_address = address;
        i2c_read.number_of_bytes = length;
        i2c_read.p_data_buffer = p_data;

        /* open the I2C channel0 driver */
        iic0_handle = open(DEVICE_INDENTIFIER "iic0", O_RDWR);

        /* Test if riic channel was opened successfully */
        if (0 <= iic0_handle)
        {
            /* Create the I2C channel with the 100kHz clock */
            if (0 == control(iic0_handle, CTL_RIIC_CREATE, &riic_clock))
            {
                /* riic successfully created and configured */

                /* read data from eeprom */
                if (0 == control(iic0_handle, CTL_RIIC_READ, &i2c_read))
                {
                    error = EEPROM_NO_ERROR;
                }
            }
        }

        /* close the I2C channel0 driver */
        close(iic0_handle);

        /* Release exclusive access to the EEPROM */
        R_OS_ReleaseMutex(gsp_eeprom_mutex);
    }

    /* Return status code */
    return (error);
}
/*****************************************************************************
 End of function eeprom_read
 *****************************************************************************/

#elif (TARGET_BOARD == TARGET_BOARD_RSK)

/***********************************************************************************************************************
 * Function Name: eeprom_write
 * Description  : Write data to EEPROM memory
 * Arguments    : uint8_t address - memory location where to write data
 *              : const void *data - pointer to the data to be written
 *              : size_t length - number of data bytes
 * Return Value : error_t - error code
 ***********************************************************************************************************************/
static e_eeprom_error_t eeprom_write (uint16_t address, const void *data, size_t length)
{
    e_eeprom_error_t error = EEPROM_ERROR;
    size_t block_length;
    const uint8_t *p_data;
    int_t iic3_handle = ( -1);
    st_r_drv_riic_create_t riic_clock;

    /* setup riic configuration */
    riic_clock.frequency = RIIC_FREQUENCY_100KHZ;

    if (NULL != gsp_eeprom_mutex)
    {
        /* Acquire exclusive access to the EEPROM */
        R_OS_AcquireMutex(gsp_eeprom_mutex);

        /* Cast data pointer */
        p_data = (const uint8_t *) data;

        /* open the I2C channel0 driver */
        iic3_handle = open(DEVICE_INDENTIFIER "iic3", O_RDWR);

        /* Test if riic channel was opened successfully */
        if (0 <= iic3_handle)
        {
            /* Configure the riic channel */
            if (0 == control(iic3_handle, CTL_RIIC_CREATE, &riic_clock))
            {
                /* i2c access control structure */
                st_r_drv_riic_config_t i2c_write;

                /* configure i2c device address on channel */
                i2c_write.device_address = EEPROM_SLAVE_ADDRESS;

                /* riic successfully created and configured */
                error = EEPROM_NO_ERROR;

                /* Write data */
                while ((length > 0) && (EEPROM_NO_ERROR == error))
                {

                    /* Prevent page write operations that would attempt to cross a page boundary */
                    block_length = (uint8_t) (8 - (address % 8));

                    /* Limit the number of bytes to write */
                    block_length = MIN(block_length, length);

                    /* configure eeprom memory address and number of bytes */
                    i2c_write.sub_address = address;
                    i2c_write.number_of_bytes = block_length;

                    /* buffer is not declared as const, so cast to char * to avoid warning */
                    i2c_write.p_data_buffer = (uint8_t *) p_data;

                    if (0 != control(iic3_handle, CTL_RIIC_WRITE, &i2c_write))
                    {
                        error = EEPROM_ERROR;
                    }
                    else
                    {
                        /* Advance data pointer */
                        p_data += block_length;

                        /* Increment word address */
                        address = (uint8_t) (address + block_length);

                        /* Remaining bytes to be written */
                        length -= block_length;

                        /* allow some time for EEPROM to complete write process */
                        R_OS_TaskSleep(5);
                    }
                }
            }
        }

        /* close the I2C channel0 driver */
        close(iic3_handle);

        /* Release exclusive access to the EEPROM */
        R_OS_ReleaseMutex(gsp_eeprom_mutex);
    }

    /* Return status code */
    return (error);
}
/*****************************************************************************
 End of function eeprom_write
 *****************************************************************************/

/***********************************************************************************************************************
 * Function Name: eeprom_read
 * Description  : Read data from EEPROM memory
 * Arguments    : uint8_t address - memory location where to read data
 *              : const void *data - pointer to the buffer where to copy the data
 *              : size_t length - number of data bytes to read
 * Return Value : error_t - error code
 ***********************************************************************************************************************/
static e_eeprom_error_t eeprom_read (uint16_t address, void *data, size_t length)
{
    e_eeprom_error_t error = EEPROM_ERROR;
    uint8_t *p_data;
    int_t iic3_handle = ( -1);
    st_r_drv_riic_create_t riic_clock;

    /* setup riic configuration */
    riic_clock.frequency = RIIC_FREQUENCY_100KHZ;

    if (NULL != gsp_eeprom_mutex)
    {
        /* i2c access control structure */
        st_r_drv_riic_config_t i2c_read;

        /* configure eeprom device address on i2c channel */
        i2c_read.device_address = EEPROM_SLAVE_ADDRESS;

        /* Acquire exclusive access to the EEPROM */
        R_OS_AcquireMutex(gsp_eeprom_mutex);

        /* Cast data pointer */
        p_data = (uint8_t *) data;

        /* configure eeprom memory address and number of bytes */
        i2c_read.sub_address = address;
        i2c_read.number_of_bytes = length;
        i2c_read.p_data_buffer = p_data;

        /* open the I2C channel0 driver */
        iic3_handle = open(DEVICE_INDENTIFIER "iic3", O_RDWR);

        /* Test if riic channel was opened successfully */
        if (0 <= iic3_handle)
        {
            /* Create the I2C channel with the 100kHz clock */
            if (0 == control(iic3_handle, CTL_RIIC_CREATE, &riic_clock))
            {
                /* riic successfully created and configured */

                /* read data from eeprom */
                if (0 == control(iic3_handle, CTL_RIIC_READ, &i2c_read))
                {
                    error = EEPROM_NO_ERROR;
                }
            }
        }

        /* close the I2C channel0 driver */
        close(iic3_handle);

        /* Release exclusive access to the EEPROM */
        R_OS_ReleaseMutex(gsp_eeprom_mutex);
    }

    /* Return status code */
    return (error);
}
/*****************************************************************************
 End of function eeprom_read
 *****************************************************************************/

#endif /* TARGET_BOARD */

/*****************************************************************************
 End  Of File
 ******************************************************************************/
