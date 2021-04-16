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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/***********************************************************************
* File: rz_co.h
*    Common Functions.
*
* - $Rev: $
* - $Date: 2018-02-16T11:36:20+09:00 $
************************************************************************/

#ifndef RZ_CO_H
#define RZ_CO_H


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include  "r_typedefs.h"
#include  "rz_co_typedef.h"

/* INTC related headers */
#include  "dev_drv.h"
#include  "r_intc.h"

/* Register access headers */
#include  "rza_io_regrw.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Function: IS_BIT_SET
*    Evaluate whether any passed bits are 1 or not (bit_flags_fast32_t)
*
* Arguments:
*    in_Variable   - The value of target bit flags
*    in_ConstValue - The value that investigating bits are 1
*
* Return Value:
*    Whether the any passed bit are 1
************************************************************************/
/* ->MISRA 19.7 : For return _Bool type */ /* ->SEC M5.1.3 */

#define  IS_BIT_SET( in_Variable, in_ConstValue ) \
    ( ( (in_Variable) & (in_ConstValue) ) != 0u )

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */


/***********************************************************************
* Function: IS_ANY_BITS_SET
*    Evaluate whether any passed bits are 1 or not
*
* Arguments:
*    in_Variable     - The value of target bit flags
*    in_OrConstValue - The value that investigating bits are 1
*
* Return Value:
*    Whether the any passed bit are 1
************************************************************************/
/* ->MISRA 19.7 : For return _Bool type */ /* ->SEC M5.1.3 */

#define  IS_ANY_BITS_SET( in_Variable, in_OrConstValue ) \
    ( ( (in_Variable) & (in_OrConstValue) ) != 0u )

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */


/***********************************************************************
* Function: IS_ALL_BITS_SET
*    Evaluate whether all passed bits are 1 or not
*
* Arguments:
*    in_Variable     - The value of target bit flags
*    in_OrConstValue - The value that investigating bits are 1
*
* Return Value:
*    Whether the all passed bit are 1
************************************************************************/
/* ->MISRA 19.7 : For return _Bool type */ /* ->SEC M5.1.3 */

#define  IS_ALL_BITS_SET( in_Variable, in_OrConstValue ) \
    ( ( (in_Variable) & (in_OrConstValue) ) == (in_OrConstValue) )

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */


/***********************************************************************
* Function: IS_BIT_NOT_SET
*    Evaluate whether the passed bit is 0 or not
*
* Arguments:
*    in_Variable   - The value of target bit flags
*    in_ConstValue - The value that investigating bit is 1
*
* Return Value:
*    Whether the passed bit is 0
************************************************************************/
/* ->MISRA 19.7 : For return _Bool type */ /* ->SEC M5.1.3 */

#define  IS_BIT_NOT_SET( in_Variable, in_ConstValue ) \
    ( ( (in_Variable) & (in_ConstValue) ) == 0u )

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */


/***********************************************************************
* Function: IS_ANY_BITS_NOT_SET
*    Evaluate whether any passed bits are 0 or not
*
* Arguments:
*    in_Variable     - The value of target bit flags
*    in_OrConstValue - The value that investigating bits are 1
*
* Return Value:
*    Whether the any passed bit are 0
************************************************************************/
/* ->MISRA 19.7 : For return _Bool type */ /* ->SEC M5.1.3 */

#define  IS_ANY_BITS_NOT_SET( in_Variable, in_OrConstValue ) \
    ( ( (in_Variable) & (in_OrConstValue) ) != (in_OrConstValue) )

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */


/***********************************************************************
* Function: IS_ALL_BITS_NOT_SET
*    Evaluate whether all passed bits are 0 or not
*
* Arguments:
*    in_Variable     - The value of target bit flags
*    in_OrConstValue - The value that investigating bits are 1
*
* Return Value:
*    Whether the all passed bit are 0
************************************************************************/
/* ->MISRA 19.7 : For return _Bool type */ /* ->SEC M5.1.3 */

