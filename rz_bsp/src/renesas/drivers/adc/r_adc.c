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
/*******************************************************************************
* File Name     : adc.c
* Device(s)     : RZ/A1H (R7S721001)
* Tool-Chain    : GNUARM-NONEv14.02-EABI
* H/W Platform  : RSK+RZA1H CPU Board
* Description   : Sample Program - ADC channel A7 user code
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 21.10.2014 1.00
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>

#include "iodefine_cfg.h"
#include "compiler_settings.h"

/* Device driver header */
#include "dev_drv.h"
/* INTC Driver Header */
#include "r_intc.h"
/* Low level register read/write header */
#include "rza_io_regrw.h"
/* Interchangeable compiler specific header */
#include "compiler_settings.h"

#include "r_adc.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define ADC_DELAY       (10000)
#define ADC_PRI_ADI     (5uL)      /* ADI interrupt priority                 */

#define ADCSR_SET       (0x4000u)  /* ADCSR setting value                    */
#define STBCR6_7_SET    (0x80uL)   /* STBCR6  setting value                  */
#define PMC1_8_SET      (0x0100uL) /* P1 PMC  setting value                  */
#define PFCAE1_8_SET    (0xFEFFuL) /* P1 PFCAE  setting value                */
#define PFCE1_8_SET     (0xFEFFuL) /* P1 PFCE  setting value                 */
#define PFC1_8_SET      (0xFEFFuL) /* P1 PFC  setting value                  */
#define PM1_8_SET       (0x0100uL) /* P1 PM  setting value                   */
#define ADST_ON         (0x2000uL) /* ADC start                              */
#define ADST_OFF        (0xDFFFuL) /* ADC stop                               */
#define ADC_10BIT_SET   (0x03FFuL) /* 15-10bit 0 write ADCDRA                */
#define ADC_12BIT_SET   (0x0FFFuL) /* 15-12bit 0 write ADCDRA                */
#define ADC_BIT_SHIFT_6 (0x0006uL) /* 6bit right shift ADCDRA                */
#define ADC_BIT_SHIFT_4 (0x0004uL) /* 4bit right shift ADCDRA                */
#define ADIE_ON         (0x4000uL) /* A/D conversion end interrupt enble     */
#define ADIE_OFF        (0xBFFFuL) /* A/D conversion end interrupt disabled  */
#define ADF_CLEAR       (0x7FFFuL) /* A/D conversion end interrupt ADF clear */
#define ADC_SET_FLG     (1u)       /* A/D conversion end interrupt ADF set   */
#define ADC_READ_ERR    (0xFFFFuL) /* Timeout error                          */
#define ADC_SR_SHIFT_ADST   (13u)  /* ADCSR shift value for ADST bits        */
#define ADC_SR_SHIFT_TRGS   (9u)   /* ADCSR shift value for TRGS bits        */
#define ADC_SR_SHIFT_CKS    (6u)   /* ADCSR shift value for CKS  bits        */
#define ADC_SR_SHIFT_MDS    (3u)   /* ADCSR shift value for MDS  bits        */
#define ADC_SR_SHIFT_CH     (0u)   /* ADCSR shift value for CH   bits        */
#define ADC_CHANNELS        (8u)

#define DELAY_TIME      (2000000uL)/* Sampling loop                          */
#define TIME_OUT        (2000000uL)/* Timeout loop                           */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

static void     adc_start (void);
static void     adc_stop (void);
static void     adc_read_single (uint16_t p_adc_data[], r_adc_ch adc_ch_begin, uint8_t adc_ch_amount);
static void     adc_initialise (r_adc_cfg* r_adc_param);
static void     adc_uninitialise (void);
static void     int_adc_end (uint32_t sense);
static bool_t   adc_in_use (r_adc_ch ch);
static void     set_ch_used (r_adc_ch ch);
static void     clear_ch_used (r_adc_ch ch);

static bool_t     s_configured = false;
static uint32_t   s_ch_in_use = 0;
static uint32_t   g_adc_result = 0;

/* Declare volatile to prevent it from being optimised by the compiler */
volatile uint8_t  g_adc_endflg = 0u;
volatile uint8_t  g_adc_complete_flag = 0u;

/******************************************************************************
* Function Name: adc_in_use
* Description  : check to see if requested channel is used in the project
* Arguments    : ch - ADC channel see r_adc_ch
* Return Value : true - channel is in use
*              : false - channel is not in use
******************************************************************************/
static bool_t adc_in_use (r_adc_ch ch)
{
    bool_t ch_in_use = false;

    if (s_ch_in_use << ch)
    {
        ch_in_use = true;
    }

    return (ch_in_use);
}
/******************************************************************************
 * End of Function adc_in_use
 ******************************************************************************/

