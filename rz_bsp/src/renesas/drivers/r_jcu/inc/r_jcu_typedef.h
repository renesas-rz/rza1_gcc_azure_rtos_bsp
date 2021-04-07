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
/******************************************************************************
* File: r_jcu_typedef.h
*    JPEG Codec Unit (JCU) driver's API. Main Header.
*
* - $ModuleRevision: {Masters}\JCU\RZ2\Single\04_RZA1 $
* - $Rev: $
* - $Date: 2018-03-06T12:53:32+09:00 $
******************************************************************************/

#ifndef  R_JCU_API_TYPEDEF_H
#define  R_JCU_API_TYPEDEF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "r_typedefs.h"
#include  "rz_co_typedef.h"


/******************************************************************************
Macro definitions
******************************************************************************/
#define  JCU_NUMBER_OF_QUANTIZATION_TABLE_DATA   64
#define  JCU_NUMBER_OF_HUFFMAN_TABLE_DATA_DC     28
#define  JCU_NUMBER_OF_HUFFMAN_TABLE_DATA_AC    178


/***********************************************************************
* Macro: JCU_VERSION
*    VERSION
************************************************************************/
#define  JCU_VERSION  300


/***********************************************************************
* Macro: JCU_VERSION_STRING
*    String of <JCU_VERSION>
************************************************************************/
#define  JCU_VERSION_STRING  "3.00"


/***********************************************************************
* Macro: JCU_RELEASE
*    Define by 1 or not define
************************************************************************/
/* #define  JCU_RELEASE  1 */


/***********************************************************************
* Define: JCU_MULTI_THREAD
*    0 or 1.
************************************************************************/
#define  JCU_MULTI_THREAD  0


/***********************************************************************
* Macro: JCU_BUFFER_ALIGNMENT
************************************************************************/
#define  JCU_BUFFER_ALIGNMENT  8


/***********************************************************************
* Macro: JCU_COLOR_ELEMENT_NUM
*    Y,Cb and Cr
************************************************************************/
#define  JCU_COLOR_ELEMENT_NUM  3


/* Section: Start_of_ErrNum */
/***********************************************************************
* Constants: JCU_ERROR
*    Error code that means detecting error by software.
************************************************************************/
/* Constant: JCU_ERROR_MASK
*    Mask for checking JCU_ERROR. */
#define  JCU_ERROR_MASK  0x00FF0000
#define  JCU_ERROR       0x00450000
#define  JCU_ERROR_OK    0  /* Constant: JCU_ERROR_OK */

/* Sub class of E_OBLIVIOUS */
#define  E_JCU_OBLIVIOUS_1    0x0B450001
#define  E_JCU_OBLIVIOUS_2    0x0B450002
#define  E_JCU_OBLIVIOUS_3    0x0B450003