#define  IS_ALL_BITS_NOT_SET( in_Variable, in_OrConstValue ) \
    ( ( (in_Variable) & (in_OrConstValue) ) == 0u )

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */


/***********************************************************************
* Macro: R_CO_REGISTER32
*
* Example:
*    R_CO_REGISTER32( UNIT.REG_A ) = 2;
*    uint32_t  value = R_CO_REGISTER32( UNIT.REG_A );
*    uint32_t*  address_of_value = &R_CO_REGISTER32( UNIT.REG_A );
************************************************************************/
#define  R_CO_REGISTER32( in_Register ) \
    in_Register
/* Without .LONG */


/***********************************************************************
* Macro: R_CO_REGISTER16
*
* Example:
*    R_CO_REGISTER16( UNIT.REG_A ) = 2;
*    uint16_t  value = R_CO_REGISTER16( UNIT.REG_A );
*    uint16_t*  address_of_value = &R_CO_REGISTER16( UNIT.REG_A );
************************************************************************/
#define  R_CO_REGISTER16( in_Register ) \
    in_Register
/* Without .WORD */


/***********************************************************************
* Macro: R_CO_REGISTER8
*
* Example:
*    R_CO_REGISTER8( UNIT.REG_A ) = 2;
*    uint8_t  value = R_CO_REGISTER8( UNIT.REG_A );
*    uint8_t*  address_of_value = &R_CO_REGISTER8( UNIT.REG_A );
************************************************************************/
#define  R_CO_REGISTER8( in_Register ) \
    in_Register
/* Without .BYTE */


/***********************************************************************
* Macro: R_CO_SET_BIT_IN_REGISTER32
*
* Arguments:
*    in_UnitName          - e.g.) CPG
*    in_RegisterName      - e.g.) STBCR3
*    in_BitName           - e.g.) MSTP36
*    in_Value             - uint_fast32_t type. A writing value
*
* Example:
*    R_CO_SET_BIT_IN_REGISTER32( CPG,STBCR3,MSTP36, 0 );
*        // This macro refers the following macros:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
************************************************************************/
#define  R_CO_SET_BIT_IN_REGISTER32( \
        in_UnitName,  in_RegisterName,  in_BitName,  in_Value ) \
\
    rza_io_reg_write_32( \
        &in_UnitName.in_RegisterName, \
        in_Value, \
        in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT, \
        in_UnitName##_##in_RegisterName##_##in_BitName )


/***********************************************************************
* Macro: R_CO_SET_BIT_IN_REGISTER16
*
* Arguments:
*    in_UnitName          - e.g.) CPG
*    in_RegisterName      - e.g.) STBCR3
*    in_BitName           - e.g.) MSTP36
*    in_Value             - uint_fast32_t type. A writing value
*
* Example:
*    R_CO_SET_BIT_IN_REGISTER16( CPG,STBCR3,MSTP36, 0 );
*        // This macro refers the following macros:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
************************************************************************/
#define  R_CO_SET_BIT_IN_REGISTER16( \
        in_UnitName,  in_RegisterName,  in_BitName,  in_Value ) \
\
    rza_io_reg_write_16( \
        &in_UnitName.in_RegisterName, \
        in_Value, \
        in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT, \
        in_UnitName##_##in_RegisterName##_##in_BitName )


/***********************************************************************
* Macro: R_CO_SET_BIT_IN_REGISTER8
*
* Arguments:
*    in_UnitName          - e.g.) CPG
*    in_RegisterName      - e.g.) STBCR3
*    in_BitName           - e.g.) MSTP36
*    in_Value             - uint_fast32_t type. A writing value
*
* Example:
*    R_CO_SET_BIT_IN_REGISTER8( CPG,STBCR3,MSTP36, 0 );
*        // This macro refers the following macros:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
************************************************************************/
#define  R_CO_SET_BIT_IN_REGISTER8( \
        in_UnitName,  in_RegisterName,  in_BitName,  in_Value ) \
