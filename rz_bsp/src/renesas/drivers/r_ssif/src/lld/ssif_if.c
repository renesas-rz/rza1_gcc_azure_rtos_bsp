/*******************************************************************************
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
* Copyright (C) 2013-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name   : ssif_if.c
* $Rev: 8681 $
* $Date:: 2018-06-28 14:10:37 +0100#$
* Description : SSIF driver interface functions
******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include "r_os_abstraction_api.h"
#include "fcntl.h"
#include "ssif.h"
#include "mcu_board_select.h"

/*******************************************************************************
Typedef definitions
*******************************************************************************/


/*******************************************************************************
Macro definitions
*******************************************************************************/


/*******************************************************************************
Exported global variables (to be accessed by other files)
*******************************************************************************/


/*******************************************************************************
Private global variables and functions
*******************************************************************************/

static int_t R_SSIF_UnInit(void* const driver_instance, int32_t* const p_errno);
static int_t R_SSIF_Open(void* const p_driver_instance, const char_t* const p_path_name, const int_t flags, const int_t mode, int32_t* const p_errno);
static int_t R_SSIF_Close(void* const p_fd, int32_t* const p_errno);
static int_t R_SSIF_Ioctl(void* const p_fd, const int_t request, void* const p_buf, int32_t* const p_errno);
static int_t R_SSIF_WriteAsync(void* const p_fd, AIOCB* const p_aio, int32_t* const p_errno);
static int_t R_SSIF_ReadAsync(void* const p_fd, AIOCB* const p_aio, int32_t* const p_errno);
static int_t R_SSIF_Cancel(void* const p_fd, AIOCB* p_aio, int32_t* const p_errno);

static size_t SSIF_StrnLen(const char_t p_str[], const size_t maxlen);
static int32_t SSIF_Strncmp(const char_t p_str1[], const char_t p_str2[], const uint32_t maxlen);
static void   SSIF_SetErrCode(const int_t error_code, int32_t* const p_errno);

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
* Function Name: R_SSIF_MakeCbTbl
* @brief         Make the SSIF driver function callback table
*
*                Description:<br>
*
* @param         none
* @retval        pointer of SSIF driver function callback table
******************************************************************************/
/* ->IPA M1.1.1 : This is liblary funciotn that is called from other module. */
/******************************************************************************
* Function Name: R_SSIF_SWLtoLen
* @brief         Convert SSICR:SWL bits to system word length
*
*                Description:<br>
*
* @param[in]     ssicr_swl  :SSICR register SWL field value(0 to 7)
* @retval        8 to 256   :system word length(byte)
******************************************************************************/
int_t R_SSIF_SWLtoLen(const ssif_chcfg_system_word_t ssicr_swl)
{
    return SSIF_SWLtoLen(ssicr_swl);
}

/******************************************************************************
Private functions
******************************************************************************/

/******************************************************************************
* Function Name: R_SSIF_Init
* @brief         Initialise the SSIF driver.
*
*                Description:<br>
*
* @param[in]     config_data :pointer of several parameters array per channels
* @param[in,out] p_errno     :pointer of error code
* @retval        not ERRROR  :driver instance.
* @retval        IOIF_EERROR      :Failure.
******************************************************************************/
///* ->MISRA 16.7, IPA M1.11.1 : This is IOIF library API type definition that can't be modified. */
//static void* R_SSIF_Init(void* const config_data, int32_t* const p_errno)
///* <-MISRA 16.7, IPA M1.11.1 */
//{
//    int_t ercd;
//    void* p_ret = (void*)IOIF_EERROR;
//
//    if (NULL == config_data)
//    {
//        ercd = IOIF_EFAULT;
//    }
//    else if (SSIF_DRVSTS_UNINIT != g_ssif_info_drv.drv_stat)
//    {
//        ercd = IOIF_EBUSY;
//    }
//    else
//    {
//        g_ssif_info_drv.drv_stat = SSIF_DRVSTS_INIT;
//
//        ercd = SSIF_Initialise((ssif_channel_cfg_t*)config_data);
//
//        if (IOIF_ESUCCESS == ercd)
//        {
//            p_ret = (void*)&g_ssif_info_drv;
//        }
//        else
//        {
//            g_ssif_info_drv.drv_stat = SSIF_DRVSTS_UNINIT;
//        }
//    }
//
//    SSIF_SetErrCode(ercd, p_errno);
//
//    return p_ret;
//}

