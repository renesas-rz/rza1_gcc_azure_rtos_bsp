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
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
 *******************************************************************************
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     aioif.h
 * @brief          ssif low level header file
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
/*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef AIOIF_H
    #define AIOIF_H

/*************************************************************************//**
 * @ingroup R_SW_PKG_93_SSIF_API
 * @defgroup R_SW_PKG_93_SSIF_LLD_AIOIF SSIF Low Level Driver API
 * @brief Internal low level SSIF driver component
 *
 * @anchor R_SW_PKG_93_SSIF_LLD_AIOIF_SUMMARY
 * @par Summary
 * @brief This interface defines the functionality that the low level driver
 * must implement for compatibility with the high level driver.
 *
 * @anchor R_SW_PKG_93_SSIF_LLD_AIOIF_LIMITATIONS
 * @par Known Limitations
 * @ref NONE_YET
 *
 * @anchor R_SW_PKG_93_SSIF_LLD_AIOIF_INSTANCES
 * @par Known Implementations
 * @ref RENESAS_APPLICATION_SOFTWARE_PACKAGE
 * @{
 *****************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
    #include "compiler_settings.h"
    #include "cmsis_os.h"
    #include "r_os_abstraction_api.h"

/*******************************************************************************
 Macro definitions
 *******************************************************************************/
    #define AHF_CREATESEM   0x1 /* create a semaphore for locking */ /* Source Merge 08-82 */
    #define AHF_LOCKSEM     0x2 /* use a semaphore for locking */    /* Source Merge 08-82 */
    #define AHF_LOCKINT     0x4 /* use interrupts for locking */

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef struct AHF_S
{
        struct aiocb *head;
        struct aiocb *tail;
        uint32_t     flags;
        event_t      semid; /* Source Merge 08-81 */
        osMutexDef_t *p_cmtx;
        uint32_t     saved_int_mask;
} AHF_S;

/* AHF (Asynchronous Helper Functions */
/**
 * @brief Creates an empty aio control queue pointer. Creates a mutex if AHF_CREATESEM bit flag is set.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] f    - flag indicating that at semaphore is to be created.
 * @return    0 on success.   negative error code on error.
 **/
int32_t ahf_create (AHF_S * const ahf, const uint32_t f); /* Source Merge 08-83 */

/**
 * @brief Delete aio control block mutex (if it exists). Note: Does not delete the aio control block queue.
 * @param[in] ahf  - aio queue structure pointer.
 **/
void ahf_destroy (AHF_S const * const ahf);

/**
 * @brief Add an aio control block to the queue.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] aio  - pointer to queue structure.
 **/
void ahf_addtail (AHF_S * const ahf, struct aiocb * const aio); /* Source Merge 08-04 */

/**
 * @brief Remove an aio control block from the queue.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] aio  - pointer to queue structure.
 * @return    aio control block, or NULL if queue is empty.
 **/
struct aiocb *ahf_removehead (AHF_S * const ahf);

int32_t ahf_remove (AHF_S *ahf, struct aiocb *aio);

/**
 * @brief Get an aio control block from the queue, but do not remove from the queue.
 * @param[in] ahf  - aio queue structure pointer.
 * @return    aio control block, or NULL if queue is empty.
 **/
struct aiocb *ahf_peekhead (AHF_S * const ahf);

/**
 * @brief Empty an aio control block queue.
 * @param[in] ahf  - aio queue structure pointer.
 **/
void ahf_cancelall (AHF_S * const ahf);

/**
 * @brief Flag that the operation is complete.
 If required by the sigev_notify variable contents, the calling task is notified.
 If a notify was setup, it is signalled.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] aio  - pointer to queue structure.
 * @return    0 on success.   negative error code on error.
 **/
void ahf_complete (AHF_S *ahf, struct aiocb * const aio);

/**
 * @brief Remove an aio control block from the queue.
 Signal completion to the calling task and wake it up.
 If aio is NULL, cancel all.
 * @param[in] ahf  - aio queue structure pointer.
 * @param[in] aio  - pointer to queue structure.
 * @return    0 on success.   negative error code on error.
 **/
int32_t ahf_cancel (AHF_S * const ahf, struct aiocb * const aio);

#endif /* AIOIF_H */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
