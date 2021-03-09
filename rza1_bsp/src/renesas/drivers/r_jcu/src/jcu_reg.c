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
* Copyright (C) 2012 - 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File: jcu_reg.c
*    JPEG Codec Unit (JCU) driver. Setting registers.
*
* - $Version: $ (=JCU_VERSION)
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\04_RZA1 $
* - $Rev: $
* - $Date: 2018-03-06T10:27:16+09:00 $
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "r_typedefs.h"
#include  "jcu_iodefine.h"
#include  "jcu_iobitmask.h"
#include  "cpg_iodefine.h"
#include  "cpg_iobitmask.h"
#include  "../src/r_jcu_local.h"
#include  "rz_co.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define NUMBER_OF_QUANTIZATION_TABLE_DATA   (64)
#define NUMBER_OF_HUFFMAN_TABLE_DATA_DC     (28)
#define NUMBER_OF_HUFFMAN_TABLE_DATA_AC     (178)

#define CBCR_OFFSET_NUM     (2u)

#define SWAP_NORMAL         (0u)
#define SWAP_16BIT_OUTPUT   (1u)
#define SWAP_32BIT_OUTPUT   (2u)

#define KIND_COLOR_FORMAT   (3u)
#define KIND_LINEOFFSET     (2u)
#define KIND_TABLE          (4u)

#define  USE_STANDBY_REQUEST  (1)


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
STATIC_INLINE uint8_t  GET_UPPER_BYTE( uint32_t const  value );
STATIC_INLINE uint8_t  GET_LOWER_BYTE( uint32_t const  value );


/***********************************************************************
* Function: GET_UPPER_BYTE
*    GET_UPPER_BYTE
*
* Return Value:
*    Byte
************************************************************************/
STATIC_INLINE uint8_t  GET_UPPER_BYTE( uint32_t const  value )
{
    enum { num_8 = 8 }; /* SEC M1.10.1, QAC-3132 */
    return  (uint8_t)( value >> num_8 );
}

/***********************************************************************
* Function: GET_LOWER_BYTE
*    GET_LOWER_BYTE
*
* Return Value:
*    Byte
************************************************************************/
STATIC_INLINE uint8_t  GET_LOWER_BYTE( uint32_t const  value )
{
    return  (uint8_t)( value );
}

/***********************************************************************
* Function: JCU_SetRegisterForInitialize
*    SetRegisterForInitialize
*
* Return Value:
*    jcu_errorcode_t
************************************************************************/
jcu_errorcode_t JCU_SetRegisterForInitialize(
    void)
{
    return JCU_ERROR_OK;
}