/******************************************************************************
* Function Name: R_SSIF_UnInit
* @brief         Uninitialise the SSIF deiver.
*
*                Description:<br>
*
* @param[in,out] driver_instance :driver instance which was returned by<br>
                                  R_SSIF_Init
* @param[in,out] p_errno         :pointer of error code
* @retval        IOIF_ESUCCESS        :Success.
* @retval        IOIF_EERROR          :Failure.
******************************************************************************/
static int_t R_SSIF_UnInit(void* const driver_instance, int32_t* const p_errno)
{
    int_t   ercd;
    int_t   ret = IOIF_ESUCCESS;
    ssif_info_drv_t* const p_info_drv   = driver_instance;

    if (NULL == p_info_drv)
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        if (SSIF_DRVSTS_INIT != p_info_drv->drv_stat)
        {
            ercd = IOIF_EFAULT;
        }
        else
        {
            ercd = SSIF_UnInitialise();
            p_info_drv->drv_stat = SSIF_DRVSTS_UNINIT;
        }
    }

    if (IOIF_ESUCCESS != ercd)
    {
        ret = IOIF_EERROR;
    }
    SSIF_SetErrCode(ercd, p_errno);

    return ret;
}

/******************************************************************************
* Function Name: R_SSIF_Open
* @brief         Open an SSIF channel
*
*                Description:<br>
*
* @param[in,out] p_drv_instance :driver instance which was returned by<br>
                                 R_SSIF_Init
* @param[in]     p_path_name    :string of channel
* @param[in]     flags          :access mode whether the channel is opened<br>
                                 for a read or a write
* @param[in]     mode           :not used
* @param[in,out] p_errno        :pointer of error code
* @retval        not IOIF_EERROR     :channel handle
* @retval        IOIF_EERROR         :Failure.
******************************************************************************/
static int_t R_SSIF_Open(void* const p_driver_instance, const char_t* const p_path_name, const int_t flags, const int_t mode, int32_t* const p_errno)
{
    uint32_t ssif_ch;
    void* p_channelHandle;
    size_t len;
    size_t req_path_len;
    ssif_info_drv_t* const p_info_drv = p_driver_instance;
    ssif_info_ch_t* p_info_ch = NULL;
    int_t ret;
    int_t ercd = IOIF_ESUCCESS;
    osStatus os_ercd;
    int32_t os_ret;
    static const char_t* const ch_name_string[SSIF_NUM_CHANS] =
    {
        SSIF_CHSTR_0,
        SSIF_CHSTR_1,
        SSIF_CHSTR_2,
#if (TARGET_RZA1 <= TARGET_RZA1LU)
        SSIF_CHSTR_3
#else
        SSIF_CHSTR_3,
        SSIF_CHSTR_4,
        SSIF_CHSTR_5
#endif
    };

    (void)mode; /* Unused Argument */

    if ((NULL == p_info_drv) || (NULL == p_path_name))
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        req_path_len = strlen(p_path_name);
        if (0u == req_path_len)
        {
            ercd = IOIF_ENOENT;
        }

        if (IOIF_ESUCCESS == ercd)
        {
            if (SSIF_DRVSTS_INIT != p_info_drv->drv_stat)
            {
                ercd = IOIF_EFAULT;
            }
        }

        if (IOIF_ESUCCESS == ercd)
        {
            /* Search the same pathname */
            for (ssif_ch = 0u; (ssif_ch < SSIF_NUM_CHANS) && (p_info_ch == NULL); ssif_ch++)
            {
                len = SSIF_StrnLen(ch_name_string[ssif_ch], SSIF_MAX_PATH_LEN);

                if (req_path_len < len)
                {
                    len = req_path_len;
                }

                if (0 == SSIF_Strncmp(p_path_name, ch_name_string[ssif_ch], len))
                {
                    /* found a match */
                    p_info_ch = &p_info_drv->info_ch[ssif_ch];
                }
            }
        }
    }

    if (NULL == p_info_ch)
    {
        ercd = IOIF_ENOENT;
    }
    else
    {
        if (IOIF_ESUCCESS == ercd)
        {
            if (false == p_info_ch->enabled)
            {
                ercd = IOIF_ENOTSUP;
            }
        }

        if (IOIF_ESUCCESS == ercd)
        {
            if (SSIF_CHSTS_INIT != p_info_ch->ch_stat)
            {
                ercd = IOIF_EBADF;
            }
        }

        if (IOIF_ESUCCESS == ercd)
        {
            /* ->MISRA 10.6 : This macro is defined by CMSIS-RTOS that can't be modified. */
            os_ret = R_OS_WaitForSemaphore( p_info_ch->sem_access, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
            /* <-MISRA 10.6 */

            if ((-1) == os_ret)
            {
                ercd = IOIF_EFAULT;
            }
            else
            {
                p_info_ch->openflag = flags;
                p_info_ch->p_aio_tx_curr = NULL;
                p_info_ch->p_aio_rx_curr = NULL;

                ercd = SSIF_EnableChannel(p_info_ch);

                if (IOIF_ESUCCESS == ercd)
                {
                    p_info_ch->ch_stat = SSIF_CHSTS_OPEN;
                }
            }
            os_ercd = R_OS_ReleaseSemaphore( p_info_ch->sem_access);
            if (osOK != os_ercd)
            {
                ercd = IOIF_EFAULT;
            }
        }
    }

    if (IOIF_ESUCCESS != ercd)
    {
        ret = IOIF_EERROR;   /* IOIF_EERROR(-1) */
    }
    else
    {
        p_channelHandle = (void*)p_info_ch;
        ret = (int_t)p_channelHandle;
    }
    SSIF_SetErrCode(ercd, p_errno);

    return ret;
}

