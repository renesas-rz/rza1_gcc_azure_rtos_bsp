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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         r_vdc_pl.c
* @version      1.00
* $Rev: 99 $
* $Date:: 2017-04-10 20:56:26 +0900#$
* @brief        VDC OS porting
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "mcu_board_select.h"                /* Device driver header */
#include "iodefine_cfg.h"
#include "dev_drv.h"                /* Device driver header */
#include "r_intc.h"                 /* Interrupt Controller driver header */
#include "r_vdc_pl.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define STP91_BIT   (0x02u)
#define STP90_BIT   (0x01u)
#define STBRQ25_BIT (0x20u)
#define STBAK25_BIT (0x20u)
#define STBRQ24_BIT (0x10u)
#define STBAK24_BIT (0x10u)
#define VDC_INT_PRIORITY  (30) /* level 5 (0-31)*/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static const uint16_t rtos_interrupt_id_table[VDC_CHANNEL_NUM][VDC_INT_TYPE_NUM] =
{
    {
        INTC_ID_S0_VI_VSYNC0,  /* ((IRQn_Type)(75)) */
        INTC_ID_S0_LO_VSYNC0,  /* ((IRQn_Type)(76)) */
        INTC_ID_S0_VSYNCERR0,  /* ((IRQn_Type)(77)) */
        INTC_ID_GR3_VLINE0,    /* ((IRQn_Type)(78)) */
        INTC_ID_S0_VFIELD0,    /* ((IRQn_Type)(79)) */
        INTC_ID_IV1_VBUFERR0,  /* ((IRQn_Type)(80)) */
        INTC_ID_IV3_VBUFERR0,  /* ((IRQn_Type)(81)) */
        INTC_ID_IV5_VBUFERR0,  /* ((IRQn_Type)(82)) */
        INTC_ID_IV6_VBUFERR0,  /* ((IRQn_Type)(83)) */
        INTC_ID_S0_WLINE0,     /* ((IRQn_Type)(84)) */
#if ((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))

        INTC_ID_S1_VI_VSYNC0,  /* ((IRQn_Type)(85)) */
        INTC_ID_S1_LO_VSYNC0,  /* ((IRQn_Type)(86)) */
        INTC_ID_S1_VSYNCERR0,  /* ((IRQn_Type)(87)) */
        INTC_ID_S1_VFIELD0,    /* ((IRQn_Type)(88)) */
        INTC_ID_IV2_VBUFERR0,  /* ((IRQn_Type)(89)) */
        INTC_ID_IV4_VBUFERR0,  /* ((IRQn_Type)(90)) */
        INTC_ID_S1_WLINE0,     /* ((IRQn_Type)(91)) */

        INTC_ID_OIR_VI_VSYNC0, /* ((IRQn_Type)(92)) */
        INTC_ID_OIR_LO_VSYNC0, /* ((IRQn_Type)(93)) */
        INTC_ID_OIR_VSYNCERR0, /* ((IRQn_Type)(94)) */
        INTC_ID_OIR_VFIELD0,   /* ((IRQn_Type)(95)) */
        INTC_ID_IV7_VBUFERR0,  /* ((IRQn_Type)(96)) */
        INTC_ID_IV8_VBUFERR0,  /* ((IRQn_Type)(97)) */
    },
    {
        INTC_ID_S0_VI_VSYNC1,  /* ((IRQn_Type)(99))  */
        INTC_ID_S0_LO_VSYNC1,  /* ((IRQn_Type)(100)) */
        INTC_ID_S0_VSYNCERR1,  /* ((IRQn_Type)(101)) */
        INTC_ID_GR3_VLINE1,    /* ((IRQn_Type)(102)) */
        INTC_ID_S0_VFIELD1,    /* ((IRQn_Type)(103)) */
        INTC_ID_IV1_VBUFERR1,  /* ((IRQn_Type)(104)) */
        INTC_ID_IV3_VBUFERR1,  /* ((,IRQn_Type)(105)) */
        INTC_ID_IV5_VBUFERR1,  /* ((IRQn_Type)(106)) */
        INTC_ID_IV6_VBUFERR1,  /* ((IRQn_Type)(107)) */
        INTC_ID_S0_WLINE1,     /* ((IRQn_Type)(108)) */

        INTC_ID_S1_VI_VSYNC1,  /* ((IRQn_Type)(109)) */
        INTC_ID_S1_LO_VSYNC1,  /* ((IRQn_Type)(110)) */
        INTC_ID_S1_VSYNCERR1,  /* ((IRQn_Type)(111)) */
        INTC_ID_S1_VFIELD1,    /* ((IRQn_Type)(112)) */
        INTC_ID_IV2_VBUFERR1,  /* ((IRQn_Type)(113)) */
        INTC_ID_IV4_VBUFERR1,  /* ((IRQn_Type)(114)) */
        INTC_ID_S1_WLINE1,     /* ((IRQn_Type)(115)) */

        INTC_ID_OIR_VI_VSYNC1, /* ((IRQn_Type)(116)) */
        INTC_ID_OIR_LO_VSYNC1, /* ((IRQn_Type)(117)) */
        INTC_ID_OIR_VLINE1,    /* ((IRQn_Type)(118)) */
        INTC_ID_OIR_VFIELD1,   /* ((IRQn_Type)(119)) */
        INTC_ID_IV7_VBUFERR1,  /* ((IRQn_Type)(120)) */
        INTC_ID_IV8_VBUFERR1,  /* ((IRQn_Type)(121)) */
#endif /*((TARGET_RZA1 == TARGET_RZA1H) || (TARGET_RZA1 == TARGET_RZA1M))*/
    }
};