/******************************************************************************
* Function Name: set_ch_used
* Description  : Sets requested channel in use in the project
*                ignored if channel is in use
* Arguments    : ch - ADC channel see r_adc_ch
* Return Value : none
******************************************************************************/
static void set_ch_used (r_adc_ch ch)
{
    if (adc_in_use(ch) == 0)
    {
        s_ch_in_use |= (uint32_t) (1 << ch);
    }
}
/******************************************************************************
 * End of Function set_ch_used
 ******************************************************************************/

/******************************************************************************
* Function Name: clear_ch_used
* Description  : Clears requested channel in use in the project
*                ignored if channel is not in use
* Arguments    : ch - ADC channel see r_adc_ch
* Return Value : none
******************************************************************************/
static void clear_ch_used (r_adc_ch ch)
{
    if (adc_in_use(ch))
    {
        s_ch_in_use &= (uint32_t) (~(1 << ch));
    }
}
/******************************************************************************
 * End of Function clear_ch_used
 ******************************************************************************/

/******************************************************************************
* Function Name: adc_initialise
* Description  : ADC Initialise (Single mode, Analog input AN7)
* Arguments    : ch - ADC channel see r_adc_ch
* Return Value : none
******************************************************************************/
static void adc_initialise (r_adc_cfg* r_adc_param)
{
    uint16_t adcsr_value = ADCSR_SET;
    bool_t abort = false;
    uint8_t adc_port = 0;

    /* Configure ADC (ADCSR_SET)
    Channel AN0, Single mode, AD conversion end interrupt enable,
    Conversion time =  256tcyc(10bit)
    ADCSR   - A/D  control/status register
    b15     - A/D end flag              - Write 0 to clear flag
    b14     - A/D interrupt enable      - AD conversion end interrupt enable
    b13     - A/D start                 - Stop
    b12-9   - Timer Trigger set         - Disables start A/D conversion by external trigger
    b8-6    - Clock Select              - Conversion time = 256tcyc(10bit)
    b5-3    - Multi-scan mode           - Single Mode
    b2-0    - Channel Select            - AN0-7 taken from ch above */
    adcsr_value = (uint16_t)(ADCSR_SET
                | (uint32_t)(r_adc_param->trgs << ADC_SR_SHIFT_TRGS)
                | (uint32_t)(r_adc_param->cks  << ADC_SR_SHIFT_CKS)
                | (uint32_t)(r_adc_param->mds  << ADC_SR_SHIFT_MDS)
                | (uint32_t)(r_adc_param->ch   << ADC_SR_SHIFT_CH));
    ADC.ADCSR = adcsr_value;

    switch (r_adc_param->mds)
    {
        case ADC_SINGLE:
        {
            adc_port = (uint8_t) (1 << r_adc_param->ch);
            break;
        }
        case ADC_MULTI_1_4:
        {
            uint8_t tmp_port;
            uint8_t tmp_port_loop;

            if (r_adc_param->ch <= R_ADC_CH3)
            {
                tmp_port_loop = r_adc_param->ch;
                adc_port = 0x01;
            }
            else
            {
                tmp_port_loop = (uint8_t) (r_adc_param->ch - R_ADC_CH4);
                adc_port = 0x10;
            }

            for (tmp_port = 0; tmp_port < tmp_port_loop; tmp_port++ )
            {
                adc_port |= (uint8_t) (adc_port << 1);
            }
            break;
        }
        case ADC_MULTI_1_8:
        {
            uint8_t tmp_port;
            uint8_t tmp_port_loop;

            tmp_port_loop = r_adc_param->ch;
            adc_port = 0x01;

            for (tmp_port = 0; tmp_port < tmp_port_loop; tmp_port++ )
            {
                adc_port |= (uint8_t) (adc_port << 1);
            }
            break;
        }
        default:
        {
            adc_port = 0;
            abort = true;
            break;
        }
    }

    if (adc_port & (0x01 << 0))
    {
        /* Configure choice of pin as AN0 */
        rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC18_SHIFT,  GPIO_PMC1_PMC18);
        rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE18_SHIFT,GPIO_PFCAE1_PFCAE18);
        rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE18_SHIFT, GPIO_PFCE1_PFCE18);
        rza_io_reg_write_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC18_SHIFT,  GPIO_PFC1_PFC18);
    }

    if (adc_port & (0x01 << 1))
    {
        /* Configure choice of pin as AN1 */
        rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC19_SHIFT,  GPIO_PMC1_PMC19);
        rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE19_SHIFT,GPIO_PFCAE1_PFCAE19);
        rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE19_SHIFT, GPIO_PFCE1_PFCE19);
        rza_io_reg_write_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC19_SHIFT,  GPIO_PFC1_PFC19);
    }

    if (adc_port & (0x01 << 2))
    {
        /* Configure P1_10 as AN2 */
        rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC110_SHIFT,  GPIO_PMC1_PMC110);
        rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE110_SHIFT,GPIO_PFCAE1_PFCAE110);
        rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE110_SHIFT, GPIO_PFCE1_PFCE110);
        rza_io_reg_write_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC110_SHIFT,  GPIO_PFC1_PFC110);
    }

    if (adc_port & (0x01 << 3))
    {
        /* Configure choice of pin as AN3 */
        rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC111_SHIFT,  GPIO_PMC1_PMC111);
        rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE111_SHIFT,GPIO_PFCAE1_PFCAE111);
        rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE111_SHIFT, GPIO_PFCE1_PFCE111);
        rza_io_reg_write_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC111_SHIFT,  GPIO_PFC1_PFC111);
    }

    if (adc_port & (0x01 << 4))
    {
        /* Configure choice of pin as AN4 */
        rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC112_SHIFT,  GPIO_PMC1_PMC112);
        rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE112_SHIFT,GPIO_PFCAE1_PFCAE112);
        rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE112_SHIFT, GPIO_PFCE1_PFCE112);
        rza_io_reg_write_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC112_SHIFT,  GPIO_PFC1_PFC112);
    }

    if (adc_port & (0x01 << 5))
    {
        /* Configure choice of pin as AN5 */
        rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC113_SHIFT,  GPIO_PMC1_PMC113);
        rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE113_SHIFT,GPIO_PFCAE1_PFCAE113);
        rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE113_SHIFT, GPIO_PFCE1_PFCE113);
        rza_io_reg_write_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC113_SHIFT,  GPIO_PFC1_PFC113);
    }

    if (adc_port & (0x01 << 6))
    {
        /* Configure choice of pin as AN6 */
        rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC114_SHIFT,  GPIO_PMC1_PMC114);
        rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE114_SHIFT,GPIO_PFCAE1_PFCAE114);
        rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE114_SHIFT, GPIO_PFCE1_PFCE114);
        rza_io_reg_write_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC114_SHIFT,  GPIO_PFC1_PFC114);
    }

    if (adc_port & (0x01 << 7))
    {
        /* Configure choice of pin as AN7 */
        rza_io_reg_write_16(&GPIO.PMC1,  1, GPIO_PMC1_PMC115_SHIFT,  GPIO_PMC1_PMC115);
        rza_io_reg_write_16(&GPIO.PFCAE1,0, GPIO_PFCAE1_PFCAE115_SHIFT,GPIO_PFCAE1_PFCAE115);
        rza_io_reg_write_16(&GPIO.PFCE1, 0, GPIO_PFCE1_PFCE115_SHIFT, GPIO_PFCE1_PFCE115);
        rza_io_reg_write_16(&GPIO.PFC1,  0, GPIO_PFC1_PFC115_SHIFT,  GPIO_PFC1_PFC115);
    }

    if (!abort)
    {
        /* static variable initialisation */
        g_adc_endflg = 0u;

        set_ch_used(r_adc_param->ch);

        /* IRQ interrupt Enable */
        __enable_irq();

        /* FIQ interrupt Enable */
        __enable_fiq();
    }
}
/******************************************************************************
 * End of Function adc_initialise
 ******************************************************************************/