\
    rza_io_reg_write_8( \
        &in_UnitName.in_RegisterName, \
        in_Value, \
        in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT, \
        in_UnitName##_##in_RegisterName##_##in_BitName )


/***********************************************************************
* Macro: R_CO_GET_BIT_IN_REGISTER32
*
* Arguments:
*    in_UnitName          - e.g.) CPG
*    in_RegisterName      - e.g.) STBCR3
*    in_BitName           - e.g.) MSTP36
*    in_Value             - uint_fast32_t type. A writing value
*
* Return Value:
*    A read value after shift. uint32_t type.
*
* Example:
*    value = R_CO_GET_BIT_IN_REGISTER32( CPG,STBCR3,MSTP36 );
*        // This macro refers the following macros:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
************************************************************************/
#define  R_CO_GET_BIT_IN_REGISTER32( \
        in_UnitName,  in_RegisterName,  in_BitName ) \
\
    rza_io_reg_read_32( \
        &in_UnitName.in_RegisterName, \
        in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT, \
        in_UnitName##_##in_RegisterName##_##in_BitName )


/***********************************************************************
* Macro: R_CO_GET_BIT_IN_REGISTER16
*
* Arguments:
*    in_UnitName          - e.g.) CPG
*    in_RegisterName      - e.g.) STBCR3
*    in_BitName           - e.g.) MSTP36
*    in_Value             - uint_fast32_t type. A writing value
*
* Return Value:
*    A read value after shift. uint16_t type.
*
* Example:
*    value = R_CO_GET_BIT_IN_REGISTER16( CPG,STBCR3,MSTP36 );
*        // This macro refers the following macros:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
************************************************************************/
#define  R_CO_GET_BIT_IN_REGISTER16( \
        in_UnitName,  in_RegisterName,  in_BitName ) \
\
    rza_io_reg_read_16( \
        &in_UnitName.in_RegisterName, \
        in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT, \
        in_UnitName##_##in_RegisterName##_##in_BitName )


/***********************************************************************
* Macro: R_CO_GET_BIT_IN_REGISTER8
*
* Arguments:
*    in_UnitName          - e.g.) CPG
*    in_RegisterName      - e.g.) STBCR3
*    in_BitName           - e.g.) MSTP36
*    in_Value             - uint_fast32_t type. A writing value
*
* Return Value:
*    A read value after shift. uint8_t type.
*
* Example:
*    value = R_CO_GET_BIT_IN_REGISTER8( CPG,STBCR3,MSTP36 );
*        // This macro refers the following macros:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
************************************************************************/
#define  R_CO_GET_BIT_IN_REGISTER8( \
        in_UnitName,  in_RegisterName,  in_BitName ) \
\
    rza_io_reg_read_8( \
        &in_UnitName.in_RegisterName, \
        in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT, \
        in_UnitName##_##in_RegisterName##_##in_BitName )


/***********************************************************************
* Macro: R_CO_SET_BIT_IN_UINT32
*
* Arguments:
*    in_AddressOfRegister - uint32_t*
*    in_UnitName          - e.g.) CPG
*    in_RegisterName      - e.g.) STBCR3
*    in_BitName           - e.g.) MSTP36
*    in_Value             - A writing value
*
* Example:
*    R_CO_SET_BIT_IN_UINT32( &value,  CPG,STBCR3,MSTP36,  0 );
*        // This macro refers:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
*
* Imeplement:
*    > *in_out_Value =
*    >     ( *in_out_Value & ~ Mask ) | ( in_Value << Shift & Mask );
************************************************************************/

#define  R_CO_SET_BIT_IN_UINT32( in_out_Value, \
            in_UnitName,  in_RegisterName,  in_BitName,  in_Value ) \
\
    ( *(in_out_Value) = (uint32_t)( \
        ( \
            ( *(in_out_Value) ) & ~( \
                in_UnitName##_##in_RegisterName##_##in_BitName \
            ) \
        ) | ( \
            ( \
                ( in_Value ) << ( \
                    in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT \
                ) \
            ) & ( \
                in_UnitName##_##in_RegisterName##_##in_BitName \
            ) \
        ) \
    ) )


