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
 * @headerfile     r_adc.h
 * @brief          Simple interface for ADC AN0 connected to board
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef R_SW_PKG_93_ADC_API_H_INCLUDED
#define R_SW_PKG_93_ADC_API_H_INCLUDED
/**************************************************************************//**
 * @ingroup R_SW_PKG_93_NONOS_DRIVERS
 * @defgroup R_SW_PKG_93_ADC_API ADC RZA1H Driver
 *
 * @anchor R_SW_PKG_93_ADC_API_SUMMARY
 * @par Summary
 *
 * This is a simple ADC Low Level Driver developed for the RZA1H
 * board.
 *
 * This is a device and board specific implementation of an ADC driver,
 * configured for the RZA1H board.
 *
 * @anchor R_SW_PKG_93_ADC_API_INSTANCES
 * @par Known Implementations:
 * This driver is used in the RZA1H Software Package.
 * @see RENESAS_APPLICATION_SOFTWARE_PACKAGE
 *
 * @anchor R_SW_PKG_93_ADC_API_SEE_ALSO

 * @see RENESAS_OS_ABSTRACTION  Renesas OS Abstraction interface
 * @{
 *****************************************************************************/

/* ADC Trigger setup parameter */
typedef enum
{
    ADC_NO_TRIGGER = 0, /*!< Started by external trigger input is disabled */
    ADC_TRGAN =      1, /*!< Started by conversion trigger TRGAN from the multi-function timer pulse unit 2 */
    ADC_TRG0N =      2, /*!< Started by conversion trigger TRG0N from the multi-function timer pulse unit 2 */
    ADC_TRG4AN =     3, /*!< Started by conversion trigger TRG4AN from the multi-function timer pulse unit 2 */
    ADC_TRG4BN =     4, /*!< Started by conversion trigger TRG4BN from the multi-function timer pulse unit 2 */
    ADC_ADTRG =      9  /*!< Started by ADTRG */
} r_adc_trgs;

/* ADC conversion accuracy setup parameter */
typedef enum
{
    ADC_256TCYC =  0,  /*!< Conversion time = 256 tcyc (maximum)*/
    ADC_298TCYC =  1,  /*!< Conversion time = 298 tcyc (maximum)*/
    ADC_340TCYC =  2,  /*!< Conversion time = 340 tcyc (maximum)*/
    ADC_382TCYC =  3   /*!< Conversion time = 382 tcyc (maximum)*/
} r_adc_cks;

/* ADC mode setup parameter */
typedef enum
{
    ADC_SINGLE =    0,  /*!< SINGLE scan mode */
    ADC_MULTI_1_4 = 4,  /*!< Multi-scan mode 1 to 4 channels */
    ADC_MULTI_1_8 = 5   /*!< Multi-scan mode 1 to 8 channels */
} r_adc_mds;

/* ADC channels supported */
typedef enum
{
    R_ADC_CH0 = 0,  /*!< ADC Channel 0 (AN0) */
    R_ADC_CH1,      /*!< ADC Channel 1 (AN1) */
    R_ADC_CH2,      /*!< ADC Channel 2 (AN2) */
    R_ADC_CH3,      /*!< ADC Channel 3 (AN3) */
    R_ADC_CH4,      /*!< ADC Channel 4 (AN4) */
    R_ADC_CH5,      /*!< ADC Channel 5 (AN5) */
    R_ADC_CH6,      /*!< ADC Channel 6 (AN6) */
    R_ADC_CH7       /*!< ADC Channel 7 (AN7) */
}   r_adc_ch;

/* ADC setup parameter */
typedef struct
{
    r_adc_trgs trgs; /*!< ADC Trigger setup parameter  */
    r_adc_cks  cks;  /*!< ADC conversion accuracy setup parameter */
    r_adc_mds  mds;  /*!< ADC mode setup parameter */
    r_adc_ch   ch;   /*!< ADC channels supported */
}r_adc_cfg;

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

/**
 * @brief Enables and configures ADC.
 * @param r_adc_cfg ADC parameters (see r_adc_cfg)
 * @return None.
 */
extern void R_ADC_Open(r_adc_cfg* r_adc_param);

/**
 * @brief Disables the ADC
 * @param ch ADC channel desired (see r_adc_ch)
 * @return None.
 */
extern void R_ADC_Close (r_adc_ch ch);

/**
 * @brief Read value from ADC
 * @param p_adc_data pointer to buffer to store the ADC result
 * @param adc_ch_begin ADC channel desired (see r_adc_ch)
 * @param adc_ch_amount the numner of ADC channel desired
 * @return None.
 */
extern void R_ADC_Read (uint16_t p_adc_data[], r_adc_ch adc_ch_begin, uint8_t adc_ch_amount);

/* ADC_H*/
#endif
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/* End of File */