/******************************************************************************
* Function Name: adc_start
* Description  : ADC start
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_start (void)
{
    uint16_t adcsr_tmp;

    /* Configure ADC
    ADC Start, Other settings are saved
    ADCSR   - A/D  control/status register
    b15     - A/D end flag              - No setting
    b14     - A/D interrupt enable      - No setting
    b13     - A/D start                 - Start
    b12-9   - Timer Trigger set         - No setting
    b8-6    - Clock Select              - No setting
    b5-3    - Multi-scan mode           - No setting
    b2-0    - Channel Select            - No change */
    adcsr_tmp = ADC.ADCSR;
    ADC.ADCSR = (uint16_t)((uint32_t)adcsr_tmp | ADST_ON);
}
/******************************************************************************
 * End of Function adc_start
 ******************************************************************************/

/******************************************************************************
* Function Name: adc_stop
* Description  : ADC stop
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_stop (void)
{
    uint16_t adcsr_tmp;

    /* Configure ADC
    ADC Stop, Other settings are saved
    ADCSR     - A/D  control/status register
    b15       - A/D end flag            - No setting
    b14       - A/D interrupt enable    - No setting
    b13       - A/D start               - Stop
    b12-9     - Timer Trigger set       - No setting
    b8-6      - Clock Select            - No setting
    b5-3      - Multi-scan mode         - No setting
    b2-0      - Channel Select          - No change */
    adcsr_tmp = ADC.ADCSR;
    ADC.ADCSR = (uint16_t)((uint32_t)adcsr_tmp & ADST_OFF);
}
/******************************************************************************
 * End of Function adc_stop
 ******************************************************************************/