/***********************************************************************
* Constant: JCU_ERROR_PARAM
*    Error code that means original.
************************************************************************/
/* Constant: JCU_ERROR_PARAM_MASK
*    Mask for checking JCU_ERROR_PARAM. */
#define  JCU_ERROR_PARAM_MASK  0x7FFFFF00
#define  JCU_ERROR_PARAM       0x01450100
#define  JCU_ERROR_PARAM_1     0x01450101
#define  JCU_ERROR_PARAM_2     0x01450102
#define  JCU_ERROR_PARAM_3     0x01450103
#define  JCU_ERROR_PARAM_4     0x01450104
#define  JCU_ERROR_PARAM_5     0x01450105
#define  JCU_ERROR_PARAM_6     0x01450106
#define  JCU_ERROR_PARAM_7     0x01450107
#define  JCU_ERROR_PARAM_8     0x01450108
#define  JCU_ERROR_PARAM_9     0x01450109
#define  JCU_ERROR_PARAM_A     0x0145010A
#define  JCU_ERROR_PARAM_B     0x0145010B
#define  JCU_ERROR_PARAM_C     0x0145010C
#define  JCU_ERROR_PARAM_D     0x0145010D
#define  JCU_ERROR_PARAM_E     0x0145010E
#define  JCU_ERROR_PARAM_F     0x0145010F
#define  JCU_ERROR_PARAM_10    0x01450110
#define  JCU_ERROR_PARAM_11    0x01450111
#define  JCU_ERROR_PARAM_12    0x01450112
#define  JCU_ERROR_PARAM_13    0x01450113
#define  JCU_ERROR_PARAM_14    0x01450114
#define  JCU_ERROR_PARAM_15    0x01450115
#define  JCU_ERROR_PARAM_16    0x01450116
#define  JCU_ERROR_PARAM_17    0x01450117
#define  JCU_ERROR_PARAM_18    0x01450118
#define  JCU_ERROR_PARAM_19    0x01450119
#define  JCU_ERROR_PARAM_1A    0x0145011A
#define  JCU_ERROR_PARAM_1B    0x0145011B
#define  JCU_ERROR_PARAM_1C    0x0145011C
#define  JCU_ERROR_PARAM_1D    0x0145011D
#define  JCU_ERROR_PARAM_1E    0x0145011E
#define  JCU_ERROR_PARAM_1F    0x0145011F
#define  JCU_ERROR_PARAM_20    0x01450120
#define  JCU_ERROR_PARAM_21    0x01450121
#define  JCU_ERROR_PARAM_22    0x01450122
#define  JCU_ERROR_PARAM_23    0x01450123
#define  JCU_ERROR_PARAM_24    0x01450124
#define  JCU_ERROR_PARAM_25    0x01450125
#define  JCU_ERROR_PARAM_26    0x01450126
#define  JCU_ERROR_PARAM_27    0x01450127
#define  JCU_ERROR_PARAM_28    0x01450128
#define  JCU_ERROR_PARAM_29    0x01450129
#define  JCU_ERROR_PARAM_2A    0x0145012A
#define  JCU_ERROR_PARAM_2B    0x0145012B
#define  JCU_ERROR_PARAM_2C    0x0145012C
#define  JCU_ERROR_PARAM_2D    0x0145012D
#define  JCU_ERROR_PARAM_2E    0x0145012E
#define  JCU_ERROR_PARAM_2F    0x0145012F
#define  JCU_ERROR_PARAM_30    0x01450130
#define  JCU_ERROR_PARAM_31    0x01450131
#define  JCU_ERROR_PARAM_32    0x01450132
#define  JCU_ERROR_PARAM_33    0x01450133
#define  JCU_ERROR_PARAM_34    0x01450134
#define  JCU_ERROR_PARAM_35    0x01450135

/* For porting layer */
#define  JCU_ERROR_PARAM_61    0x01450161
#define  JCU_ERROR_PARAM_62    0x01450162
#define  JCU_ERROR_PARAM_63    0x01450163
#define  JCU_ERROR_PARAM_64    0x01450164


/***********************************************************************
* Constant: JCU_ERROR_STATUS
*    Error code that means original.
************************************************************************/
/* Constant: JCU_ERROR_STATUS_MASK
*    Mask for checking JCU_ERROR_STATUS. */
/* Sub class of E_STATE */
#define  JCU_ERROR_STATUS_MASK  0x7FFFFF00
#define  JCU_ERROR_STATUS       0x10450200
#define  JCU_ERROR_STATUS_1     0x10450201
#define  JCU_ERROR_STATUS_2     0x10450202
#define  JCU_ERROR_STATUS_3     0x10450203
#define  JCU_ERROR_STATUS_4     0x10450204
#define  JCU_ERROR_STATUS_5     0x10450205
#define  JCU_ERROR_STATUS_6     0x10450206
#define  JCU_ERROR_STATUS_7     0x10450207
#define  JCU_ERROR_STATUS_8     0x10450208
#define  JCU_ERROR_STATUS_9     0x10450209
#define  JCU_ERROR_STATUS_A     0x1045020A
#define  JCU_ERROR_STATUS_B     0x1045020B
#define  JCU_ERROR_STATUS_C     0x1045020C
#define  JCU_ERROR_STATUS_D     0x1045020D
#define  JCU_ERROR_STATUS_E     0x1045020E
#define  JCU_ERROR_STATUS_F     0x1045020F
#define  JCU_ERROR_STATUS_10    0x10450210
#define  JCU_ERROR_STATUS_11    0x10450211
#define  JCU_ERROR_STATUS_12    0x10450212
#define  JCU_ERROR_STATUS_13    0x10450213
#define  JCU_ERROR_STATUS_14    0x10450214