/***********************************************************************
* Function: JCU_SetRegisterForCodec
*    SetRegisterForCodec
*
* Arguments:
*    codec
*
* Return Value:
*    None
************************************************************************/
void JCU_SetRegisterForCodec( const jcu_codec_t  codec )
{
    volatile uint8_t      dummy_read;
    int32_t               watch_dog_count = 0;
    static const int32_t  watch_dog_max_count = 0x10000000;


#if  USE_STANDBY_REQUEST
    /* Turn on standby request. */
    R_CO_SET_BIT_IN_REGISTER8( CPG,STBREQ1,STBRQ13,  1 );
    dummy_read = R_CO_GET_BIT_IN_REGISTER8( CPG,STBREQ1,STBRQ13 );
    dummy_read = 0;
    while ( dummy_read == 0 )
    {
        dummy_read = R_CO_GET_BIT_IN_REGISTER8( CPG,STBACK1,STBAK13 );

        watch_dog_count += 1;
        if ( watch_dog_count >= watch_dog_max_count )
        {
            R_CO_SetErrNum( E_UNRECOVERABLE ); /* Error: */
        }
    }
#endif

    /* Reset this IP */
    R_CO_SET_BIT_IN_REGISTER8( CPG,SWRSTCR2,SRST21,  1 );
    dummy_read = R_CO_GET_BIT_IN_REGISTER8( CPG,SWRSTCR2,SRST21 );
    R_UNREFERENCED_VARIABLE( dummy_read );

    R_CO_SET_BIT_IN_REGISTER8( CPG,SWRSTCR2,SRST21,  0 );
    dummy_read = R_CO_GET_BIT_IN_REGISTER8( CPG,SWRSTCR2,SRST21 );
    R_UNREFERENCED_VARIABLE( dummy_read );

#if  USE_STANDBY_REQUEST
    /* Turn off standby request. */
    R_CO_SET_BIT_IN_REGISTER8( CPG,STBREQ1,STBRQ13,  0 );
    dummy_read = R_CO_GET_BIT_IN_REGISTER8( CPG,STBREQ1,STBRQ13 );
    dummy_read = R_CO_GET_BIT_IN_REGISTER8( CPG,STBACK1,STBAK13 );
#endif

    /* Issue bus reset of JCU. */
    R_CO_SET_BIT_IN_REGISTER8( JCU,JCCMD,BRST,  1 );

    /* Enable interrupts in registers (Interrupt lines in INTC are still disabled) */
    /* This registers are set to 0 by reset this IP */
    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JINTE1 ) = (uint32_t) JCU_JINTS1_ALL;
    /* <-QAC 0306 */

    /* Clear the all of count mode settings */
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT,DINLC,  0 );
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT,DINRINI,  0 );
    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JIFESLC ) = 0u;
    /* <-QAC 0306 */

    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT,JOUTC,  0 );
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT,JOUTRINI,  0 );
    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JIFEDDC ) = 0u;
    /* <-QAC 0306 */

    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT,JINC,  0 );
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT,JINRINI,  0 );
    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JIFDSDC ) = 0u;
    /* <-QAC 0306 */

    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT,DOUTLC,  0 );
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT,DOUTRINI,  0 );
    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JIFDDLC ) = 0u;
    /* <-QAC 0306 */

    /* Set the which process is select */
    R_CO_SET_BIT_IN_REGISTER8( JCU,JCMOD,DSP,  codec );
    return;
}

/***********************************************************************
* Function: JCU_ClearInterruptFlag
*    ClearInterruptFlag
*
* Return Value:
*    None
************************************************************************/
void JCU_ClearInterruptFlag(
    void)
{
    /* Interrupt request clear */
    R_CO_SET_BIT_IN_REGISTER8( JCU,JCCMD,JEND,  1 );

    /* Clear the JEDI interrupt source flag */
    /* ->QAC 0306 */
    R_CO_REGISTER8( JCU.JINTS0 ) = (uint8_t) 0u;  /* Write 0 only */
    /* <-QAC 0306 */

    /* Clear the JDTI interrupt source flag */
    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JINTS1 ) &= ~JCU_JINTS1_ALL;
    /* <-QAC 0306 */
    return;
}

/***********************************************************************
* Function: JCU_SetRegisterForStart
*    SetRegisterForStart
*
* Return Value:
*    None
************************************************************************/
void JCU_SetRegisterForStart(
    const jcu_int_detail_errors_t error_filter)
{
    uint32_t reg_jinte0;
    reg_jinte0 = (uint32_t)R_CO_REGISTER8( JCU.JINTE0 ) & (uint32_t)(~JCU_INT_ERROR_ALL);
    R_CO_REGISTER8( JCU.JINTE0 ) = (uint8_t)( reg_jinte0 | (uint32_t)error_filter );
    R_CO_SET_BIT_IN_REGISTER8( JCU,JCCMD,JSRT,  1 );
    return;
}