/******************************************************************************
* Function Name: R_SSIF_Close
* @brief         Close an SSIF channel.
*
*                Description:<br>
*
* @param[in,out] p_fd       :channel handle which was returned by R_SSIF_Open
* @param[in,out] p_errno    :pointer of error code
* @retval        IOIF_ESUCCESS   :Success.
* @retval        IOIF_EERROR     :Failure.
******************************************************************************/
static int_t R_SSIF_Close(void* const p_fd, int32_t* const p_errno)
{
    ssif_info_ch_t* const p_info_ch = p_fd;
    int_t ret = IOIF_ESUCCESS;
    int_t ercd;
    osStatus os_ercd;
    int32_t os_ret;

    if (NULL == p_info_ch)
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        /* ->MISRA 10.6 : This macro is defined by CMSIS-RTOS that can't be modified. */
        /* Get semaphore to access the channel data */
        os_ret = R_OS_WaitForSemaphore( p_info_ch->sem_access, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
        /* <-MISRA 10.6 */

        if ((-1) == os_ret)
        {
            ercd = IOIF_EFAULT;
        }
        else
        {
            if (SSIF_CHSTS_OPEN != p_info_ch->ch_stat)
            {
                ercd = IOIF_EFAULT;
            }
            else
            {
                SSIF_PostAsyncCancel(p_info_ch, NULL);

                ercd = SSIF_DisableChannel(p_info_ch);

                if (IOIF_ESUCCESS == ercd)
                {
                    p_info_ch->ch_stat = SSIF_CHSTS_INIT;
                }
            }

            /* Relese semaphore */
            os_ercd = R_OS_ReleaseSemaphore( p_info_ch->sem_access);

            if (osOK != os_ercd)
            {
                ercd = IOIF_EFAULT;
            }
        }
    }

    if (IOIF_ESUCCESS != ercd)
    {
        ret = IOIF_EERROR;   /* IOIF_EERROR(-1) */
    }
    SSIF_SetErrCode(ercd, p_errno);

    return ret;
}

/******************************************************************************
* Function Name: R_SSIF_Ioctl
* @brief         IOCTL function of the SSIF deiver
*
*                Description:<br>
*
* @param[in,out] p_fd       :channel handle which was returned by R_SSIF_Open
* @param[in]     request    :IOCTL request code
* @param[in,out] p_buf      :Meaning depends upon request.
* @param[in,out] p_errno    :pointer of error code
* @retval        IOIF_ESUCCESS   :Success.
* @retval        IOIF_EERROR     :Failure.
******************************************************************************/
static int_t R_SSIF_Ioctl(void* const p_fd, const int_t request, void* const p_buf, int32_t* const p_errno)
{
    ssif_info_ch_t* const p_info_ch = p_fd;
    int_t   ret = IOIF_ESUCCESS;
    int_t ercd = IOIF_ESUCCESS;
    osStatus os_ercd;
    int32_t os_ret;

    if (NULL == p_info_ch)
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        if (SSIF_CHSTS_OPEN != p_info_ch->ch_stat)
        {
            ercd = IOIF_EFAULT;
        }
        else
        {
            /* ->MISRA 10.6 : This macro is defined by CMSIS-RTOS that can't be modified. */
            os_ret = R_OS_WaitForSemaphore( p_info_ch->sem_access, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
            /* <-MISRA 10.6 */

            if ((-1) == os_ret)
            {
                ercd = IOIF_EFAULT;
            }

            if (IOIF_ESUCCESS == ercd)
            {
                switch (request)
                {
                    case SSIF_CONFIG_CHANNEL:
                    {
                        if (NULL == p_buf)
                        {
                            ercd = IOIF_EFAULT;
                        }
                        else
                        {
                            ssif_channel_cfg_t* const ch_info = (ssif_channel_cfg_t*)p_buf;
                            ercd = SSIF_IOCTL_ConfigChannel(p_info_ch, ch_info);
                        }
                        break;
                    }

                    case SSIF_GET_STATUS:
                    {
                        if (NULL == p_buf)
                        {
                            ercd = IOIF_EFAULT;
                        }
                        else
                        {
                            ercd = SSIF_IOCTL_GetStatus(p_info_ch, (uint32_t*)p_buf);
                        }
                        break;
                    }

                    default:
                    {
                        ercd = IOIF_EINVAL;
                        break;
                    }
                } /* switch */
            }
        }

        os_ercd = R_OS_ReleaseSemaphore( p_info_ch->sem_access);
        if (osOK != os_ercd)
        {
            ercd = IOIF_EFAULT;
        }
    }

    if (IOIF_ESUCCESS != ercd)
    {
        ret = IOIF_EERROR;   /* IOIF_EERROR(-1) */
    }
    SSIF_SetErrCode(ercd, p_errno);

    return ret;
}