/***********************************************************************
* Macro: R_CO_SET_BIT_IN_UINT16
*
* Arguments:
*    in_AddressOfRegister - uint16_t*
*    in_UnitName          - e.g.) CPG
*    in_RegisterName      - e.g.) STBCR3
*    in_BitName           - e.g.) MSTP36
*    in_Value             - A writing value
*
* Example:
*    R_CO_SET_BIT_IN_UINT16( &value,  CPG,STBCR3,MSTP36,  0 );
*        // This macro refers:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
*
* Imeplement:
*    > *in_out_Value =
*    >     ( *in_out_Value & ~ Mask ) | ( in_Value << Shift & Mask );
************************************************************************/

#define  R_CO_SET_BIT_IN_UINT16( in_out_Value, \
            in_UnitName,  in_RegisterName,  in_BitName,  in_Value ) \
\
    ( *(in_out_Value) = (uint16_t)( \
        ( \
            ( *(in_out_Value) ) & ~( \
                in_UnitName##_##in_RegisterName##_##in_BitName \
            ) \
        ) | ( \
            ( \
                ( in_Value ) << ( \
                    in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT \
                ) \
            ) & ( \
                in_UnitName##_##in_RegisterName##_##in_BitName \
            ) \
        ) \
    ) )


/***********************************************************************
* Macro: R_CO_SET_BIT_IN_UINT8
*
* Arguments:
*    in_AddressOfRegister - uint8_t*
*    in_UnitName          - e.g.) CPG
*    in_RegisterName      - e.g.) STBCR3
*    in_BitName           - e.g.) MSTP36
*    in_Value             - A writing value
*
* Example:
*    R_CO_SET_BIT_IN_UINT8( &value,  CPG,STBCR3,MSTP36,  0 );
*        // This macro refers:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
*
* Imeplement:
*    > *in_out_Value =
*    >     ( *in_out_Value & ~ Mask ) | ( in_Value << Shift & Mask );
************************************************************************/

#define  R_CO_SET_BIT_IN_UINT8( in_out_Value, \
            in_UnitName,  in_RegisterName,  in_BitName,  in_Value ) \
\
    ( *(in_out_Value) = (uint8_t)( \
        ( \
            ( *(in_out_Value) ) & ~( \
                in_UnitName##_##in_RegisterName##_##in_BitName \
            ) \
        ) | ( \
            ( \
                ( in_Value ) << ( \
                    in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT \
                ) \
            ) & ( \
                in_UnitName##_##in_RegisterName##_##in_BitName \
            ) \
        ) \
    ) )


/***********************************************************************
* Macro: R_CO_GET_BIT_IN_UINT32
*
* Arguments:
*    in_Value        - uint32_t
*    in_UnitName     - e.g.) CPG
*    in_RegisterName - e.g.) STBCR3
*    in_BitName      - e.g.) MSTP36
*
* Return Value:
*    A read value after shift. uint_fast32_t type.
*
* Example:
*    value = R_CO_GET_BIT_IN_UINT32( CPG.STBCR3,  CPG,STBCR3,MSTP36 );
*        // This macro refers:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
*
* Imeplement:
*    > return  ( in_Value & Mask ) >> Shift;
************************************************************************/

#define  R_CO_GET_BIT_IN_UINT32( in_Value, \
            in_UnitName,  in_RegisterName,  in_BitName ) \
\
    ( (uint_fast32_t)( \
        ( \
            (in_Value) & ( \
                in_UnitName##_##in_RegisterName##_##in_BitName \
            ) \
        ) >> ( \
            in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT \
        ) \
    ) )