/***********************************************************************
* Function: JCU_SetRegisterForSetCountMode
*    SetRegisterForSetCountMode
*
* Arguments:
*     buffer - [in]
*
* Return Value:
*    None
************************************************************************/
void JCU_SetRegisterForSetCountMode(
    const jcu_count_mode_param_t    *const buffer)
{
    jcu_internal_information_t* const  self = R_JCU_GetInternalInformation();
    errnum_t  e;

#if ! defined( JCU_RELEASE )
    if ( buffer == NULL )
    {
        e=JCU_ERROR_PARAM_28;    /* Error: */
        goto fin;
    }
    if ( self   == NULL )
    {
        e=JCU_ERROR_PARAM_29;    /* Error: */
        goto fin;
    }
#endif

    switch(self->Codec)
    {
    case JCU_STATUS_ENCODE:
        /* ->QAC 0306 */
        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT, DINRINI,  buffer->inputBuffer.isInitAddress );
        if ((buffer->inputBuffer.isEnable != false) && (buffer->inputBuffer.isInitAddress != false))
        {
            R_CO_REGISTER32( JCU.JIFESA ) = (uint32_t)(buffer->inputBuffer.restartAddress);
        } /* end if */
        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFESLC, LINES,  buffer->inputBuffer.dataCount );

        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT, JOUTRINI,  buffer->outputBuffer.isInitAddress );
        if ((buffer->outputBuffer.isEnable != false) && (buffer->outputBuffer.isInitAddress != false))
        {
            R_CO_REGISTER32( JCU.JIFEDA ) = (uint32_t)(buffer->outputBuffer.restartAddress);
        } /* end if */
        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFEDDC, JDATAS,  buffer->outputBuffer.dataCount );
        /* <-QAC 0306 */

        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT, DINLC,  buffer->inputBuffer.isEnable );
        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT, JOUTC,  buffer->outputBuffer.isEnable );
        break;

    case JCU_STATUS_DECODE:
        /* ->QAC 0306 */
        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT,JINRINI,  buffer->inputBuffer.isInitAddress );
        if ((buffer->inputBuffer.isEnable != false) && (buffer->inputBuffer.isInitAddress != false))
        {
            R_CO_REGISTER32( JCU.JIFDSA ) = (uint32_t)(buffer->inputBuffer.restartAddress);
        } /* end if */
        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDSDC,JDATAS,  buffer->inputBuffer.dataCount );

        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT,DOUTRINI,  buffer->outputBuffer.isInitAddress );
        if ((buffer->outputBuffer.isEnable != false) && (buffer->outputBuffer.isInitAddress != false))
        {
            R_CO_REGISTER32( JCU.JIFDDA ) = (uint32_t)(buffer->outputBuffer.restartAddress);
        } /* end if */
        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDDLC,LINES,  buffer->outputBuffer.dataCount );
        /* <-QAC 0306 */

        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT,JINC,  buffer->inputBuffer.isEnable );
        R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT,DOUTLC,  buffer->outputBuffer.isEnable );
        break;

    case JCU_CODEC_NOT_SELECTED:
        /* ->QAC 2018 : For MISRA 15.3, SEC R3.5.2 */
    default:
        /* <-QAC 2018 */
        break;  /* NOT REACHED */
    } /* end switch */

    e=0;
fin:
    R_CO_SetErrNum( e );
    return;
}

/***********************************************************************
* Function: JCU_SetRegisterForContinue
*    SetRegisterForContinue
*
* Arguments:
*     type - [in]
*
* Return Value:
*    None
************************************************************************/
void JCU_SetRegisterForContinue(
    const jcu_continue_type_t  type)
{
    errnum_t  e;
    jcu_internal_information_t* const  self = R_JCU_GetInternalInformation();

#if ! defined( JCU_RELEASE )
    if ( self == NULL )
    {
        e=JCU_ERROR_PARAM_2A;    /* Error: */
        goto fin;
    }
#endif

    /* Restart register setting */
    switch (type)
    {
    case JCU_INPUT_BUFFER:
        if (self->Codec == JCU_STATUS_DECODE)
        {
            R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT, JINRCMD,  1 );
        }
        else
        {
            R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT, DINRCMD,  1 );
        } /* end if */
        break;

    case JCU_OUTPUT_BUFFER:
        if (self->Codec == JCU_STATUS_DECODE)
        {
            R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT, DOUTRCMD,  1 );
        }
        else
        {
            R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT, JOUTRCMD,  1 );
        } /* end if */
        break;

    case JCU_IMAGE_INFO:
        R_CO_SET_BIT_IN_REGISTER8( JCU,JCCMD, JRST,  1 );
        break;

        /* ->QAC 2018 : For MISRA 15.3, SEC R3.5.2 */
    default:
        /* <-QAC 2018 */
        /* NOT REACHED */
        break;
    } /* end switch */

    e=0;
fin:
    R_CO_SetErrNum( e );
    return;
}