/***********************************************************************
* Constants: E_JCU_JCDERR
*    Error code that means detecting error by JCU hardware.
************************************************************************/
/* Constant: E_JCU_JCDERR_MASK
*    Mask for checking E_JCU_JCDERR. */
#define  E_JCU_JCDERR_MASK  0x7FFFE000
#define  E_JCU_JCDERR       0x01452000

#define  JCU_JCDERR_OK                0           /* Constant: JCU_JCDERR_OK               */
#define  JCU_JCDERR_SOI_NOT_FOUND     0x01452100  /* Constant: JCU_JCDERR_SOI_NOT_FOUND    */
#define  JCU_JCDERR_INVALID_SOF       0x01452200  /* Constant: JCU_JCDERR_INVALID_SOF      */
#define  JCU_JCDERR_UNPROVIDED_SOF    0x01452300  /* Constant: JCU_JCDERR_UNPROVIDED_SOF   */
#define  JCU_JCDERR_SOF_ACCURACY      0x01452400  /* Constant: JCU_JCDERR_SOF_ACCURACY     */
#define  JCU_JCDERR_DQT_ACCURACY      0x01452500  /* Constant: JCU_JCDERR_DQT_ACCURACY     */
#define  JCU_JCDERR_COMPONENT_1       0x01452600  /* Constant: JCU_JCDERR_COMPONENT_1      */
#define  JCU_JCDERR_COMPONENT_2       0x01452700  /* Constant: JCU_JCDERR_COMPONENT_2      */
#define  JCU_JCDERR_NO_SOF0_DQT_DHT   0x01452800  /* Constant: JCU_JCDERR_NO_SOF0_DQT_DHT  */
#define  JCU_JCDERR_SOS_NOT_FOUND     0x01452900  /* Constant: JCU_JCDERR_SOS_NOT_FOUND    */
#define  JCU_JCDERR_EOI_NOT_FOUND     0x01452A00  /* Constant: JCU_JCDERR_EOI_NOT_FOUND    */
#define  JCU_JCDERR_RESTART_INTERVAL  0x01452B00  /* Constant: JCU_JCDERR_RESTART_INTERVAL */
#define  JCU_JCDERR_IMAGE_SIZE        0x01452C00  /* Constant: JCU_JCDERR_IMAGE_SIZE       */
#define  JCU_JCDERR_LAST_MCU_DATA     0x01452D00  /* Constant: JCU_JCDERR_LAST_MCU_DATA    */
#define  JCU_JCDERR_BLOCK_DATA        0x01452E00  /* Constant: JCU_JCDERR_BLOCK_DATA       */

/* Section: End_of_ErrNum */


/******************************************************************************
Typedef definitions
******************************************************************************/
#if  JCU_MULTI_THREAD
typedef struct st_jcu_context_t  jcu_context_t;
#endif