/******************************************************************************
* Function Name: adc_read_single
* Description  : ANx A/D conversion
* Arguments    : p_adc_data - pointer to buffer to store the ADC result
*                adc_ch_begin - ADC channel desired (see r_adc_ch)
*                adc_ch_amount - the number of ADC channels desired
* Return Value : A/D conversion value
*              : Time out error
******************************************************************************/
static void adc_read_single (uint16_t p_adc_data[], r_adc_ch adc_ch_begin, uint8_t adc_ch_amount)
{
    uint32_t adc_time_count;

    adc_time_count = 0u;

    /* Wait */
    do
    {
        if (TIME_OUT <= adc_time_count)
        {
            /* Time out. This is intentional. */
           break;
        }

        /* Time count up */
        adc_time_count++;
    }
    while (ADC_SET_FLG != (uint32_t)g_adc_endflg);

    if (TIME_OUT <= adc_time_count)
    {
        /* ADC stop */
        adc_stop();

        /* Timeout error */
        *p_adc_data = ADC_READ_ERR;
    }
    else if ((adc_ch_amount == 0) || (adc_ch_amount + adc_ch_begin - 1 > R_ADC_CH7))
    {
        /* Parameter error */
        *p_adc_data = ADC_READ_ERR;
    }
    else
    {
        /* Normal processing 6bit right shift ADCDR  */
        volatile uint16_t * p_adc_addr;
        int32_t loop_ch;

        p_adc_addr = (uint16_t*)&(ADC.ADDRA) + adc_ch_begin;

        for (loop_ch = 0; loop_ch < (int32_t)adc_ch_amount; loop_ch++)
        {
            p_adc_data[loop_ch] = (uint16_t)(((uint32_t)*p_adc_addr >> ADC_BIT_SHIFT_4) & ADC_12BIT_SET);
            p_adc_addr++;
        }
    }

    /* ADI interrupt disabled */
    R_INTC_Disable(INTC_ID_ADI);

    /* g_adc_end_flg clear */
    g_adc_endflg = 0u;

    /* ADI interrupt Enable */
    R_INTC_Enable(INTC_ID_ADI);
}
/******************************************************************************
 * End of Function adc_read_single
 ******************************************************************************/

/******************************************************************************
* Function Name: adc_uninitialise
* Description  : ADC Uninitialise
* Arguments    : none
* Return Value : none
******************************************************************************/
static void adc_uninitialise (void)
{
    /* Configure ADC
    ADC All clear
    ADCSR     - A/D  control/status register */
    ADC.ADCSR = 0u;
}
/******************************************************************************
 * End of Function adc_uninitialise
 ******************************************************************************/

/******************************************************************************
 * Function Name: int_adc_end
 * Description  : A/D conversion end interrupt
 *              : Set g_adc_end_flg of Inform conversion end
 * Arguments    : Type of interrupt request (unused)
 * Return Value : none
 ******************************************************************************/
static void int_adc_end (uint32_t int_sense)
{
    uint16_t adcsr_tmp;

    UNUSED_PARAM(int_sense);


    g_adc_endflg = ADC_SET_FLG;

       /* Clear ADC flag */
    if (0x8000u == (ADC.ADCSR & 0x8000u))
    {
        /* Clear ADC complete flag */
        ADC.ADCSR &= 0x7FFFu;
    }

    /* Read the result register for AN2 */
    g_adc_result = ((uint32_t)ADC.ADDRC >> 4u);

    /* Check if the upper and lower limits have been read */
    if ((uint32_t)0u == g_adc_result)
    {
        g_adc_complete_flag |= 0x80u;
    }

    if ((uint32_t)4090u < g_adc_result)
    {
        g_adc_complete_flag |= 0x01u;
    }

    /* -> MISRA 10.6 */
    /* Configure ADC
    Clear of A/D end flag, Other settings are saved
    ADCSR   - A/D control/status register
    b15     - A/D end flag              - CLEAR
    b14     - A/D interrupt enable      - No setting
    b13     - A/D start                 - No setting
    b12-9   - Timer Trigger set         - No setting
    b8-6    - Clock Select              - No setting
    b5-3    - Multiple-scan mode           - No setting
    b2-0    - Channel Select            - No setting */
    adcsr_tmp = ADC.ADCSR;
    ADC.ADCSR = (uint16_t)((uint32_t)adcsr_tmp & ADF_CLEAR);
}
/******************************************************************************
 * End of Function int_adc_end
 ******************************************************************************/

