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
* Copyright (C) 2012 - 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File: jcu_para.c
*    JPEG Codec Unit (JCU) driver. checking parameter.
*
* - $Version: $ (=JCU_VERSION)
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\04_RZA1 $
* - $Rev: $
* - $Date: 2018-03-06T12:53:34+09:00 $
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <string.h>
#include  "r_typedefs.h"
#include  "jcu_iodefine.h"
#include  "r_jcu.h"
#include  "../src/r_jcu_local.h"
#include  "rz_co.h"

#ifndef  JCU_RELEASE

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define MASK_LOW_3BIT   (0x7u)
#define MOD_8           (0x8u)
#define MOD_16          (0x10u)

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/***********************************************************************
* Function: JCU_ParaCheckSelectCodec
*    SelectCodec api's parameter checking
*
* Arguments:
*    [in] codec  codec type
*
* Return Value:
*    jcu_errorcode_t
************************************************************************/
jcu_errorcode_t JCU_ParaCheckSelectCodec(
    const jcu_codec_t   codec)
{
    jcu_errorcode_t  returnValue;

    if ((codec != JCU_ENCODE)
            && (codec != JCU_DECODE))
    {
        returnValue = JCU_ERROR_PARAM_F;  /* Error: */
    }
    else
    {
        returnValue = JCU_ERROR_OK;
    } /* end if */

    R_CO_SetErrNum( returnValue );
    return (returnValue);
}

/***********************************************************************
* Function: JCU_ParaCheckStart
*    Start api's parameter checking
*
* Arguments:
*    [in] codec  codec type
*
* Return Value:
*    jcu_errorcode_t
************************************************************************/
jcu_errorcode_t JCU_ParaCheckStart(
    void)
{
    jcu_errorcode_t   returnValue;

    /* If jcu already start, it cannot update this bit (this is limitation of hardware) */
    if ( (uint32_t) R_CO_GET_BIT_IN_REGISTER8( JCU,JCCMD,JSRT ) == 1u )
    {
        returnValue = JCU_ERROR_STATUS_14;  /* Error: */
    }
    else
    {
        returnValue = JCU_ERROR_OK;
    } /* end if */

    R_CO_SetErrNum( returnValue );
    return (returnValue);
}

/***********************************************************************
* Function: JCU_ParaCheckSetCountMode
*    parameter check for the SetCountMode
*
* Arguments:
*    [in] buffer the parameter for the Count Mode
*
* Return Value:
*    jcu_errorcode_t
************************************************************************/
jcu_errorcode_t JCU_ParaCheckSetCountMode(
    const jcu_count_mode_param_t  *const buffer)
{
    jcu_errorcode_t  returnValue = JCU_ERROR_OK;

    if (buffer == NULL)
    {
        returnValue = JCU_ERROR_PARAM_10;  /* Error: */
        goto fin;
    } /* end if */

    if ((buffer->inputBuffer.isEnable != false) && (buffer->outputBuffer.isEnable != false))
    {
        returnValue = JCU_ERROR_PARAM_11;  /* Error: */
        goto fin;
    }

    if (buffer->inputBuffer.isEnable != false)
    {
        /* When initAddress is true, restartAddress has to set the address */
        if (buffer->inputBuffer.isInitAddress != false)
        {
            if (buffer->inputBuffer.restartAddress == NULL)
            {
                returnValue = JCU_ERROR_PARAM_12;  /* Error: */
                goto fin;
            } /* end if */
        } /* end if */

        /* Datasize(JDATAS/LINES bit) have to 8byte alignment */
        if ((buffer->inputBuffer.dataCount & MASK_LOW_3BIT) != 0u)
        {
            returnValue = JCU_ERROR_PARAM_13;  /* Error: */
            goto fin;
        } /* end if */

        /* Datasize have to bigger than 0 */
        if (buffer->inputBuffer.dataCount == 0u)
        {
            returnValue = JCU_ERROR_PARAM_14;  /* Error: */
            goto fin;
        } /* end if */

    } /* end if */

    if (buffer->outputBuffer.isEnable != false)
    {
        /* When initAddress is true, restartAddress has to set the address */
        if (buffer->outputBuffer.isInitAddress != false)
        {
            if (buffer->outputBuffer.restartAddress == NULL)
            {
                returnValue = JCU_ERROR_PARAM_15;  /* Error: */
                goto fin;
            } /* end if */
        } /* end if */

        /* Datasize(JDATAS/LINES bit) have to 8byte alignment */
        if ((buffer->outputBuffer.dataCount & MASK_LOW_3BIT) != 0u)
        {
            returnValue = JCU_ERROR_PARAM_16;  /* Error: */
            goto fin;
        } /* end if */

        /* Datasize have to bigger than 0 */
        if (buffer->outputBuffer.dataCount == 0u)
        {
            returnValue = JCU_ERROR_PARAM_17;  /* Error: */
            goto fin;
        } /* end if */

    } /* end if */
fin:
    R_CO_SetErrNum( returnValue );
    return (returnValue);
}