/***********************************************************************
* Enumeration: jcu_status_information_t
*    Status
*
*    : JCU_STATUS_UNDEF        - 0x00, Undefined
*    : JCU_STATUS_INIT         - 0x01, Initialized
*    : JCU_STATUS_SELECTED     - 0x02, Codec is selected
*    : JCU_STATUS_READY        - 0x08, Ready
*    : JCU_STATUS_RUN          - 0x10, Run
*    : JCU_STATUS_INTERRUPTING - 0x40, Next will be Interrupting
*    : JCU_STATUS_INTERRUPTED  - 0x80, Next will be Interrupted
************************************************************************/
typedef enum
{
    JCU_STATUS_UNDEF        = 0x00,
    JCU_STATUS_INIT         = 0x01,
    JCU_STATUS_SELECTED     = 0x02,
    JCU_STATUS_READY        = 0x08,
    JCU_STATUS_RUN          = 0x10,
    JCU_STATUS_INTERRUPTING = 0x40,
    JCU_STATUS_INTERRUPTED  = 0x80
} jcu_status_information_t;


/***********************************************************************
* Structure: jcu_async_status_t
*    Status of asynchronize
************************************************************************/
typedef struct st_jcu_async_status_t  jcu_async_status_t;
struct st_jcu_async_status_t
{
    jcu_status_information_t  Status;
    bit_flags_fast32_t        SubStatusFlags;
    bool_t                    IsPaused;
    bit_flags_fast32_t        InterruptFlags;
    r_co_function_t           OnInterrupted;
    volatile void*            OnInterruptedArgument;
    r_co_function_t           OnFinished;
    volatile void*            OnFinishedArgument;
    r_co_function_t           OnFinalized;
    volatile void*            OnFinalizedArgument;
    errnum_t                  ReturnValue;
    bit_flags_fast32_t        CancelFlags;  /*<r_jcu_cancel_flags_t>*/
};


/* Section: Global */
/***********************************************************************
* Enumeration: jcu_sub_status_t
*    Sub Status
*
*    : JCU_SUB_INFOMATION_READY    - 0x00000008
*    : JCU_SUB_DECODE_OUTPUT_PAUSE - 0x00000100
*    : JCU_SUB_DECODE_INPUT_PAUSE  - 0x00000200
*    : JCU_SUB_ENCODE_OUTPUT_PAUSE - 0x00001000
*    : JCU_SUB_ENCODE_INPUT_PAUSE  - 0x00002000
************************************************************************/
typedef enum
{
    JCU_SUB_INFOMATION_READY    = 0x00000008,
    JCU_SUB_DECODE_OUTPUT_PAUSE = 0x00000100,
    JCU_SUB_DECODE_INPUT_PAUSE  = 0x00000200,
    JCU_SUB_ENCODE_OUTPUT_PAUSE = 0x00001000,
    JCU_SUB_ENCODE_INPUT_PAUSE  = 0x00002000,
    JCU_SUB_PAUSE_ALL           = JCU_SUB_INFOMATION_READY | JCU_SUB_DECODE_OUTPUT_PAUSE |
                                  JCU_SUB_DECODE_INPUT_PAUSE | JCU_SUB_ENCODE_OUTPUT_PAUSE | JCU_SUB_ENCODE_INPUT_PAUSE
} jcu_sub_state_t;
typedef bit_flags_fast32_t jcu_sub_status_t;


/***********************************************************************
* Enumeration: r_jcu_cancel_flag_t
*    Cancel Status
*
*    : JCU_CANNEL_REQUEST   - 0x00000001
*    : JCU_CANNELING        - 0x00000002
*    : JCU_CANNELED         - 0x00000004
*    : JCU_FINALIZE_REQUEST - 0x00000010
*    : JCU_FINALIZING       - 0x00000020
*    : JCU_FINALIZED        - 0x00000040
************************************************************************/
typedef enum
{
    JCU_CANNEL_REQUEST   = 0x00000001,
    JCU_CANNELING        = 0x00000002,
    JCU_CANNELED         = 0x00000004,
    JCU_FINALIZE_REQUEST = 0x00000010,
    JCU_FINALIZING       = 0x00000020,
    JCU_FINALIZED        = 0x00000040
} r_jcu_cancel_flag_t;
typedef bit_flags_fast32_t  r_jcu_cancel_flags_t;


/******************************************************************************
Struct & Enum definitions
******************************************************************************/

