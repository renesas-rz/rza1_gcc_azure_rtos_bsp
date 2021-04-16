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
* File: jcu_api.c
*    JPEG Codec Unit (JCU) driver API. Main Code for single thread.
*
* - $Version: $ (=JCU_VERSION)
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\04_RZA1 $
* - $Rev: $
* - $Date: 2018-03-06T10:27:16+09:00 $
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  <string.h>
#include  "r_typedefs.h"
#include  "jcu_iodefine.h"
#include  "jcu_iobitmask.h"
#include  "r_jcu.h"
#include  "../src/r_jcu_local.h"
#include  "../src/r_jcu_pl.h"
#include  "rz_co.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/***********************************************************************
* Variable: gs_jcu_internal_information
************************************************************************/
static jcu_internal_information_t  gs_jcu_internal_information;


/******************************************************************************
Private global variables and functions
******************************************************************************/
static jcu_errorcode_t  R_JCU_Initialize_ST( jcu_config_t*  in_out_Config );
static jcu_errorcode_t  R_JCU_TerminateAsync_ST( r_co_function_t  in_OnFinalized,  volatile void*  in_OnFinalizedArgument );
static jcu_errorcode_t  R_JCU_SelectCodec_ST( const jcu_codec_t  in_Codec );
static jcu_errorcode_t  R_JCU_StartAsync_ST( r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument );
static jcu_errorcode_t  R_JCU_ContinueAsync_ST( const jcu_continue_type_t  in_Type,  r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument );
static jcu_errorcode_t  R_JCU_SetDecodeParam_ST( const jcu_decode_param_t*const  in_Decode,  const jcu_buffer_param_t*const  in_Buffer );
static jcu_errorcode_t  R_JCU_SetPauseForImageInfo_ST( const bool_t  in_IsPause );
static jcu_errorcode_t  R_JCU_GetImageInfo_ST( jcu_image_info_t* const  out_Buffer );
static jcu_errorcode_t  R_JCU_SetErrorFilter_ST( jcu_int_detail_errors_t  in_Filter );
static jcu_errorcode_t  R_JCU_SetQuantizationTable_ST( const jcu_table_no_t  in_TableNum,  const uint8_t* const  in_Table );
static jcu_errorcode_t  R_JCU_SetHuffmanTable_ST( const jcu_table_no_t  in_TableNum,  const jcu_huff_t  in_Type,  const uint8_t* const  in_Table );
static jcu_errorcode_t  R_JCU_SetEncodeParam_ST( const jcu_encode_param_t* const  in_Encode,  const jcu_buffer_param_t* const  in_Buffer );
static jcu_errorcode_t  R_JCU_GetEncodedSize_ST( size_t* const  out_Size );


/***********************************************************************
* Function: R_JCU_Initialize
*    Initialize the driver
*
* Arguments:
*    in_out_Config - (in/out) NULL or <jcu_config_t>*
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_Initialize( jcu_config_t*  in_out_Config )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_Initialize_ST( in_out_Config );
#else
    return  R_JCU_Initialize_MT( in_out_Config );
#endif
}


/***********************************************************************
* Function: R_JCU_TerminateAsync
*    Terminate the driver function
*
* Arguments:
*    in_CallbackFunction - <r_co_function_t>
*    in_Argument         - Argument of "in_CallbackFunction"
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t R_JCU_TerminateAsync( r_co_function_t  in_OnFinalized,  volatile void*  in_OnFinalizedArgument )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_TerminateAsync_ST( in_OnFinalized,  in_OnFinalizedArgument );
#else
    return  R_JCU_TerminateAsync_MT( in_OnFinalized,  in_OnFinalizedArgument );
#endif
}


/***********************************************************************
* Function: R_JCU_SelectCodec
*    Select Codec Type
*
* Arguments:
*    in_Codec - <jcu_codec_t>
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_SelectCodec( const jcu_codec_t  in_Codec )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_SelectCodec_ST( in_Codec );
#else
    return  R_JCU_SelectCodec_MT( in_Codec );
#endif
}


/***********************************************************************
* Function: R_JCU_StartAsync
*    Start decoding or encoding. Asynchronized.
*
* Arguments:
*    in_OnFinished         - <r_co_function_t>
*    in_OnFinishedArgument - Argument of "in_OnFinished"
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_StartAsync( r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_StartAsync_ST( in_OnFinished,  in_OnFinishedArgument );
#else
    return  R_JCU_StartAsync_MT( in_OnFinished,  in_OnFinishedArgument );
#endif
}


/***********************************************************************
* Function: R_JCU_ContinueAsync
*    Start decoding or encoding. Asynchronized.
*
* Arguments:
*    in_Type               - <jcu_continue_type_t>
*    in_OnFinished         - <r_co_function_t>
*    in_OnFinishedArgument - Argument of "in_OnFinished"
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_ContinueAsync( const jcu_continue_type_t  in_Type,
                                      r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_ContinueAsync_ST( in_Type,  in_OnFinished,  in_OnFinishedArgument );
#else
    return  R_JCU_ContinueAsync_MT( in_Type,  in_OnFinished,  in_OnFinishedArgument );
#endif
}


/***********************************************************************
* Function: R_JCU_SetDecodeParam
*    Set parameter for decode.
*
* Arguments:
*    in_Decode - Select the encode parameter for decoding. <jcu_decode_param_t>
*    in_Buffer - Buffer settings for decode. <jcu_buffer_param_t>
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
/* ->SEC M1.1.1 API Function */
jcu_errorcode_t R_JCU_SetDecodeParam(
    const jcu_decode_param_t*const  in_Decode,
    const jcu_buffer_param_t*const  in_Buffer )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_SetDecodeParam_ST( in_Decode,  in_Buffer );