/***********************************************************************
* Function: JCU_ParaCheckSetDecodeParam
*    SetDecodeParam api's parameter checking
*
* Arguments:
*    [in] buffer        - input and output buffer settings
*    [in] interruptKind - tye type of interrupt that use in this system
*
* Return Value:
*    jcu_errorcode_t
************************************************************************/
jcu_errorcode_t JCU_ParaCheckSetDecodeParam(
    const jcu_decode_param_t   *const decode,
    const jcu_buffer_param_t   *const buffer)
{
    jcu_errorcode_t  returnValue = JCU_ERROR_OK;

    if ((decode == NULL) || (buffer == NULL))
    {
        returnValue = JCU_ERROR_PARAM_19;  /* Error: */
        goto fin;
    } /* end if */

    /* NULL check for the address data */
    if ((buffer->source.address == NULL)
            || (buffer->destination.address == NULL))
    {
        returnValue = JCU_ERROR_PARAM_1A;  /* Error: */
        goto fin;
    }  /* end if */

    /* Check for the address alignment */
    /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */
    if ( (uint32_t) buffer->source.address % MOD_8 != 0u )
        /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */
    {
        returnValue = JCU_ERROR_PARAM_35;  /* Error: */
        goto fin;
    }  /* end if */

    /* Check for the address alignment */
    /* ->MISRA 11.3 */ /* ->SEC R2.7.1 */
    if ( (uint32_t) buffer->destination.address % MOD_8 != 0u )
        /* <-MISRA 11.3 */ /* <-SEC R2.7.1 */
    {
        returnValue = JCU_ERROR_PARAM_1B;  /* Error: */
        goto fin;
    }  /* end if */

    /*Check for Cb/Cr offset */
    if(decode->decodeFormat == JCU_OUTPUT_YCbCr422)
    {
        if (((decode->outputCbCrOffset) != JCU_CBCR_OFFSET_0) && ((decode->outputCbCrOffset) != JCU_CBCR_OFFSET_128))
        {
            returnValue = JCU_ERROR_PARAM_1C;  /* Error: */
        }  /* end if */
    }
    else
    {
        if ((decode->outputCbCrOffset) != JCU_CBCR_OFFSET_0)
        {
            returnValue = JCU_ERROR_PARAM_34;  /* Error: */
        }  /* end if */
    } /* end if */
fin:
    R_CO_SetErrNum( returnValue );
    return (returnValue);
}

/***********************************************************************
* Function: JCU_ParaCheckGetImageInfo
*    GetImageInfo api's parameter checking
*
* Arguments:
*    [in] buffer         buffer address that set the image information
*
* Return Value:
*    jcu_errorcode_t
************************************************************************/
jcu_errorcode_t JCU_ParaCheckGetImageInfo(
    const jcu_image_info_t   *const buffer)
{
    jcu_errorcode_t  returnValue = JCU_ERROR_OK;

    if (buffer == NULL)
    {
        returnValue = JCU_ERROR_PARAM_1D;  /* Error: */
    } /* end if */

    R_CO_SetErrNum( returnValue );
    return (returnValue);
}