/**************************************************************************//**
 * Function Name : R_VDC_OnInitialize
 * @brief       VDC User difinistion part Init
 * @param[in]   ch          : Channel
 * @retval      none
 *****************************************************************************/
void R_VDC_OnInitialize ( const uint32_t user_num )
{
    uint32_t            reg_data;
    volatile uint8_t    dummy_read;

    if ((vdc_channel_t)user_num == VDC_CHANNEL_0)
    {
        /* Standby control register 9 (STBCR9)
            b1      ------0-;  MSTP91 : 0 : Video display controller channel 0 & LVDS enable */
        reg_data    = (uint32_t)CPG.STBCR9 & (uint32_t)~STP91_BIT;
        CPG.STBCR9  = (uint8_t)reg_data;

        /* In order to reflect the change, a dummy read should be done. */
        dummy_read = CPG.STBCR9;

        /* Standby Request Register 2 (STBREQ2)
            b5      --0-----;  STBRQ25 : The standby request to VDC channel 0 is invalid. */
        reg_data    = (uint32_t)CPG.STBREQ2 & (uint32_t)~STBRQ25_BIT;
        CPG.STBREQ2 = (uint8_t)reg_data;

        /* Standby Acknowledge Register 2 (STBACK2)
            b5      --*-----;  STBAK25 : Standby acknowledgement from VDC channel 0. */
        while (((uint32_t)CPG.STBACK2 & (uint32_t)STBAK25_BIT) != 0u)
        {
            /* Wait for the STBAK25 to be cleared to 0. */
            ;
        }
    }
    else
    {
        /* Standby control register 9 (STBCR9)
            b1      ------0-;  MSTP91 : 0 : Video display controller channel 0 & LVDS enable
            b0      -------0;  MSTP90 : 0 : Video display controller channel 1 enable */
        reg_data    = (uint32_t)CPG.STBCR9 & (uint32_t)~(STP91_BIT|STP90_BIT);
        CPG.STBCR9  = (uint8_t)reg_data;

        /* In order to reflect the change, a dummy read should be done. */
        dummy_read = CPG.STBCR9;
        (void) dummy_read;

        /* Standby Request Register 2 (STBREQ2)
            b4      ---0----;  STBRQ24 : The standby request to VDC channel 1 is invalid. */
        reg_data    = (uint32_t)CPG.STBREQ2 & (uint32_t)~STBRQ24_BIT;
        CPG.STBREQ2 = (uint8_t)reg_data;

        /* Standby Acknowledge Register 2 (STBACK2)
            b4      ---*----;  STBAK24 : Standby acknowledgement from VDC channel 1. */
        while (((uint32_t)CPG.STBACK2 & (uint32_t)STBAK24_BIT) != 0u)
        {
            /* Wait for the STBAK24 to be cleared to 0. */
            ;
        }
    }
    R_VDC_OnEnableInterrupt( user_num );

}   /* End of function R_VDC_OnInitialize() */

/**************************************************************************//**
 * Function Name : R_VDC_OnFinalize
 * @brief       VDC User difinistion part final
 * @param[in]   ch          : Channel
 * @retval      none
 *****************************************************************************/
