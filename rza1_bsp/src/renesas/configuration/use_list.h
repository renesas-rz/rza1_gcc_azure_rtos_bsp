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
* Copyright (C) 2016 - 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* $FileName: use_list.h $
* $ModuleRevision: {Masters}\RZ_RTX_BSP\ForOSPL\RSK_GCC\205_02_JCU_PFV $
* $Rev: 6174 $
* $Date:: 2018-01-15 14:12:23 +0000#$
* Description: Use List
******************************************************************************/

#ifndef USE_LIST_H
#define USE_LIST_H


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/***********************************************************************
* Defines:
*    Whether there is #include related modules.
*
* Description:
*    Code of refering these macros can be written at #if before
*    #include "other_module.h" or at config header file of each modules.
*
*    Almost macro can be defined in config header file of each modules.
*    Only limited macro can be define in this file. It switches whether
*    there is header file of related modules. Self module's macro cannot
*    be defined in this file, even if a module have related module or
*    does not have it.
*
*    Macro name must be start from USE_LIST_.
*    Setting must be Define or Not Define in this file.
************************************************************************/

/* #define  USE_LIST_RZA1H_OS_LESS */ /* Define: USE_LIST_RZA1H_OS_LESS */
   #define  USE_LIST_RZA1H_BSP        /* Define: USE_LIST_RZA1H_BSP */
   #define  USE_LIST_FREE_RTOS        /* Define: USE_LIST_FREE_RTOS */
   #define  USE_LIST_CMSIS            /* Define: USE_LIST_CMSIS */
/* #define  USE_LIST_RTX */           /* Define: USE_LIST_RTX */
/* #define  USE_LIST_OPT_LIB          /* Define: USE_LIST_OPT_LIB */

#if 0
/* #define  USE_LIST_RZA1H_OS_LESS */ /* Define: USE_LIST_RZA1H_OS_LESS */
  #define  USE_LIST_RZA1H_BSP         /* Define: USE_LIST_RZA1H_BSP */
/*  #define  USE_LIST_FREE_RTOS */      /* Define: USE_LIST_FREE_RTOS */
  #define  USE_LIST_CMSIS             /* Define: USE_LIST_CMSIS */
 #define  USE_LIST_RTX            /* Define: USE_LIST_RTX */
/* #define  USE_LIST_OPT_LIB */       /* Define: USE_LIST_OPT_LIB */
#endif

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* USE_LIST_H */