/***********************************************************************
* Enumeration: jcu_errorcode_t
*    JCU_ERROR_PARAM, ...
************************************************************************/
typedef errnum_t  jcu_errorcode_t;


/***********************************************************************
* Enumeration: jcu_detail_error_t
*    JCU_JCDERR_SOI_NOT_FOUND, ...
************************************************************************/
typedef errnum_t  jcu_detail_error_t;


/***********************************************************************
* Enumeration: jcu_codec_t
*    The type of codec
*
*    : JCU_ENCODE       - 0
*    : JCU_DECODE       - 1
*    : JCU_NOT_SELECTED - 2
************************************************************************/
typedef enum
{
    JCU_ENCODE       = 0,
    JCU_DECODE       = 1,
    JCU_NOT_SELECTED = 2
} jcu_codec_t;


/***********************************************************************
* Structure: jcu_count_mode_param_t
*    Buffer settings for separating decode or encode
************************************************************************/
typedef struct
{
    struct
    {
        /* Variable: isEnable
            true: It uses the count mode process. */
        bool_t      isEnable;

        /* Variable: isInitAddress
            Address is changed when processing is stopped. */
        bool_t      isInitAddress;

        /* Variable: restartAddress
            This is enable when isInitAddrss is true. */
        uint32_t   *restartAddress;

        /* Variable: dataCount
            Encode:8line Decode:8byte. */
        uint32_t    dataCount;

    } inputBuffer;

    struct
    {
        /* Variable: isEnable
            true: It uses the count mode process. */
        bool_t      isEnable;

        /* Variable: isInitAddress
            Address is changed when processing is stopped. */
        bool_t      isInitAddress;      /*    */

        /* Variable: restartAddress
            This is enable when isInitAddress is true. */
        uint32_t    *restartAddress;    /*     */

        /* Variable: dataCount
            Encode:8byte Decode:8line. */
        uint32_t    dataCount;

    } outputBuffer;
} jcu_count_mode_param_t;


/* Section: Global */
/***********************************************************************
* Enumeration: jcu_continue_type_t
*    Type of continue buffer
*
*    : JCU_INPUT_BUFFER  - 0
*    : JCU_OUTPUT_BUFFER - 1
*    : JCU_IMAGE_INFO    - 2
************************************************************************/
typedef enum
{
    JCU_INPUT_BUFFER,
    JCU_OUTPUT_BUFFER,
    JCU_IMAGE_INFO
} jcu_continue_type_t;


/***********************************************************************
* Constants: jcu_int_detail_error_t
*    Bit flags of detail error
*
*    JCU_INT_ERROR_RESTART_INTERVAL_DATA - INT7_MASK
*    JCU_INT_ERROR_SEGMENT_TOTAL_DATA    - INT6_MASK
*    JCU_INT_ERROR_MCU_BLOCK_DATA        - INT5_MASK
*    JCU_INT_ERROR_ALL                   - e.g. for mask
************************************************************************/
typedef enum
{
    JCU_INT_ERROR_RESTART_INTERVAL_DATA = 0x80u,
    JCU_INT_ERROR_SEGMENT_TOTAL_DATA    = 0x40u,
    JCU_INT_ERROR_MCU_BLOCK_DATA        = 0x20u,
    JCU_INT_ERROR_ALL                   = ( JCU_INT_ERROR_RESTART_INTERVAL_DATA |
            JCU_INT_ERROR_SEGMENT_TOTAL_DATA | JCU_INT_ERROR_MCU_BLOCK_DATA )
} jcu_int_detail_error_t;


/***********************************************************************
* Type: jcu_int_detail_errors_t
*    Bit flags of <jcu_int_detail_error_t>
************************************************************************/
typedef bit_flags_fast32_t  jcu_int_detail_errors_t;