/******************************************************************************
* Function Name: R_SSIF_WriteAsync
* @brief         Enqueue asynchronous write request
*
*                Description:<br>
*
* @param[in,out] p_fd       :channel handle which was returned by R_SSIF_Open
* @param[in]     p_aio      :aio control block of write request
* @param[in,out] p_errno    :pointer of error code
* @retval        IOIF_ESUCCESS   :Success.
* @retval        IOIF_EERROR     :Failure.
******************************************************************************/
static int_t R_SSIF_WriteAsync(void* const p_fd, AIOCB* const p_aio, int32_t* const p_errno)
{
    ssif_info_ch_t* const p_info_ch = p_fd;
    int_t   ret = IOIF_ESUCCESS;
    int_t ercd = IOIF_ESUCCESS;

    if ((NULL == p_info_ch) || (NULL == p_aio))
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        if (((uint32_t)O_RDONLY) == ((uint32_t)p_info_ch->openflag & O_ACCMODE))
        {
            ercd = IOIF_EACCES;
        }
        else if (0u == p_aio->aio_nbytes)
        {
            ercd = IOIF_EINVAL;
        }
        else
        {
            p_aio->aio_return = SSIF_ASYNC_W;
            SSIF_PostAsyncIo(p_info_ch, p_aio);
        }
    }

    if (IOIF_ESUCCESS != ercd)
    {
        ret = IOIF_EERROR;   /* IOIF_EERROR(-1) */
    }
    SSIF_SetErrCode(ercd, p_errno);

    return ret;
}

/******************************************************************************
* Function Name: R_SSIF_ReadAsync
* @brief         Enqueue asynchronous read request
*
*                Description:<br>
*
* @param[in,out] p_fd       :channel handle which was returned by R_SSIF_Open
* @param[in]     p_aio      :aio control block of read request
* @param[in,out] p_errno    :pointer of error code
* @retval        IOIF_ESUCCESS   :Success.
* @retval        IOIF_EERROR     :Failure.
******************************************************************************/
static int_t R_SSIF_ReadAsync(void* const p_fd, AIOCB* const p_aio, int32_t* const p_errno)
{
    ssif_info_ch_t* const p_info_ch = p_fd;
    int_t   ret = IOIF_ESUCCESS;
    int_t ercd = IOIF_ESUCCESS;

    if ((NULL == p_info_ch) || (NULL == p_aio))
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
#if (TARGET_RZA1 <= TARGET_RZA1LU)
        if ((O_WRONLY == ((uint32_t)p_info_ch->openflag & O_ACCMODE)))
#else
        if ((O_WRONLY == ((uint32_t)p_info_ch->openflag & O_ACCMODE))
            || (SSIF_CFG_ENABLE_ROMDEC_DIRECT
                == p_info_ch->romdec_direct.mode))
#endif
        {
            ercd = IOIF_EACCES;
        }
        else if (0u == p_aio->aio_nbytes)
        {
            ercd = IOIF_EINVAL;
        }
        else
        {
            p_aio->aio_return = SSIF_ASYNC_R;
            SSIF_PostAsyncIo(p_info_ch, p_aio);
        }
    }

    if (IOIF_ESUCCESS != ercd)
    {
        ret = IOIF_EERROR;   /* IOIF_EERROR(-1) */
    }
    SSIF_SetErrCode(ercd, p_errno);

    return ret;
}