/***********************************************************************
* Function: JCU_SetRegisterForSetDecodePrm
*    SetRegisterForSetDecod
*
* Arguments:
*     decode - [in]
*     buffer - [in]
*     interruptKind - [in]
*
* Return Value:
*    None
************************************************************************/
void JCU_SetRegisterForSetDecodePrm(
    const jcu_decode_param_t   *const decode,
    const jcu_buffer_param_t   *const buffer)
{
    const uint32_t byteSize[KIND_COLOR_FORMAT] =
        /* ->SEC M1.10.1 Table data. */
    {
        2u, /* JCU_OUTPUT_YCbCr422 */
        4u, /* JCU_OUTPUT_ARGB8888 */
        2u  /* JCU_OUTPUT_RGB565   */
    };
    /* <-SEC M1.10.1 */
    errnum_t  e;

#if ! defined( JCU_RELEASE )
    if ( decode == NULL )
    {
        e=JCU_ERROR_PARAM_2B;    /* Error: */
        goto fin;
    }
    if ( buffer == NULL )
    {
        e=JCU_ERROR_PARAM_2C;    /* Error: */
        goto fin;
    }
#endif

    /* Settings for the cull */
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT, VINTER,  decode->verticalSubSampling );
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT, HINTER,  decode->horizontalSubSampling );
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT, OPF,     decode->decodeFormat );
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT, JINSWAP,   buffer->source.swapSetting );
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDCNT, DOUTSWAP,  buffer->destination.swapSetting );

    /* Settings for input jpeg file information */
    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JIFDSA ) = (uint32_t)(buffer->source.address);
    /* <-QAC 0306 */
    /* Settings for output image data information */
    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JIFDDA ) = (uint32_t)(buffer->destination.address);
    /* <-QAC 0306 */
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDDOFST, DDMW,
                                ((uint32_t)buffer->lineOffset * (uint32_t)(byteSize[decode->decodeFormat])) );
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFDADT, ALPHA,
                                (uint32_t)(decode->alpha) );


    /* Setting for Cb/Cr offset */
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFESHSZ, DOUTYCHG,  decode->outputCbCrOffset );

    e=0;
fin:
    R_CO_SetErrNum( e );
    return;

}

/***********************************************************************
* Function: JCU_SetRegisterForSetQtTable
*    SetRegisterForSetQtTable
*
* Arguments:
*     tableNo - [in]
*     table   - [in]
*
* Return Value:
*    None
************************************************************************/
void JCU_SetRegisterForSetQtTable(
    const jcu_table_no_t   tableNo,
    const uint8_t* const   table)
{
    volatile uint8_t*  tableAtRegister;
    int_fast32_t       counter;
    errnum_t           e;

    /* Select the destination of the Quantization table */
    switch (tableNo)
    {
        /* ->QAC 0306 */
    case JCU_TABLE_NO_0:
        tableAtRegister = (volatile uint8_t *)&JCU.JCQTBL0;
        break;
    case JCU_TABLE_NO_1:
        tableAtRegister = (volatile uint8_t *)&JCU.JCQTBL1;
        break;
    case JCU_TABLE_NO_2:
        tableAtRegister = (volatile uint8_t *)&JCU.JCQTBL2;
        break;
    case JCU_TABLE_NO_3:
        tableAtRegister = (volatile uint8_t *)&JCU.JCQTBL3;
        break;
        /* ->QAC 2018 : For MISRA 15.3, SEC R3.5.2 */
    default:
        /* <-QAC 2018 */
        /* NOT REACHED */
        return;
        /* <-QAC 0306 */
    } /* end switch */

#if ! defined( JCU_RELEASE )
    if ( tableAtRegister == NULL )
    {
        e=JCU_ERROR_PARAM_2D;    /* Error: */
        goto fin;
    }
    if ( table == NULL )
    {
        e=JCU_ERROR_PARAM_2E;    /* Error: */
        goto fin;
    }
#endif

    /* Update the quantization table data */
    for (counter = 0; counter < NUMBER_OF_QUANTIZATION_TABLE_DATA; counter++)
    {
        tableAtRegister[ counter ] = table[ counter ];
    } /* end for */

    e=0;
fin:
    R_CO_SetErrNum( e );
    return;
}

