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
* Copyright (C) 2012 - 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File: jcu_local.h
*    JPEG Codec Unit (JCU) driver. Internal functions.
*
* - $Version: $ (=JCU_VERSION)
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\04_RZA1 $
* - $Rev: $
* - $Date: 2018-03-06T10:26:18+09:00 $
******************************************************************************/

#ifndef JCU_LOCAL_H
#define JCU_LOCAL_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "jcu_iodefine.h"
#include  "jcu_iobitmask.h"
#include  "r_jcu.h"
#include  "rz_co_typedef.h"


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/***********************************************************************
* Structure: jcu_internal_information_t
*    Internal Information for this driver.
************************************************************************/
typedef struct
{
    jcu_codec_status_t       Codec;
    bool_t                   IsCountMode;
    jcu_int_detail_errors_t  ErrorFilter;  /* 1=Enable */

    jcu_async_status_t       AsyncStatus;
} jcu_internal_information_t;


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/
jcu_internal_information_t*  R_JCU_GetInternalInformation(void);
jcu_errorcode_t  R_JCU_FinalizeStep2_Sub(void);

#ifndef  JCU_RELEASE
jcu_errorcode_t JCU_ParaCheckStart(void);
jcu_errorcode_t JCU_ParaCheckSetCountMode(const jcu_count_mode_param_t  *const buffer);
jcu_errorcode_t JCU_ParaCheckSelectCodec(const jcu_codec_t codec);
jcu_errorcode_t JCU_ParaCheckSetCallbackFunc(const uintptr_t type);
jcu_errorcode_t JCU_ParaCheckSetDecodeParam(const jcu_decode_param_t *const decode, const jcu_buffer_param_t *const buffer);
jcu_errorcode_t JCU_ParaCheckGetImageInfo(const jcu_image_info_t *const buffer);
jcu_errorcode_t JCU_ParaCheckGetErrorInfo(const jcu_detail_error_t *const errorCode);
jcu_errorcode_t JCU_ParaCheckSetQuantizationTbl(const jcu_table_no_t tableNo);
jcu_errorcode_t JCU_ParaCheckSetHuffmanTable(const jcu_table_no_t tableNo, const jcu_huff_t type);
jcu_errorcode_t JCU_ParaCheckEncodeParam(const jcu_encode_param_t *const encode, const jcu_buffer_param_t *const buffer);
jcu_errorcode_t JCU_CheckInterruptSource(const uint32_t interruptKind);
#endif

jcu_errorcode_t JCU_SetRegisterForInitialize(void);
void JCU_SetRegisterForCodec(const jcu_codec_t codec);
void JCU_SetRegisterForStart(const jcu_int_detail_errors_t error_filter);
void JCU_SetRegisterForReset(void);
void JCU_SetRegisterForSetCountMode(const jcu_count_mode_param_t *const buffer);
void JCU_SetRegisterForContinue(const jcu_continue_type_t type);
void JCU_SetRegisterForSetDecodePrm(const jcu_decode_param_t *const decode, const jcu_buffer_param_t *const buffer);
void JCU_SetRegisterForSetQtTable(const jcu_table_no_t tableNo, const uint8_t* const  table);
void JCU_SetRegisterForSetHuffTbl(const jcu_table_no_t tableNo, const jcu_huff_t type, const uint8_t *const table);
void JCU_SetRegisterForSetEncodePrm(const jcu_encode_param_t *const encode, const jcu_buffer_param_t *const buffer);
void JCU_SetRegisterForSetPause(const bool_t is_pause, const jcu_int_detail_errors_t filter);
void JCU_GetRegisterForGetImageInfo(jcu_image_info_t *const buffer);
void JCU_GetRegisterForGetErrorInfo(jcu_detail_error_t *const errorCode);
void JCU_GetEncodedSize(size_t *const out_Size);
void JCU_OnInterrupting( bit_flags_fast32_t*  in_out_InterruptFlags );
void JCU_ClearInterruptFlag(void);

#endif /* JCU_LOCAL_H */