/******************************************************************************
* Function Name: R_SSIF_Cancel
* @brief         Cancel read or write request(s)
*
*                Description:<br>
*
* @param[in,out] p_fd       :channel handle which was returned by R_SSIF_Open
* @param[in]     p_aio      :aio control block to cancel or NULL to cancel all.
* @param[in,out] p_errno    :pointer of error code
* @retval        IOIF_ESUCCESS   :Success.
* @retval        IOIF_EERROR     :Failure.
******************************************************************************/
static int_t R_SSIF_Cancel(void* const p_fd, AIOCB* const p_aio, int32_t* const p_errno)
{
    ssif_info_ch_t* const p_info_ch = p_fd;
    int_t ret = IOIF_ESUCCESS;
    int_t ercd = IOIF_ESUCCESS;
    osStatus os_ercd;
    int32_t os_ret;

    if (NULL == p_info_ch)
    {
        ercd = IOIF_EFAULT;
    }
    else
    {
        /* ->MISRA 10.6 : This macro is defined by CMSIS-RTOS that can't be modified. */
        /* Get semaphore to access the channel data */
        os_ret = R_OS_WaitForSemaphore( p_info_ch->sem_access, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
        /* <-MISRA 10.6 */

        if ((-1) == os_ret)
        {
            ercd = IOIF_EFAULT;
        }
        else
        {
            if (SSIF_CHSTS_OPEN != p_info_ch->ch_stat)
            {
                ercd = IOIF_EFAULT;
            }
            else
            {
                SSIF_PostAsyncCancel(p_info_ch, p_aio);
            }

            os_ercd = R_OS_ReleaseSemaphore( p_info_ch->sem_access);

            if (osOK != os_ercd)
            {
                ercd = IOIF_EFAULT;
            }
        }
    }

    if (IOIF_ESUCCESS != ercd)
    {
        ret = IOIF_EERROR;   /* IOIF_EERROR(-1) */
    }
    SSIF_SetErrCode(ercd, p_errno);

    return ret;
}

/******************************************************************************
* Function Name: SSIF_StrnLen
* @brief         computes the length of the string
*
*                Description:<br>
*
* @param[in]     p_str      :pointer of string.
* @param[in]     maxlen     :maximum length of inspection
* @retval        < maxlen   :number of characters in the string
* @retval        maxlen     :string is longer than maxlen
******************************************************************************/
static size_t SSIF_StrnLen(const char_t p_str[], const size_t maxlen)
{
    size_t len;

    if (NULL == p_str)
    {
        len = 0;
    }
    else
    {
        for (len = 0; len < maxlen; len++)
        {
            if ((int_t)p_str[len] == '\0')
            {
                break;
            }
        }
    }

    return len;
}

/******************************************************************************
* Function Name: SSIF_Strncmp
* @brief         Compare two strings
*
*                Description:<br>
*
* @param[in]     p_str1     :pointer of string1
* @param[in]     p_str2     :pointer of string2
* @param[in]     maxlen     :maximum length of comparison
* @retval        zero       :strings are same.
* @retval        non zero   :strings are different.
******************************************************************************/
static int32_t SSIF_Strncmp(const char_t p_str1[], const char_t p_str2[], const uint32_t maxlen)
{
    int32_t result = 0;
    uint32_t index;

    if ((NULL == p_str1) || (NULL == p_str2))
    {
        result = -1;
    }
    else
    {
        for (index = 0; index < maxlen; index++)
        {
            /* compare charctor */
            result = ((int_t)p_str1[index]) - ((int_t)p_str2[index]);
            if ((result != 0)
                || ((int_t)p_str1[index] == '\0')
                || ((int_t)p_str2[index] == '\0'))
            {
                /* "charactor mismatch" or "end of string" */
                break;
            }
        }
    }

    return result;
}

/******************************************************************************
* Function Name: SSIF_SetErrCode
* @brief         Set error code to error code pointer.
*
*                Description:<br>
*                If error code pointer is NULL, do nothing.
* @param[in]     error_code :Error code.
* @param[in,out] p_errno    :Pointer of set error code.
* @retval        none
******************************************************************************/
static void SSIF_SetErrCode(const int_t error_code, int32_t* const p_errno)
{
    if (NULL != p_errno)
    {
        *p_errno = error_code;
    }

    return;
}