/***********************************************************************
* Function: JCU_SetRegisterForSetHuffTbl
*    SetRegisterForSetHuffTbl
*
* Arguments:
*     tableNo - [in]
*     type    - [in]
*     table   - [in]
*
* Return Value:
*    None
************************************************************************/
void JCU_SetRegisterForSetHuffTbl(
    const jcu_table_no_t   tableNo,
    const jcu_huff_t       type,
    const uint8_t *const   table)
{
    volatile uint8_t*  tableAtRegister;
    int_fast32_t       copyCount;
    int_fast32_t       counter;
    errnum_t           e;

    /* Select the destination Huffman table and the size of copy data */
    if (tableNo == JCU_TABLE_NO_0)
    {
        /* ->QAC 0306 */
        if (type == JCU_HUFFMAN_DC)
        {
            tableAtRegister  = (volatile uint8_t *)&JCU.JCHTBD0;
            copyCount        = NUMBER_OF_HUFFMAN_TABLE_DATA_DC;
        }
        else
        {
            tableAtRegister  = (volatile uint8_t *)&JCU.JCHTBA0;
            copyCount        = NUMBER_OF_HUFFMAN_TABLE_DATA_AC;
        } /* end if */
        /* <-QAC 0306 */
    }
    else
    {
        /* ->QAC 0306 */
        if (type == JCU_HUFFMAN_DC)
        {
            tableAtRegister  = (volatile uint8_t *)&JCU.JCHTBD1;
            copyCount        = NUMBER_OF_HUFFMAN_TABLE_DATA_DC;
        }
        else
        {
            tableAtRegister  = (volatile uint8_t *)&JCU.JCHTBA1;
            copyCount        = NUMBER_OF_HUFFMAN_TABLE_DATA_AC;
        } /* end if */
        /* <-QAC 0306 */
    }  /* end if */

#if ! defined( JCU_RELEASE )
    if ( tableAtRegister == NULL )
    {
        e=JCU_ERROR_PARAM_2F;    /* Error: */
        goto fin;
    }
    if ( table == NULL )
    {
        e=JCU_ERROR_PARAM_30;    /* Error: */
        goto fin;
    }
#endif

    /* Update the Huffman table */
    for (counter = 0; counter < copyCount; counter++)
    {
        tableAtRegister[ counter ] = table[ counter ];
    } /* end for */

    e=0;
fin:
    R_CO_SetErrNum( e );
    return;
}