#else
    return  R_JCU_SetDecodeParam_MT( in_Decode,  in_Buffer );
#endif
}


/***********************************************************************
* Function: R_JCU_SetPauseForImageInfo
*    Whether stop or not stop for R_JCU_GetImageInfo()
*
* Arguments:
*    in_IsPause - Whether stop or not stop
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t R_JCU_SetPauseForImageInfo( const bool_t  in_IsPause )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_SetPauseForImageInfo_ST( in_IsPause );
#else
    return  R_JCU_SetPauseForImageInfo_MT( in_IsPause );
#endif
}


/***********************************************************************
* Function: R_JCU_GetImageInfo
*    GetImageInfo
*
* Arguments:
*    out_Buffer - Image information. <jcu_image_info_t>
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t R_JCU_GetImageInfo( jcu_image_info_t* const  out_Buffer )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_GetImageInfo_ST( out_Buffer );
#else
    return  R_JCU_GetImageInfo_MT( out_Buffer );
#endif
}


/***********************************************************************
* Function: R_JCU_SetErrorFilter
*    SetErrorFilter
*
* Arguments:
*    in_Filter - enable bit of error. <jcu_int_detail_errors_t>
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_SetErrorFilter( jcu_int_detail_errors_t  in_Filter )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_SetErrorFilter_ST( in_Filter );
#else
    return  R_JCU_SetErrorFilter_MT( in_Filter );
#endif
}


/***********************************************************************
* Function: R_JCU_SetQuantizationTable
*    SetQuantizationTable
*
* Arguments:
*    tableNo - The table number to set the value. <jcu_table_no_t>
*    table   - The body of the table to set.
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_SetQuantizationTable(
    const jcu_table_no_t  in_TableNum,  const uint8_t* const  in_Table )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_SetQuantizationTable_ST( in_TableNum,  in_Table );
#else
    return  R_JCU_SetQuantizationTable_MT( in_TableNum,  in_Table );
#endif
}


/***********************************************************************
* Function: R_JCU_SetHuffmanTable
*    Set the Huffman table
*
* Arguments:
*    in_TableNum - The table number to set the value. <jcu_table_no_t>
*    in_Type     - The type of Huffman table type (AC or DC). <jcu_huff_t>
*    in_Table    - The body of the table to set
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_SetHuffmanTable( const jcu_table_no_t  in_TableNum,  const jcu_huff_t  in_Type,
                                        const uint8_t* const  in_Table )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_SetHuffmanTable_ST( in_TableNum,  in_Type,  in_Table );
#else
    return  R_JCU_SetHuffmanTable_MT( in_TableNum,  in_Type,  in_Table );
#endif
}


/***********************************************************************
* Function: R_JCU_SetEncodeParam
*    Set the parameter fo encoding
*
* Arguments:
*    in_Encode - Select the encode parameter for encoding. <jcu_encode_param_t>
*    in_Buffer - Select the buffer settings for encoding. <jcu_buffer_param_t>
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_SetEncodeParam( const jcu_encode_param_t* const  in_Encode,
                                       const jcu_buffer_param_t* const  in_Buffer )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_SetEncodeParam_ST( in_Encode,  in_Buffer );
#else
    return  R_JCU_SetEncodeParam_MT( in_Encode,  in_Buffer );
#endif
}


/***********************************************************************
* Function: R_JCU_GetEncodedSize
*    GetEncodedSize
*
* Arguments:
*    out_Size - EncodedSize
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_GetEncodedSize( size_t* const  out_Size )
{
#if  ! JCU_MULTI_THREAD
    return  R_JCU_GetEncodedSize_ST( out_Size );
#else
    return  R_JCU_GetEncodedSize_MT( out_Size );
#endif
}


/***********************************************************************
* Implement: R_JCU_Initialize_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
static jcu_errorcode_t  R_JCU_Initialize_ST( jcu_config_t*  in_out_Config )
/* <-SEC M1.1.1 */
{
    errnum_t          e;
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;

    R_UNREFERENCED_VARIABLE( in_out_Config );

    if (!( self->AsyncStatus.Status == JCU_STATUS_UNDEF ))
    {
        e = JCU_ERROR_STATUS_1;
        goto fin;  /* Error: Already initialized. */
    }

    /* Initialize the variable */
    self->AsyncStatus.OnInterrupted = (r_co_function_t) R_JCU_OnInterrupted;
    self->AsyncStatus.OnInterruptedArgument = NULL;
    self->AsyncStatus.OnFinished = NULL;
    self->AsyncStatus.OnFinishedArgument = NULL;
    self->AsyncStatus.OnFinalized = NULL;
    self->AsyncStatus.OnFinalizedArgument = NULL;
    self->AsyncStatus.InterruptFlags = 0u;
    self->AsyncStatus.CancelFlags = 0u;
    self->AsyncStatus.IsPaused = false;
    self->AsyncStatus.SubStatusFlags = 0u;
    self->IsCountMode = false;
    self->ErrorFilter = JCU_INT_ERROR_ALL;

    /* User defined initialize */
    e= R_JCU_OnInitialize();
    if(e)
    {
        goto fin;
    }

    /* Set the register */
    e= JCU_SetRegisterForInitialize();
    if(e)
    {
        goto fin;
    }

    /* Set the updated status */
    self->AsyncStatus.Status = JCU_STATUS_INIT;

    /* Initialize the status for the encode */
    self->Codec = JCU_CODEC_NOT_SELECTED;

    e = 0;
fin:
    if ( e )    /* Rollback */
    {
        bool_t  is_event = false;

        R_JCU_TerminateAsync( (r_co_function_t) R_CO_SetTrue,  &is_event );
        while ( ! is_event )
        {
            /* Pooling */
        }
    }
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_TerminateAsync_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
static jcu_errorcode_t  R_JCU_TerminateAsync_ST(
    r_co_function_t  in_OnFinalized,  volatile void*  in_OnFinalizedArgument )
/* <-SEC M1.1.1 */
{
    errnum_t  e;
    bool_t    was_enabled = false;
    bool_t    is_callback = false;
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;
    int_fast32_t  oblivious = 0;

#if  ! defined( JCU_RELEASE )
    if ( in_OnFinalized == NULL )
    {
        e = JCU_ERROR_PARAM_1;     /* Error: */
        goto fin;
    }
#endif

    self->AsyncStatus.ReturnValue = 0;

    was_enabled = R_JCU_DisableInterrupt();

    /* Check the status */
    if ( self->AsyncStatus.Status == JCU_STATUS_UNDEF )
    {
        e = JCU_ERROR_OK;
        goto fin;
    } /* end if */

    if ( self->AsyncStatus.Status == JCU_STATUS_RUN )
    {
        self->AsyncStatus.OnFinalized = in_OnFinalized;
        self->AsyncStatus.OnFinalizedArgument = in_OnFinalizedArgument;

#if  ! defined( JCU_RELEASE )
        if (!( self->AsyncStatus.OnFinalizedArgument != self->AsyncStatus.OnFinishedArgument ))
        {
            oblivious = __LINE__;
        }
#endif

        self->AsyncStatus.CancelFlags |= JCU_FINALIZE_REQUEST;

        /* "R_JCU_OnInterrupted" will be called */
    }
    else
    {
        e = R_JCU_FinalizeStep2_Sub();
        is_callback = true;
    }

    if ( oblivious != 0 )
    {
        e=E_JCU_OBLIVIOUS_3;    /* Error: */
        goto fin;
    }
    e=0;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
    R_CO_SetErrNum( e );
    if ( is_callback )
    {
        in_OnFinalized( in_OnFinalizedArgument );
    }
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Function: R_JCU_FinalizeStep2_Sub
*    Terminate the driver function
*
* Arguments:
*    None
*
* Return Value:
*    <jcu_errorcode_t> type.
************************************************************************/
jcu_errorcode_t  R_JCU_FinalizeStep2_Sub(void)
{
    errnum_t  e;
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;


    /* Set the updated status */
    self->AsyncStatus.CancelFlags = 0;
    self->AsyncStatus.Status = JCU_STATUS_UNDEF;


    /* Run the additional function selected by parameter */
    e= R_JCU_OnFinalize();
    if(e)
    {
        goto fin;
    }

    e=0;
fin:
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_SelectCodec_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
jcu_errorcode_t R_JCU_SelectCodec_ST( const jcu_codec_t codec )
/* <-SEC M1.1.1 */
{
    errnum_t          e;
    bool_t            was_enabled; /* = false; */ /* QAC 3197 */
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;


    was_enabled = R_JCU_DisableInterrupt();

#if  ! defined( JCU_RELEASE )
    if (!(
                self->AsyncStatus.Status == JCU_STATUS_INIT  ||
                self->AsyncStatus.Status == JCU_STATUS_SELECTED  ||
                self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_2;     /* Error: */
        goto fin;
    }

    e= JCU_ParaCheckSelectCodec(codec);
    if(e)
    {
        goto fin;
    }
#endif

    /* Clear the count mode flag */
    self->IsCountMode = false;

    /* Register set */
    JCU_SetRegisterForCodec(codec);

    /* Set the updated status */
    self->AsyncStatus.Status = JCU_STATUS_SELECTED;
    self->AsyncStatus.IsPaused = false;

    /* Set codec type to internal information variable */
    switch (codec)
    {
    case JCU_ENCODE:
        self->Codec = JCU_STATUS_ENCODE;
        break;
    case JCU_DECODE:
        self->Codec = JCU_STATUS_DECODE;
        break;
        /* ->QAC 2018 : For MISRA 15.3, SEC R3.5.2 */
    default:
        /* <-QAC 2018 */
        /* NOT REACHED */
        break;
    } /* end switch */

    e = 0;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_StartAsync_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
static jcu_errorcode_t  R_JCU_StartAsync_ST( r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument )
/* <-SEC M1.1.1 */
{
    errnum_t          e;
    bool_t            was_enabled; /* = false; */ /* QAC 3197 */
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;


    was_enabled = R_JCU_DisableInterrupt();

#if  ! defined( JCU_RELEASE )
    if (!( self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_3;     /* Error: */
        goto fin;
    }

    if (!( in_OnFinished != NULL ))
    {
        e = JCU_ERROR_PARAM_2;     /* Error: */
        goto fin;
    }

    e= JCU_ParaCheckStart();
    if(e)
    {
        goto fin;
    }
#endif

    self->AsyncStatus.OnFinished = in_OnFinished;
    self->AsyncStatus.OnFinishedArgument = in_OnFinishedArgument;
    self->AsyncStatus.ReturnValue = 0;

    self->AsyncStatus.Status = JCU_STATUS_RUN;
    JCU_SetRegisterForStart( self->ErrorFilter );

    e = 0;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_ContinueAsync_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
static jcu_errorcode_t  R_JCU_ContinueAsync_ST( const jcu_continue_type_t  in_Type,
        r_co_function_t  in_OnFinished,  volatile void*  in_OnFinishedArgument )
/* <-SEC M1.1.1 */
{
    errnum_t          e;
    bool_t            was_enabled; /* = false; */ /* QAC 3197 */
    enum {            num_8_bit = 8 };
    bit_flags32_t     mask;
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;


    was_enabled = R_JCU_DisableInterrupt();

#if  ! defined( JCU_RELEASE )
    if (!( self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_4;    /* Error: */
        goto fin;
    }

    if (!( self->AsyncStatus.IsPaused == true ))
    {
        e = JCU_ERROR_STATUS_5;    /* Error: */
        goto fin;
    }

    if (!( in_OnFinished != NULL ))
    {
        e = JCU_ERROR_PARAM_3;    /* Error: */
        goto fin;
    }
#endif

    /* Clear the sub status flag */
    switch ( in_Type )
    {
    case JCU_INPUT_BUFFER:
        mask = ( ( JCU_JINTS1_JINF | JCU_JINTS1_DINLF ) << ( sizeof( JCU.JINTS0 ) * num_8_bit ) );
        break;
    case JCU_OUTPUT_BUFFER:
        mask = ( ( JCU_JINTS1_DOUTLF | JCU_JINTS1_JOUTF ) << ( sizeof( JCU.JINTS0 ) * num_8_bit ) );
        break;
    default:
        if (!( in_Type == JCU_IMAGE_INFO ))
        {
            e=JCU_ERROR_PARAM_4;    /* Error: */
            goto fin;
        }
        mask = JCU_JINTS0_INS3;
        break;
    }
    if (!( IS_ANY_BITS_SET( self->AsyncStatus.SubStatusFlags, mask ) ))
    {
        e=JCU_ERROR_PARAM_5;    /* Error: */
        goto fin;
    }
    self->AsyncStatus.SubStatusFlags &= ~mask;
    self->AsyncStatus.ReturnValue = 0;

    /* JCU will restart */
    if ( self->AsyncStatus.SubStatusFlags == 0 )
    {
        /* Change the status */
        self->AsyncStatus.Status = JCU_STATUS_RUN;
        self->AsyncStatus.IsPaused = false;

        /* Register set */
        JCU_SetRegisterForContinue( in_Type );
    }

    /* JCU will not restart */
    else
    {
        /* Register set */
        JCU_SetRegisterForContinue( in_Type );

        /* Change the status */
        self->AsyncStatus.Status = JCU_STATUS_READY;

        /* Set event flags */
        in_OnFinished( in_OnFinishedArgument );
    }

    e = 0;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_SetDecodeParam_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
static jcu_errorcode_t  R_JCU_SetDecodeParam_ST(
    const jcu_decode_param_t*const  in_Decode,
    const jcu_buffer_param_t*const  in_Buffer )
/* <-SEC M1.1.1 */
{
    jcu_errorcode_t   e;
    bool_t            was_enabled; /* = false; */ /* QAC 3197 */
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;


    was_enabled = R_JCU_DisableInterrupt();

#if  ! defined( JCU_RELEASE )
    if (!(
                self->AsyncStatus.Status == JCU_STATUS_SELECTED  ||
                self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_6;     /* Error: */
        goto fin;
    }
    if (!(
                self->Codec == JCU_STATUS_DECODE ))
    {
        e = JCU_ERROR_STATUS_7;     /* Error: */
        goto fin;
    }

    if (!( in_Decode != NULL ))
    {
        e = JCU_ERROR_PARAM_6;     /* Error: */
        goto fin;
    }

    if (! (in_Buffer != NULL ))
    {
        e = JCU_ERROR_PARAM_7;     /* Error: */
        goto fin;
    }

    e = JCU_ParaCheckSetDecodeParam(in_Decode, in_Buffer);
    if(e)
    {
        goto fin;
    }
#endif

    /* Register set */
    JCU_SetRegisterForSetDecodePrm(in_Decode, in_Buffer);

    /* Set the updated status */
    self->AsyncStatus.Status = JCU_STATUS_READY;

    e = 0;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_SetPauseForImageInfo_ST
************************************************************************/
static jcu_errorcode_t R_JCU_SetPauseForImageInfo_ST( const bool_t  in_IsPause )
{
    jcu_errorcode_t   e;
    bool_t            was_enabled; /* = false; */ /* QAC 3197 */
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;


    was_enabled = R_JCU_DisableInterrupt();

#if  ! defined( JCU_RELEASE )
    if (!(
                self->AsyncStatus.Status == JCU_STATUS_SELECTED  ||
                self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_8;     /* Error: */
        goto fin;
    }
    if (!(
                self->Codec == JCU_STATUS_DECODE ))
    {
        e = JCU_ERROR_STATUS_9;     /* Error: */
        goto fin;
    }
#endif

    /* Register set */
    JCU_SetRegisterForSetPause( in_IsPause, self->ErrorFilter );

    e = 0;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_GetImageInfo_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
jcu_errorcode_t R_JCU_GetImageInfo_ST( jcu_image_info_t* const  out_Buffer )
/* <-SEC M1.1.1 */
{
    jcu_errorcode_t   e;
    bool_t            was_enabled; /* = false; */ /* QAC 3197 */
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;


    was_enabled = R_JCU_DisableInterrupt();

#if  ! defined( JCU_RELEASE )
    if (!( self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_A;     /* Error: */
        goto fin;
    }

    if (!( self->Codec == JCU_STATUS_DECODE ))
    {
        e = JCU_ERROR_STATUS_B;     /* Error: */
        goto fin;
    }

    if (!( out_Buffer != NULL ))
    {
        e = JCU_ERROR_PARAM_8;     /* Error: */
        goto fin;
    }

    e = JCU_ParaCheckGetImageInfo( out_Buffer );
    if(e)
    {
        goto fin;
    }
#endif

    /* Register set */
    JCU_GetRegisterForGetImageInfo( out_Buffer );

    e = 0;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_SetErrorFilter_ST
************************************************************************/
static jcu_errorcode_t  R_JCU_SetErrorFilter_ST( jcu_int_detail_errors_t  in_Filter )
{
    jcu_errorcode_t  e;

    jcu_internal_information_t* const  self = &gs_jcu_internal_information;

#if  ! defined( JCU_RELEASE )
    if (!(
                self->AsyncStatus.Status == JCU_STATUS_INIT  ||
                self->AsyncStatus.Status == JCU_STATUS_SELECTED  ||
                self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_C;     /* Error: */
        goto fin;
    }
#endif

    if ( IS_ANY_BITS_SET( in_Filter, (bit_flags_fast32_t) ~JCU_INT_ERROR_ALL ) )
    {
        e = JCU_ERROR_PARAM_9;    /* Error: */
        goto fin;
    }

    self->ErrorFilter = in_Filter;

    e = 0;
fin:
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_SetQuantizationTable_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
static jcu_errorcode_t  R_JCU_SetQuantizationTable_ST(
    const jcu_table_no_t  in_TableNum,  const uint8_t* const  in_Table )
/* <-SEC M1.1.1 */
{
    jcu_errorcode_t   e;
    bool_t            was_enabled; /* = false; */ /* QAC 3197 */
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;

#if  ! defined( JCU_RELEASE )
    e = JCU_ParaCheckSetQuantizationTbl( in_TableNum );
    if (e != JCU_ERROR_OK)
    {
        goto efin;
    }
#endif

    was_enabled = R_JCU_DisableInterrupt();

#if  ! defined( JCU_RELEASE )
    if (!(
                self->AsyncStatus.Status == JCU_STATUS_SELECTED  ||
                self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_D;     /* Error: */
        goto fin;
    }
    if (!(
                self->Codec == JCU_STATUS_ENCODE ))
    {
        e = JCU_ERROR_STATUS_E;     /* Error: */
        goto fin;
    }

    if (!( in_Table != NULL ))
    {
        e = JCU_ERROR_PARAM_A;     /* Error: */
        goto fin;
    }
#endif

    /* Register set */
    JCU_SetRegisterForSetQtTable( in_TableNum,  in_Table );

    e = 0;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
efin:
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_SetHuffmanTable_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
static jcu_errorcode_t  R_JCU_SetHuffmanTable_ST( const jcu_table_no_t  in_TableNum,  const jcu_huff_t  in_Type,
        const uint8_t* const  in_Table )
/* <-SEC M1.1.1 */
{
    jcu_errorcode_t   e;
    bool_t            was_enabled = false; /* QAC 3197 */
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;

#if  ! defined( JCU_RELEASE )
    e= JCU_ParaCheckSetHuffmanTable( in_TableNum,  in_Type );
    if(e)
    {
        goto fin;
    }
#endif


    was_enabled = R_JCU_DisableInterrupt();

#if  ! defined( JCU_RELEASE )
    if (!(
                self->AsyncStatus.Status == JCU_STATUS_SELECTED  ||
                self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_F;     /* Error: */
        goto fin;
    }
    if (!(
                self->Codec == JCU_STATUS_ENCODE ))
    {
        e = JCU_ERROR_STATUS_10;     /* Error: */
        goto fin;
    }

    if (!( in_Table != NULL ))
    {
        e = JCU_ERROR_PARAM_B;     /* Error: */
        goto fin;
    }
#endif


    /* Register set */
    JCU_SetRegisterForSetHuffTbl( in_TableNum,  in_Type,  in_Table );

    e = JCU_ERROR_OK;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_SetEncodeParam_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
static jcu_errorcode_t  R_JCU_SetEncodeParam_ST( const jcu_encode_param_t* const  in_Encode,
        const jcu_buffer_param_t* const  in_Buffer )
/* <-SEC M1.1.1 */
{
    jcu_errorcode_t   e;
    bool_t            was_enabled; /* = false; */ /* QAC 3197 */
    jcu_internal_information_t* const  self = &gs_jcu_internal_information;


    was_enabled = R_JCU_DisableInterrupt();

#if  ! defined( JCU_RELEASE )
    if (!(
                self->AsyncStatus.Status == JCU_STATUS_SELECTED  ||
                self->AsyncStatus.Status == JCU_STATUS_READY ))
    {
        e = JCU_ERROR_STATUS_11;     /* Error: */
        goto fin;
    }
    if (!(
                self->Codec == JCU_STATUS_ENCODE ))
    {
        e = JCU_ERROR_STATUS_12;     /* Error: */
        goto fin;
    }

    if (!( in_Encode != NULL ))
    {
        e = JCU_ERROR_PARAM_C;     /* Error: */
        goto fin;
    }

    if (!( in_Buffer != NULL ))
    {
        e = JCU_ERROR_PARAM_D;     /* Error: */
        goto fin;
    }

    e= JCU_ParaCheckEncodeParam( in_Encode,  in_Buffer );
    if(e)
    {
        goto fin;
    }
#endif

    /* Register set */
    JCU_SetRegisterForSetEncodePrm( in_Encode,  in_Buffer );

    /* Set the updated status */
    self->AsyncStatus.Status = JCU_STATUS_READY;

    e = 0;
fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Implement: R_JCU_GetEncodedSize_ST
************************************************************************/
/* ->SEC M1.1.1 API Function */
static jcu_errorcode_t  R_JCU_GetEncodedSize_ST( size_t* const  out_Size )
/* <-SEC M1.1.1 */
{
    jcu_errorcode_t   e;

#if  ! defined( JCU_RELEASE )
    {
        jcu_internal_information_t* const  self = &gs_jcu_internal_information;

        if (!( self->AsyncStatus.Status == JCU_STATUS_READY ))
        {
            e = JCU_ERROR_STATUS_13;    /* Error: */
            goto fin;
        }
    }
#endif


    JCU_GetEncodedSize(out_Size);


    e = 0;
#if  ! defined( JCU_RELEASE )
fin:
#endif
    R_CO_SetErrNum( e );
    return  (jcu_errorcode_t) e;
}


/***********************************************************************
* Function: R_JCU_GetInternalInformation
*    R_JCU_GetInternalInformation
*
* Arguments:
*    None
*
* Return Value:
*    <jcu_internal_information_t> type.
************************************************************************/
jcu_internal_information_t*  R_JCU_GetInternalInformation(void)
{
    return  &gs_jcu_internal_information;
}


/***********************************************************************
* Function: R_JCU_OnInterrupting
*    Operations on interrupting
*
* Return Value:
*    Error code, 0=No error
************************************************************************/
errnum_t  R_JCU_OnInterrupting()
{
    errnum_t  e;

    jcu_internal_information_t* const  self = R_JCU_GetInternalInformation();

#if  ! defined( JCU_RELEASE )
    if ( self == NULL )
    {
        e=JCU_ERROR_PARAM_26;    /* Error: */
        goto fin;
    }
#endif

    e=0;

    self->AsyncStatus.Status = JCU_STATUS_INTERRUPTING;

    JCU_OnInterrupting( &self->AsyncStatus.InterruptFlags );


    /* Set state */
    self->AsyncStatus.Status = JCU_STATUS_INTERRUPTED;

    /* Call callback function */
    self->AsyncStatus.OnInterrupted( self->AsyncStatus.OnInterruptedArgument );

fin:
    return  e;
}


/***********************************************************************
* Function: R_JCU_OnInterrupted
*    Bottom half operations on interrupting
*
* Return Value:
*    Error code, 0=No error
************************************************************************/
errnum_t  R_JCU_OnInterrupted(void)
{
    enum { num_8_bit = 8 };
    bit_flags32_t const  sub_status_bits =
        ( JCU_JINTS0_INS3 ) |
        ( JCU_JINTS1_JINF   << (sizeof( JCU.JINTS0 ) * num_8_bit) ) |
        ( JCU_JINTS1_DOUTLF << (sizeof( JCU.JINTS0 ) * num_8_bit) ) |
        ( JCU_JINTS1_DINLF  << (sizeof( JCU.JINTS0 ) * num_8_bit) ) |
        ( JCU_JINTS1_JOUTF  << (sizeof( JCU.JINTS0 ) * num_8_bit) );

    errnum_t  e;
    errnum_t  ee;
    uint32_t  interrupt_flags;
    bool_t    was_enabled = false;
    bool_t    is_end;
    bool_t    or;
    bool_t    is_callback_1 = false;
    bool_t    is_callback_2 = false;

    jcu_internal_information_t* const  self = R_JCU_GetInternalInformation();

#if  ! defined( JCU_RELEASE )
    if ( self == NULL )
    {
        e=JCU_ERROR_PARAM_27;    /* Error: */
        goto fin;
    }
#endif

    e=0;

    was_enabled = R_JCU_DisableInterrupt();

    if (!( self->AsyncStatus.Status != JCU_STATUS_UNDEF ))
    {
        e=E_STATE;    /* Error: */
        goto fin;
    }

    {
        interrupt_flags = self->AsyncStatus.InterruptFlags;
        self->AsyncStatus.InterruptFlags = 0u;
    } /* This code must do in I-Lock area. */


    if ( IS_ANY_BITS_SET( interrupt_flags,
                          ( JCU_JINTS1_DBTF | JCU_JINTS1_CBTF ) << (sizeof( JCU.JINTS0 ) * num_8_bit) ) )
    {
        self->AsyncStatus.IsPaused = false;
        self->AsyncStatus.SubStatusFlags = 0u;
        is_end = true;
    }
    else if ( IS_ANY_BITS_SET( interrupt_flags,  sub_status_bits ) )
    {
        self->AsyncStatus.IsPaused = true;
        self->AsyncStatus.SubStatusFlags = interrupt_flags & sub_status_bits;
        is_end = false;
    }
    else
    {
        is_end = false;
    }


    or = ( (is_end) || (self->AsyncStatus.IsPaused) );
    if (!or)
    {
        or = IS_BIT_SET( interrupt_flags,  JCU_JINTS0_INS5 );    /* or = for MISRA 12.4 */
    }
    if ( or )
    {
        /* Set "AsyncStatus" */
        self->AsyncStatus.Status = JCU_STATUS_READY;
        is_callback_1 = true;;
    }
    else
    {
        self->AsyncStatus.Status = JCU_STATUS_RUN;
    }


    if ( IS_BIT_SET( interrupt_flags,  JCU_JINTS0_INS5 ) )  /* Check input data and try to flush the cache. */
    {
        jcu_detail_error_t  ed;

        JCU_GetRegisterForGetErrorInfo( &ed );
        e = (errnum_t) ed;
        goto fin;
    }


    if ( is_callback_1 )
    {
        if ( IS_BIT_SET( self->AsyncStatus.CancelFlags,  JCU_FINALIZE_REQUEST ) )
        {
            is_callback_2 = true;
            ee= R_JCU_FinalizeStep2_Sub();
            if(e==0)
            {
                e=ee;
            }
        }
    }

fin:
    if ( was_enabled )
    {
        R_JCU_EnableInterrupt();
    }

    R_CO_SetErrNum( e );

    if ( is_callback_2 )
    {
        self->AsyncStatus.OnFinalized( self->AsyncStatus.OnFinalizedArgument );
    }
    if ( is_callback_1 )
    {
        self->AsyncStatus.OnFinished( self->AsyncStatus.OnFinishedArgument );
    }

    return  e;
}