/***********************************************************************
* Enumeration: jcu_swap_t
*    SampleEnum
*
*    : JCU_SWAP_NONE                        - 01234567
*    : JCU_SWAP_BYTE                        - 10325476
*    : JCU_SWAP_WORD                        - 23016745
*    : JCU_SWAP_WORD_AND_BYTE               - 32107654
*    : JCU_SWAP_LONG_WORD                   - 45670123
*    : JCU_SWAP_LONG_WORD_AND_BYTE          - 54761032
*    : JCU_SWAP_LONG_WORD_AND_WORD          - 67452301
*    : JCU_SWAP_LONG_WORD_AND_WORD_AND_BYTE - 76543210
************************************************************************/
typedef enum
{
    JCU_SWAP_NONE                        = 0x00,
    JCU_SWAP_BYTE                        = 0x01,
    JCU_SWAP_WORD                        = 0x02,
    JCU_SWAP_WORD_AND_BYTE               = 0x03,
    JCU_SWAP_LONG_WORD                   = 0x04,
    JCU_SWAP_LONG_WORD_AND_BYTE          = 0x05,
    JCU_SWAP_LONG_WORD_AND_WORD          = 0x06,
    JCU_SWAP_LONG_WORD_AND_WORD_AND_BYTE = 0x07
} jcu_swap_t;


/***********************************************************************
* Structure: jcu_buffer_t
*    Buffer settings
************************************************************************/
typedef struct
{
    jcu_swap_t    swapSetting;
    uint32_t     *address;
} jcu_buffer_t;


/***********************************************************************
* Structure: jcu_buffer_param_t
*    Buffer parameter for input and output
************************************************************************/
typedef struct
{

    /* Variable: source */
    jcu_buffer_t  source;

    /* Variable: destination */
    jcu_buffer_t  destination;

    /* Variable: lineOffset
    *    Count of pixels in one line */
    int16_t       lineOffset;
} jcu_buffer_param_t;


/* Section: Global */
/***********************************************************************
* Enumeration: jcu_sub_sampling_t
*    Sub sampling settings for decoding
*
*    : JCU_SUB_SAMPLING_1_1 - 0x00 = 1/1
*    : JCU_SUB_SAMPLING_1_2 - 0x01 = 1/2
*    : JCU_SUB_SAMPLING_1_4 - 0x02 = 1/4
*    : JCU_SUB_SAMPLING_1_8 - 0x03 = 1/8
************************************************************************/
typedef enum
{
    JCU_SUB_SAMPLING_1_1 = 0x00,
    JCU_SUB_SAMPLING_1_2 = 0x01,
    JCU_SUB_SAMPLING_1_4 = 0x02,
    JCU_SUB_SAMPLING_1_8 = 0x03
} jcu_sub_sampling_t;


/***********************************************************************
* Enumeration: jcu_decode_format_t
*    Kind of pixel format can process the decoding
*
*    : JCU_OUTPUT_YCbCr422 - 0x00
*    : JCU_OUTPUT_ARGB8888 - 0x01
*    : JCU_OUTPUT_RGB565   - 0x02
************************************************************************/
typedef enum
{
    JCU_OUTPUT_YCbCr422 = 0x00,
    JCU_OUTPUT_ARGB8888 = 0x01,
    JCU_OUTPUT_RGB565   = 0x02
} jcu_decode_format_t;


/***********************************************************************
* Enumeration: jcu_cbcr_offset_t
*    Cb/Cr offset
*
*    : JCU_CBCR_OFFSET_0   - 0 = No offset
*    : JCU_CBCR_OFFSET_128 - 1 = +128 offset
************************************************************************/
typedef enum
{
    JCU_CBCR_OFFSET_0 = 0,
    JCU_CBCR_OFFSET_128 = 1
} jcu_cbcr_offset_t;