/***********************************************************************
* Function: JCU_SetRegisterForSetEncodePrm
*    SetRegisterForSetEncode
*
* Arguments:
*     encode - [in]
*     buffer - [in]
*
* Return Value:
*    None
************************************************************************/
void JCU_SetRegisterForSetEncodePrm(
    const jcu_encode_param_t   *const encode,
    const jcu_buffer_param_t   *const buffer)
{
    errnum_t  e;
    jcu_jpeg_format_t encodeFormatTemp;
    const uint32_t byteSize[KIND_LINEOFFSET] =
        /* ->SEC M1.10.1 Table data. */
    {
        0u, /* Reserved          */
        2u  /* JCU_JPEG_YCbCr422 */
    };
    /* <-SEC M1.10.1 */

#if ! defined( JCU_RELEASE )
    if ( encode == NULL )
    {
        e=JCU_ERROR_PARAM_31;    /* Error: */
        goto fin;
    }
    if ( buffer == NULL )
    {
        e=JCU_ERROR_PARAM_32;    /* Error: */
        goto fin;
    }
#endif

    /* Settings for the input image format */
    R_CO_SET_BIT_IN_REGISTER8( JCU,JCMOD, REDU,  encode->encodeFormat );

    /* Settings for the quantisation table */
    R_CO_SET_BIT_IN_REGISTER8(JCU,JCQTN, QT1, (uint8_t) encode->QuantizationTable[JCU_ELEMENT_Y]);
    R_CO_SET_BIT_IN_REGISTER8(JCU,JCQTN, QT2, (uint8_t) encode->QuantizationTable[JCU_ELEMENT_Cb]);
    R_CO_SET_BIT_IN_REGISTER8(JCU,JCQTN, QT3, (uint8_t) encode->QuantizationTable[JCU_ELEMENT_Cr]);

    /* Settings for the Huffman table */
    R_CO_SET_BIT_IN_REGISTER8(JCU,JCHTN, HTA1, (uint8_t) (encode->HuffmanTable[JCU_ELEMENT_Y]));
    R_CO_SET_BIT_IN_REGISTER8(JCU,JCHTN, HTD1, (uint8_t) (encode->HuffmanTable[JCU_ELEMENT_Y]));
    R_CO_SET_BIT_IN_REGISTER8(JCU,JCHTN, HTA2, (uint8_t) (encode->HuffmanTable[JCU_ELEMENT_Cb]));
    R_CO_SET_BIT_IN_REGISTER8(JCU,JCHTN, HTD2, (uint8_t) (encode->HuffmanTable[JCU_ELEMENT_Cb]));
    R_CO_SET_BIT_IN_REGISTER8(JCU,JCHTN, HTA3, (uint8_t) (encode->HuffmanTable[JCU_ELEMENT_Cr]));
    R_CO_SET_BIT_IN_REGISTER8(JCU,JCHTN, HTD3, (uint8_t) (encode->HuffmanTable[JCU_ELEMENT_Cr]));

    /* ->QAC 0306 */
    R_CO_REGISTER8( JCU.JCDRIU ) = GET_UPPER_BYTE( encode->DRI_value );
    R_CO_REGISTER8( JCU.JCDRID ) = GET_LOWER_BYTE( encode->DRI_value );
    /* Settings for the image size */
    /* width setting */
    R_CO_REGISTER8( JCU.JCHSZU ) = GET_UPPER_BYTE( encode->width );
    R_CO_REGISTER8( JCU.JCHSZD ) = GET_LOWER_BYTE( encode->width );
    /* height setting */
    R_CO_REGISTER8( JCU.JCVSZU ) = GET_UPPER_BYTE( encode->height );
    R_CO_REGISTER8( JCU.JCVSZD ) = GET_LOWER_BYTE( encode->height );
    /* <-QAC 0306 */


    /* Settings for input jpeg file information */
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT, DINSWAP,  buffer->source.swapSetting );

    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JIFESA ) = (uint32_t)( buffer->source.address );
    /* <-QAC 0306 */

    /* Settings for output image data information */
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFECNT, JOUTSWAP,  buffer->destination.swapSetting );

    /* ->QAC 0306 */
    R_CO_REGISTER32( JCU.JIFEDA ) = (uint32_t)( buffer->destination.address );
    /* <-QAC 0306 */

    encodeFormatTemp = encode->encodeFormat;
    if((encodeFormatTemp != JCU_JPEG_YCbCr444) && (encodeFormatTemp != JCU_JPEG_YCbCr422))
    {
        encodeFormatTemp = JCU_JPEG_YCbCr444;
    } /* end if */

    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFESOFST, ESMW,
                                ((uint32_t)buffer->lineOffset * (uint32_t)(byteSize[encodeFormatTemp])) );

    /* Setting for Cb/Cr offset */
    R_CO_SET_BIT_IN_REGISTER32( JCU,JIFESVSZ, DINYCHG,  encode->inputCbCrOffset );

    e=0;
fin:
    R_CO_SetErrNum( e );
    return;

}

/***********************************************************************
* Function: JCU_SetRegisterForSetPause
*
* Arguments:
*     is_stop - [out]
*
* Return Value:
*    None
************************************************************************/
void JCU_SetRegisterForSetPause( const bool_t is_pause, const jcu_int_detail_errors_t filter )
{
    /* ->QAC 0306 */
    if ( is_pause )
    {
        R_CO_REGISTER8( JCU.JINTE0 ) = (uint8_t)( filter | JCU_JINTE0_INT3 );
    }
    else
    {
        R_CO_REGISTER8( JCU.JINTE0 ) = (uint8_t) filter;
    }
    /* <-QAC 0306 */
}

