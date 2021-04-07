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
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name     : compiler_settings.h
* Device(s)     : RZ/A1L
* Tool-Chain    : GNUARM-NONE-EABI-v16.01
* H/W Platform  : Platform Independent
* Description   : Any compiler specific settings are stored here.
*               : Variants of this file must be created for each compiler
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 18.06.2013 1.00
*               : 21.03.2014 2.00
******************************************************************************/


#include "application_cfg.h"
#include "r_compiler_abstraction_api.h"
#include "r_os_abstraction_api.h"
#include "r_devlink_wrapper.h"
#include "r_task_priority.h"

#ifndef COMPILER_SETTINGS_H
#define COMPILER_SETTINGS_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define BSS_INTERNAL_SECTION   __attribute__ ((section (".BSS_DMAC_SAMPLE_INTERNAL_RAM")))
#define VRAM_SECTION0  __attribute__ ((section (".VRAM_SECTION0")))
#define VRAM_SECTION1  __attribute__ ((section (".VRAM_SECTION1")))
#define VRAM_SECTION2  __attribute__ ((section (".VRAM_SECTION2")))

/*Event Defines*/
#define SIGEV_NONE      (1) /**< No notification */
#define SIGEV_THREAD    (2) /**< A notification function is called in the driver's context */
#define SIGEV_EVENT     (3) /**< An event flag is set */
#define SIGEV_CALLBACK  (4) /**< An inter-process callback is made */

#define NO_CACHE_SECTION 	VRAM_SECTION0
union sigval
{
    int             sival_int;          /**< Integer interpretation */
    void *          sival_ptr;          /**< Pointer interpretation */
};

struct sigevent
{
    int             sigev_notify;       /**< SIGEV_NONE, SIGEV_THREAD, SIGEV_EVENT */
    //int             sigev_signo;        /**< signal or callback fp */
    event_t			sigev_signo;
    union sigval    sigev_value;        /**< user defined value to pass */
    void            (*sigev_notify_function) (union sigval);
                                        /**< notification function */
};

struct aiocb
{
    /* Implementation specific stuff */
    struct aiocb *  pNext;              /**< next aio request */
    struct aiocb *  pPrev;              /**< previous aio request */
    ssize_t         aio_return;         /**< return value of operation */
    int             aio_complete;       /**< non-zero when op is completed */

    /* User parameters */
    int             aio_fildes;         /**< FD for asynchronous op */
    volatile void*  aio_buf;            /**< Buffer to read from/write to */
    size_t          aio_nbytes;         /**< Number of bytes to read/write */
    struct sigevent aio_sigevent;       /**< Signal to deliver */

    /* Private implementation specific function pointer used by IOIF. Application should not set. */
    ssize_t (*aio_iofn)(void *,void *,size_t,int);  /* synchronous I/O function */
};
typedef struct aiocb AIOCB;




/******************************************************************************
Variable External definitions and Function External definitions
******************************************************************************/
extern void __enable_irq(void);
extern uint32_t __disable_irq(void);

extern void __enable_fiq(void);
extern void __disable_fiq(void);

/******************************************************************************
Functions Prototypes
******************************************************************************/

/* COMPILER_SETTINGS_H */
#endif  

/* End of File */