/***********************************************************************
* Function: JCU_ParaCheckSetQuantizationTbl
*    SetQuantizationTable api's parameter checking
*
* Arguments:
*    [in] tableNo        the table number that set the parameter
*
* Return Value:
*    jcu_errorcode_t
************************************************************************/
jcu_errorcode_t JCU_ParaCheckSetQuantizationTbl(
    const jcu_table_no_t   tableNo)
{
    jcu_errorcode_t  returnValue = JCU_ERROR_OK;

    /* ->MISRA 13.7 : For fail safe. */
    if (tableNo > JCU_TABLE_NO_3)
        /* <-MISRA 13.7 */
        /* ->MISRA 14.1 ->QAC 3201 : For fail safe. */
    {
        returnValue = JCU_ERROR_PARAM_1E;  /* Error: */
    } /* end if */
    /* <-MISRA 14.1 <-QAC 3201 */

    R_CO_SetErrNum( returnValue );
    return (returnValue);
}

/***********************************************************************
* Function: JCU_ParaCheckSetHuffmanTable
*    SetHuffmanTable api's parameter checking
*
* Arguments:
*    [in] tableNo   - the table number that set the parameter
*    [in] type      - the type which table is set(AC or DC)
*
* Return Value:
*    jcu_errorcode_t
************************************************************************/
jcu_errorcode_t JCU_ParaCheckSetHuffmanTable(
    const jcu_table_no_t   tableNo,
    const jcu_huff_t       type)
{
    jcu_errorcode_t  returnValue = JCU_ERROR_OK;

    /* ->MISRA 13.7 : For fail safe. */
    if (tableNo > JCU_TABLE_NO_1)
        /* <-MISRA 13.7 */
    {
        returnValue = JCU_ERROR_PARAM_1F;  /* Error: */
        goto fin;
    } /* end if */

    /* ->MISRA 13.7 : For fail safe. */
    if ((type != JCU_HUFFMAN_AC)
            && (type != JCU_HUFFMAN_DC))
        /* <-MISRA 13.7 */
        /* ->MISRA 14.1 ->QAC 3201 : For fail safe. */
    {
        returnValue = JCU_ERROR_PARAM_20;  /* Error: */
    } /* end if */
    /* <-MISRA 14.1 <-QAC 3201 */
fin:
    R_CO_SetErrNum( returnValue );
    return (returnValue);
}

/***********************************************************************
* Function: JCU_ParaCheckEncodeParam
*     api's parameter checking
*
* Arguments:
*     encode - [in] <jcu_encode_param_t>.
*     buffer - [in] <jcu_buffer_param_t>.
*
* Return Value:
*    jcu_errorcode_t
************************************************************************/
jcu_errorcode_t JCU_ParaCheckEncodeParam(
    const jcu_encode_param_t   *const encode,
    const jcu_buffer_param_t   *const buffer)
{
    jcu_errorcode_t  returnValue = JCU_ERROR_OK;

    if ((encode == NULL) || (buffer == NULL))
    {
        returnValue = JCU_ERROR_PARAM_21;  /* Error: */
        goto fin;
    } /* end if */

    /* NULL check for the address data */
    if ((buffer->source.address == NULL)
            || (buffer->destination.address == NULL))
    {
        returnValue = JCU_ERROR_PARAM_22;  /* Error: */
        goto fin;
    }  /* end if */


    if (encode->encodeFormat == JCU_JPEG_YCbCr422)
    {
        /* Check the width of the image data */
        if ((encode->width % MOD_16) != 0u)
        {
            returnValue = JCU_ERROR_PARAM_23;  /* Error: */
            goto fin;
        } /* end if */

        /* Check the height of the image data */
        if ((encode->height % MOD_8) != 0u)
        {
            returnValue = JCU_ERROR_PARAM_24;  /* Error: */
            goto fin;
        } /* end if */
    }
    else
    {
        returnValue = JCU_ERROR_PARAM;
        goto fin;
    } /* end if */

    /*Check for Cb/Cr offset */
    if (((encode->inputCbCrOffset) != JCU_CBCR_OFFSET_0) && ((encode->inputCbCrOffset) != JCU_CBCR_OFFSET_128))
    {
        returnValue = JCU_ERROR_PARAM_25;  /* Error: */
    }  /* end if */

fin:
    R_CO_SetErrNum( returnValue );
    return (returnValue);
}

#endif