/***********************************************************************
* Function: JCU_GetRegisterForGetImageInfo
*    Get "jcu_image_info_t" type value
*
* Arguments:
*     buffer - [out]<jcu_image_info_t>.
*
* Return Value:
*    None
************************************************************************/
void JCU_GetRegisterForGetImageInfo(
    jcu_image_info_t *const buffer)
{
    enum { num_8_bit = 8 };
    errnum_t  e;

#if ! defined( JCU_RELEASE )
    if ( buffer == NULL )
    {
        e=JCU_ERROR_PARAM_33;    /* Error: */
        goto fin;
    }
#endif

    /* Set the register value to the variables */
    /* ->QAC 0306 */
    buffer->width           = ((uint32_t) R_CO_REGISTER8( JCU.JCHSZU ) << num_8_bit);
    buffer->width           |= (uint32_t) R_CO_REGISTER8( JCU.JCHSZD );
    buffer->height          = ((uint32_t) R_CO_REGISTER8( JCU.JCVSZU ) << num_8_bit);
    buffer->height          |= (uint32_t) R_CO_REGISTER8( JCU.JCVSZD );
    buffer->encodedFormat   = (jcu_jpeg_format_t)R_CO_GET_BIT_IN_REGISTER8( JCU,JCMOD, REDU );
    /* <-QAC 0306 */

    e=0;
fin:
    R_CO_SetErrNum( e );
    return;
}

/***********************************************************************
* Function: JCU_GetRegisterForGetErrorInfo
*    Get "jcu_detail_error_t" type value
*
* Arguments:
*     errorCode - [out].
*
* Return Value:
*    None
************************************************************************/
void JCU_GetRegisterForGetErrorInfo(
    jcu_detail_error_t *const errorCode)
{
    if (errorCode != NULL)
    {
        /* ->QAC 0306 */
        uint8_t const  code = R_CO_GET_BIT_IN_REGISTER8( JCU,JCDERR, ERR );
        /* <-QAC 0306 */

        if ( (uint_fast8_t) code == 0u )
        {
            *errorCode = JCU_JCDERR_OK;
        }
        else
        {
            *errorCode = (jcu_detail_error_t)( E_JCU_JCDERR + ( (int_t) code << 8 ) );
        }
    } /* end if */
    return;
}

/***********************************************************************
* Function: JCU_GetEncodedSize
*    GetEncodedSize
*
* Arguments:
*     out_Size - [out].
*
* Return Value:
*    None
************************************************************************/
void JCU_GetEncodedSize(
    size_t *const out_Size)
{
    if (out_Size != NULL)
    {
        enum { num_16_bits = 16 };
        enum { num_8_bits = 8 };

        /* ->QAC 0306 */
        *out_Size  = (((uint32_t) R_CO_REGISTER8( JCU.JCDTCU )) << num_16_bits);
        *out_Size |= (((uint32_t) R_CO_REGISTER8( JCU.JCDTCM )) << num_8_bits);
        *out_Size |=  ((uint32_t) R_CO_REGISTER8( JCU.JCDTCD ));
        /* <-QAC 0306 */
    } /* end if */
    return;
}

/***********************************************************************
* Function: JCU_OnInterrupting
*    Operations on interrupting
*
* Return Value:
*    Error code, 0=No error
************************************************************************/
void  JCU_OnInterrupting( bit_flags_fast32_t*  in_out_InterruptFlags )
{
    enum { num_8_bit = 8 };

    uint32_t  flags0;
    uint32_t  flags1;

    /* Get interrupt status */
    /* ->QAC 0306 */
    flags0 = R_CO_REGISTER8(  JCU.JINTS0 );
    flags1 = R_CO_REGISTER32( JCU.JINTS1 );
    *in_out_InterruptFlags |= flags0;
    *in_out_InterruptFlags |= flags1 << (sizeof( JCU.JINTS0 ) * num_8_bit);

    /* Clear interrupt status "JINTS0" */
    if ( flags0 != 0u )
    {
        R_CO_REGISTER8( JCU.JINTS0 ) = (uint8_t) 0u;  /* Write 0 only */
        R_CO_SET_BIT_IN_REGISTER8( JCU,JCCMD,  JEND,  1u );
    }

    /* Clear interrupt status "JINTS1" */
    R_CO_REGISTER32( JCU.JINTS1 ) = ~flags1 & JCU_JINTS1_ALL;
    /* <-QAC 0306 */
}