/***********************************************************************
* Macro: R_CO_GET_BIT_IN_UINT16
*
* Arguments:
*    in_Value         - uint16_t
*    in_UnitName      - e.g.) CPG
*    in_RegisterName  - e.g.) STBCR3
*    in_BitName       - e.g.) MSTP36
*
* Return Value:
*    A read value after shift. uint_fast16_t type.
*
* Example:
*    value = R_CO_GET_BIT_IN_UINT16( CPG.STBCR3,  CPG,STBCR3,MSTP36 );
*        // This macro refers:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
*
* Imeplement:
*    > return  ( in_Value & Mask ) >> Shift;
************************************************************************/

#define  R_CO_GET_BIT_IN_UINT16( in_Value, \
            in_UnitName,  in_RegisterName,  in_BitName ) \
\
    ( (uint_fast16_t)( \
        ( \
            (in_Value) & ( \
                in_UnitName##_##in_RegisterName##_##in_BitName \
            ) \
        ) >> ( \
            in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT \
        ) \
    ) )


/***********************************************************************
* Macro: R_CO_GET_BIT_IN_UINT8
*
* Arguments:
*    in_Value         - uint8_t
*    in_UnitName      - e.g.) CPG
*    in_RegisterName  - e.g.) STBCR3
*    in_BitName       - e.g.) MSTP36
*
* Return Value:
*    A read value after shift. uint_fast8_t type.
*
* Example:
*    value = R_CO_GET_BIT_IN_UINT8( CPG.STBCR3,  CPG,STBCR3,MSTP36 );
*        // This macro refers:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
*
* Imeplement:
*    > return  ( in_Value & Mask ) >> Shift;
************************************************************************/

#define  R_CO_GET_BIT_IN_UINT8( in_Value, \
            in_UnitName,  in_RegisterName,  in_BitName ) \
\
    ( (uint_fast8_t)( \
        ( \
            (in_Value) & ( \
                in_UnitName##_##in_RegisterName##_##in_BitName \
            ) \
        ) >> ( \
            in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT \
        ) \
    ) )


/***********************************************************************
* Macro: R_CO_IS_U_BIT_OVERFLOW
*
* Arguments:
*    in_UnitName      - e.g.) CPG
*    in_RegisterName  - e.g.) STBCR3
*    in_BitName       - e.g.) MSTP36
*    in_Value         - A writing value
*
* Return Value:
*    Whether "in_Value" is overflow from the bits of "in_BitName".
*
* Example:
*    if ( R_CO_IS_U_BIT_OVERFLOW( CPG,STBCR3,MSTP36,  in_Argument ) ) { error(); }
*        // This macro refers:
*        //     CPG_STBCR3_MSTP36
*        //     CPG_STBCR3_MSTP36_SHIFT
*
* Imeplement:
*    > ( in_Value & ~ ( Mask >> Shift ) ) != 0
************************************************************************/

#define  R_CO_IS_U_BIT_OVERFLOW( \
            in_UnitName,  in_RegisterName,  in_BitName,  in_Value ) \
\
    ( ( ( in_Value ) & ~( \
        ( \
            in_UnitName##_##in_RegisterName##_##in_BitName \
        ) >> ( \
            in_UnitName##_##in_RegisterName##_##in_BitName##_SHIFT \
        ) \
    ) ) != 0 )


/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Function: R_CO_SetErrNum
*    Sets error code to current thread.
*
* Arguments:
*    e - Error code
*
* Return Value:
*    None.
*
* Description:
*    If "e" argument is 0, this function do nothing.
*
*    If you set break point at this function, you can see call tree
*    at the place of raising an error.
************************************************************************/
void  R_CO_SetErrNum( errnum_t e );


/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Function: R_CO_SetTrue
*    Sets the event. OS-less boolean version.
*
* Arguments:
*    ref_Event - Event to set
*
* Return Value:
*    None
*
* Example:
*    >    volatile bool_t  is_event;
*    >        :
*    >    is_event = false;
*    >    R_DRIVER_TransferStart( (r_co_function_t) R_CO_SetTrue,  &is_event );
*    >    while ( ! is_event )
*    >        { }  // Pooling
************************************************************************/
void  R_CO_SetTrue( volatile bool_t*  ref_Event );