/******************************************************************************
* Interface Functions
******************************************************************************/

/******************************************************************************
* Function Name: R_ADC_Open
* Description  : Enables and configures ADC.
* Arguments    : r_adc_cfg - ADC parameters see r_adc_ch
* Return Value : 0
******************************************************************************/
void R_ADC_Open(r_adc_cfg* r_adc_param)
{
    if (!s_configured)
    {
        uint8_t  stbcr6_tmp;

        /* Registration interrupt function  */
        R_INTC_RegistIntFunc(INTC_ID_ADI, &int_adc_end);

        /* Set interrupt priority */
        R_INTC_SetPriority(INTC_ID_ADI, ADC_PRI_ADI);

        /* A/D end interrupt enable */
        R_INTC_Enable(INTC_ID_ADI);

        /* IRQ interrupt disabled */
        __disable_fiq();

        /* FIQ interrupt disabled */
        __disable_irq();

        /* Configure Low-power mode register
        ADC clock supply,Other settings are saved
        STBCR6  - Standby control register 6
        b7      - Module Stop 67            - ADC clock supply
        b6-0    - Module Stop 66-63         - No setting */
        stbcr6_tmp = CPG.STBCR6;
        CPG.STBCR6 = (uint8_t)((uint32_t)stbcr6_tmp & ~STBCR6_7_SET);

        s_configured = true;
    }

    /* ADC initialise */
    adc_initialise(r_adc_param);
}
/******************************************************************************
 * End of Function R_ADC_Open
 ******************************************************************************/

/******************************************************************************
* Function Name: R_ADC_Close
* Description  : Disables the ADC
* Arguments    : ch - ADC channel see r_adc_ch
* Return Value : 0
******************************************************************************/
void R_ADC_Close (r_adc_ch ch)
{
    /* A/D end interrupt disable */
    R_INTC_Disable(INTC_ID_ADI);

    clear_ch_used(ch);

    if (0 == s_ch_in_use)
    {
        uint8_t  stbcr6_tmp;

        /* A/D end interrupt enable */
        R_INTC_Disable(INTC_ID_ADI);

        /* IRQ interrupt disabled */
        __disable_fiq();

        /* FIQ interrupt disabled */
        __disable_irq();

        /* ADC Uninitialised */
        adc_uninitialise();

        /* Configure Low-power mode register
        Clear ADC clock supply,Other settings are saved
        STBCR6  - Standby control register 6
        b7      - Module Stop 67            - ADC clock supply
        b6-0    - Module Stop 66-63         - No setting */
        stbcr6_tmp = CPG.STBCR6;
        CPG.STBCR6 = (uint8_t)((uint32_t)stbcr6_tmp | STBCR6_7_SET);

        s_configured = false;
    }
}
/******************************************************************************
 * End of Function R_ADC_Close
 ******************************************************************************/

/******************************************************************************
 * Function Name: R_ADC_Read
 * Description  : Read value from ADC channel
 * Arguments    : p_adc_data - pointer to buffer to store the ADC result
 *                adc_ch_begin - ADC channel desired (see r_adc_ch)
 *                adc_ch_amount - the numner of ADC channel desired
 * Return Value : Note
 ******************************************************************************/
void R_ADC_Read (uint16_t p_adc_data[], r_adc_ch adc_ch_begin, uint8_t adc_ch_amount)
{
    /* ADC start */
    adc_start();

    /* Wait for conversion to end */
    while (ADC_SET_FLG != g_adc_endflg)
    {
        R_COMPILER_Nop();
    }

    adc_read_single(p_adc_data, adc_ch_begin, adc_ch_amount);

    /* ADC start */
    adc_stop();
}
/******************************************************************************
 * End of Function R_ADC_Read
 ******************************************************************************/

/* End of File */