/***********************************************************************
* Enumeration: jcu_jpeg_format_t
*    Kind of pixel format for the jpeg file format when decoding
*
*    : JCU_JPEG_YCbCr444 - 0
*    : JCU_JPEG_YCbCr422 - 1
*    : JCU_JPEG_YCbCr420 - 2
*    : JCU_JPEG_YCbCr411 - 6
************************************************************************/
typedef enum
{
    JCU_JPEG_YCbCr444   = 0x00,
    JCU_JPEG_YCbCr422   = 0x01,
    JCU_JPEG_YCbCr420   = 0x02,
    JCU_JPEG_YCbCr411   = 0x06
} jcu_jpeg_format_t;


/***********************************************************************
* Structure: jcu_image_info_t
*    Image information data store variable
************************************************************************/
typedef struct
{
    uint32_t            width;
    uint32_t            height;
    jcu_jpeg_format_t   encodedFormat;
} jcu_image_info_t;


/* Section: Global */
/***********************************************************************
* Enumeration: jcu_table_no_t
*    The index of the table
*
*    : JCU_TABLE_NO_0 - 0
*    : JCU_TABLE_NO_1 - 1
*    : JCU_TABLE_NO_2 - 2
*    : JCU_TABLE_NO_3 - 3
************************************************************************/
typedef enum
{
    JCU_TABLE_NO_0 = 0,
    JCU_TABLE_NO_1 = 1,
    JCU_TABLE_NO_2 = 2,
    JCU_TABLE_NO_3 = 3
} jcu_table_no_t;


/***********************************************************************
* Enumeration: jcu_huff_t
*    Kind of Huffman table
*
*    : JCU_HUFFMAN_AC - 0
*    : JCU_HUFFMAN_DC - 1
************************************************************************/
typedef enum
{
    JCU_HUFFMAN_AC,
    JCU_HUFFMAN_DC
} jcu_huff_t;


/***********************************************************************
* Enumeration: jcu_color_element_t
*    Kind of color data for using the encoder parameter
*
*    : JCU_ELEMENT_Y  - 0
*    : JCU_ELEMENT_Cb - 1
*    : JCU_ELEMENT_Cr - 2
************************************************************************/
typedef enum
{
    JCU_ELEMENT_Y,
    JCU_ELEMENT_Cb,
    JCU_ELEMENT_Cr
} jcu_color_element_t;


/***********************************************************************
* Structure: jcu_decode_param_t
*    Setting parameter for the Decoding
************************************************************************/
typedef struct
{
    jcu_sub_sampling_t  verticalSubSampling;
    jcu_sub_sampling_t  horizontalSubSampling;
    jcu_decode_format_t decodeFormat;
    jcu_cbcr_offset_t   outputCbCrOffset;
    uint8_t             alpha;
} jcu_decode_param_t;


/***********************************************************************
* Structure: jcu_encode_param_t
*    Setting parameter for the Encoding
************************************************************************/
typedef struct
{
    jcu_jpeg_format_t   encodeFormat;
    int32_t             QuantizationTable[JCU_COLOR_ELEMENT_NUM];
    int32_t             HuffmanTable[JCU_COLOR_ELEMENT_NUM];
    uint32_t            DRI_value;
    uint32_t            width;
    uint32_t            height;
#ifndef _SH
    jcu_cbcr_offset_t   inputCbCrOffset;
#endif
} jcu_encode_param_t;


/* Section: Global */
/***********************************************************************
* Enumeration: jcu_codec_status_t
*    Codec type
*
*    : JCU_CODEC_NOT_SELECTED - -1
*    : JCU_STATUS_ENCODE      - 0
*    : JCU_STATUS_DECODE      - 1
************************************************************************/
typedef enum
{
    JCU_CODEC_NOT_SELECTED  = -1,
    JCU_STATUS_ENCODE       = 0,
    JCU_STATUS_DECODE       = 1
} jcu_codec_status_t;


/***********************************************************************
* Structure: jcu_config_t
*
* Example:
*    jcu_config_t  config = {0};
************************************************************************/
typedef struct st_jcu_config_t
{
    int32_t  reserved;
} jcu_config_t;


/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

#endif /* #define  R_JCU_API_TYPEDEF_H */