/******************************************************************************
Functions Prototypes
******************************************************************************/

/***********************************************************************
* Function: R_COUNT_OF
*    Returns element count of the array
*
* Arguments:
*    Array - An array
*
* Return Value:
*    Count of specified array's element
*
* Description:
*    Compatible with _countof (MS C++) and ARRAY_SIZE (Linux).
*    But our naming rule is not match.
*
* Example:
*    > uint32_t  array[10];
*    > R_COUNT_OF( array )  // = 10
*
* Example:
*    Array argument must not be specified the pointer using like array.
*    > uint32_t   array[10];
*    > func( array );
*    >
*    > void  func( uint32_t array[] )  // "array" is a pointer
*    > {
*    >     R_COUNT_OF( array )  // NG
*    > }
************************************************************************/
/* ->MISRA 19.7 : Cannot function */ /* ->SEC M5.1.3 */

#define  R_COUNT_OF( Array )  ( sizeof( Array ) / sizeof( *(Array) ) )

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */


/***********************************************************************
* Macro: R_UNREFERENCED_VARIABLE
*    Assertion that specified variable is not referenced
*
* Description:
*    This avoids warning of "set but never used".
************************************************************************/
/* ->MISRA 19.7 : Argument must be any type */ /* ->SEC M5.1.3 */

#define  R_UNREFERENCED_VARIABLE( a1 ) \
    ((void)(a1))

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */
/* ->QAC 3206 : Not used argument */


/***********************************************************************
* Macro: R_UNREFERENCED_VARIABLE_2
*    Assertion that specified variable is not referenced
*
* Description:
*    This avoids warning of "set but never used".
************************************************************************/
/* ->MISRA 19.7 : Argument must be any type */ /* ->SEC M5.1.3 */

#define  R_UNREFERENCED_VARIABLE_2( a1, a2 ) \
    ((void)(a1),  (void)(a2))

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */
/* ->QAC 3206 : Not used argument */


/***********************************************************************
* Macro: R_UNREFERENCED_VARIABLE_3
*    Assertion that specified variable is not referenced
*
* Description:
*    This avoids warning of "set but never used".
************************************************************************/
/* ->MISRA 19.7 : Argument must be any type */ /* ->SEC M5.1.3 */

#define  R_UNREFERENCED_VARIABLE_3( a1, a2, a3 ) \
    ((void)(a1),  (void)(a2),  (void)(a3))

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */
/* ->QAC 3206 : Not used argument */


/***********************************************************************
* Macro: R_UNREFERENCED_VARIABLE_4
*    Assertion that specified variable is not referenced
*
* Description:
*    This avoids warning of "set but never used".
************************************************************************/
/* ->MISRA 19.7 : Argument must be any type */ /* ->SEC M5.1.3 */

#define  R_UNREFERENCED_VARIABLE_4( a1, a2, a3, a4 ) \
    ((void)(a1),  (void)(a2),  (void)(a3),  (void)(a4))

/* <-MISRA 19.7 */ /* <-SEC M5.1.3 */
/* ->QAC 3206 : Not used argument */


/***********************************************************************
* Macro: STATIC_INLINE
*    Static inline in C source file
************************************************************************/
#ifndef  __cplusplus

#ifdef __CC_ARM
#define  STATIC_INLINE  static __inline
#endif

#ifdef __ICCARM__
#define  STATIC_INLINE  static inline
#endif

#if  defined( __GNUC__ )  &&  ! defined( __CC_ARM )
#define  STATIC_INLINE  static inline
#endif

#ifdef _SH
#define  STATIC_INLINE  static
#endif

#else

#define  STATIC_INLINE  static inline

#endif  /* __cplusplus */


/***********************************************************************
* End of File:
************************************************************************/
#ifdef __cplusplus
}  /* extern "C" */
#endif /* __cplusplus */

#endif /* RZ_CO_H */