void R_VDC_OnFinalize( const uint32_t user_num )
{
    uint32_t            reg_data;
    volatile uint8_t    dummy_read;

    R_VDC_OnDisableInterrupt( user_num );

    if ((vdc_channel_t)user_num == VDC_CHANNEL_0)
    {
        /* Standby Request Register 2 (STBREQ2)
            b5      --1-----;  STBRQ25 : The standby request to VDC channel 0 is valid. */
        reg_data    = (uint32_t)CPG.STBREQ2 | (uint32_t)STBRQ25_BIT;
        CPG.STBREQ2 = (uint8_t)reg_data;

        /* Standby Acknowledge Register 2 (STBACK2)
            b5      --*-----;  STBAK25 : Standby acknowledgement from VDC channel 0. */
        while (((uint32_t)CPG.STBACK2 & (uint32_t)STBAK25_BIT) == 0u)
        {
            /* Wait for the STBAK25 to be set to 1. */
            ;
        }

        /* Standby control register 9 (STBCR9)
            b1      ------1-;  MSTP91 : 1 : Video display controller channel 0 & LVDS disable */
        reg_data    = (uint32_t)CPG.STBCR9 | (uint32_t)STP91_BIT;
        CPG.STBCR9  = (uint8_t)reg_data;
    }
    else
    {
        /* Standby Request Register 2 (STBREQ2)
            b4      ---1----;  STBRQ24 : The standby request to VDC channel 1 is valid. */
        reg_data    = (uint32_t)CPG.STBREQ2 | (uint32_t)STBRQ24_BIT;
        CPG.STBREQ2 = (uint8_t)reg_data;

        /* Standby Acknowledge Register 2 (STBACK2)
            b4      ---*----;  STBAK24 : Standby acknowledgement from VDC channel 1. */
        while (((uint32_t)CPG.STBACK2 & (uint32_t)STBAK24_BIT) == 0u)
        {
            /* Wait for the STBAK24 to be set to 1. */
            ;
        }

        /* Standby control register 9 (STBCR9)
            b1      -------1;  MSTP90 : 1 : Video display controller channel 1 */
        reg_data    = (uint32_t)CPG.STBCR9 | (uint32_t)STP90_BIT;
        CPG.STBCR9  = (uint8_t)reg_data;
    }

    /* In order to reflect the change, a dummy read should be done. */
    dummy_read = CPG.STBCR9;
    (void) dummy_read;
}   /* End of function R_VDC_OnFinalize() */

/**************************************************************************//**
 * Function Name : R_VDC_OnEnableInterrupt
 * @brief       VDC User difinistion part Enable Interrupt
 * @param[in]   ch          : Channel
 * @retval      Error code
 *****************************************************************************/
int32_t R_VDC_OnEnableInterrupt(const vdc_channel_t ch)
{
   int32_t        int_err = DEVDRV_SUCCESS;
   int32_t        ret_code = 0;
   uint16_t       num_of_irq;
   void           (* func)(uint32_t int_sense);
   vdc_int_type_t int_type;

   for (int_type = (vdc_int_type_t)0; (int_type < VDC_INT_TYPE_NUM) && (DEVDRV_SUCCESS == int_err); int_type++)
   {
       num_of_irq  = rtos_interrupt_id_table[ch][int_type];
       func    = R_VDC_GetISR(ch, int_type);
       int_err = R_INTC_RegistIntFunc(num_of_irq, func);

       if(DEVDRV_SUCCESS != int_err)
       {
           ret_code = -1;
       }
       else
       {
           ret_code = R_INTC_SetPriority(num_of_irq, VDC_INT_PRIORITY);
           if(DEVDRV_SUCCESS != int_err)
           {
               ret_code = -1;
           }
           else
           {
               ret_code = R_INTC_Enable(num_of_irq);
               if(DEVDRV_SUCCESS != int_err)
               {
                   ret_code = -1;
               }
           }
       }
   }

    return ret_code;

}   /* End of function R_VDC_OnEnableInterrupt() */

/**************************************************************************//**
 * Function Name : R_VDC_OnDisableInterrupt
 * @brief       VDC User difinistion part Disable Interrupt
 * @param[in]   ch          : Channel
 * @retval      Error code
 *****************************************************************************/
int32_t R_VDC_OnDisableInterrupt(const vdc_channel_t ch)
{
   int32_t        int_err = DEVDRV_SUCCESS;
   int32_t         ret_code = 0;
   uint16_t       num_of_irq;
   vdc_int_type_t  int_type;

   for (int_type = (vdc_int_type_t)0; (int_type < VDC_INT_TYPE_NUM) && (DEVDRV_SUCCESS == int_err); int_type++)
   {
       num_of_irq  = rtos_interrupt_id_table[ch][int_type];
       ret_code = R_INTC_Disable(num_of_irq);
       if(DEVDRV_SUCCESS != int_err)
       {
           ret_code = -1;
       }
   }

   return ret_code;
}   /* End of function R_VDC_OnDisableInterrupt() */
