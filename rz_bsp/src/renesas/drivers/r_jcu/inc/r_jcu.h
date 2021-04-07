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
 * @headerfile     r_jcu.h
 * @brief          JPEG Codec Unit (JCU) driver's API. Main Header.
*
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\04_RZA1 $
* - $Rev: $
* - $Date: 2018-03-06T10:14:18+09:00 $
******************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef  R_SW_PKG_93_JCU_API_H_INCLUDED
#define  R_SW_PKG_93_JCU_API_H_INCLUDED

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <stddef.h>
#include  "r_typedefs.h"
#include  "r_jcu_typedef.h"
#include  "rz_co_typedef.h"

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_NONOS_DRIVERS
 * @defgroup R_SW_PKG_93_JCU_API JCU RZA1 Driver
 *
 * @anchor R_SW_PKG_93_JCU_API_SUMMARY
 * @par Summary
 *
 * JPEG Codec Unit (JCU) driver's API. Main Header..
 *
 * @anchor R_SW_PKG_93_JCU_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1 Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Struct & Enum definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
jcu_errorcode_t R_JCU_Initialize( jcu_config_t*  in_out_Config );
jcu_errorcode_t R_JCU_TerminateAsync( r_co_function_t  in_OnFinalized,  volatile void*  in_OnFinalizedArgument );
jcu_errorcode_t R_JCU_SelectCodec( const jcu_codec_t  in_Codec );
jcu_errorcode_t R_JCU_StartAsync( r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument );
jcu_errorcode_t R_JCU_ContinueAsync( const jcu_continue_type_t  in_Type,  r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument );
jcu_errorcode_t R_JCU_SetDecodeParam( const jcu_decode_param_t*const  in_Decode,  const jcu_buffer_param_t*const  in_Buffer );
jcu_errorcode_t R_JCU_SetPauseForImageInfo( const bool_t  in_IsPause );
jcu_errorcode_t R_JCU_GetImageInfo( jcu_image_info_t* const  out_Buffer );
jcu_errorcode_t R_JCU_SetErrorFilter( jcu_int_detail_errors_t  in_Filter );
jcu_errorcode_t R_JCU_SetQuantizationTable( const jcu_table_no_t  in_TableNum,  const uint8_t* const  in_Table );
jcu_errorcode_t R_JCU_SetHuffmanTable( const jcu_table_no_t  in_TableNum,  const jcu_huff_t  in_Type,  const uint8_t* const  in_Table );
jcu_errorcode_t R_JCU_SetEncodeParam( const jcu_encode_param_t* const  in_Encode,  const jcu_buffer_param_t* const  in_Buffer );
jcu_errorcode_t R_JCU_GetEncodedSize( size_t* const  out_Size );

bool_t    R_JCU_DisableInterrupt(void);
void      R_JCU_EnableInterrupt(void);
errnum_t  R_JCU_OnInterrupting(void);
errnum_t  R_JCU_OnInterrupted(void);
void      R_JCU_PrintRegisters(void);

#endif /* R_SW_PKG_93_JCU_API_H_INCLUDED */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
