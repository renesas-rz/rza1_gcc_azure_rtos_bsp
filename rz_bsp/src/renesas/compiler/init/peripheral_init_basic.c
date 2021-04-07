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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : peripheral_init_basic.c
* Device(s)     : RZ/A1L
* Tool-Chain    : GNUARM-NONE-EABI-v16.01
* H/W Platform  : Stream it! v2
* Description   : Basic peripheral initialisation function
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 06.07.2016 1.00    Ported from RZA1H
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* Default  type definition header */
#include "r_typedefs.h"

/* I/O Register root header */
#include "iodefine_cfg.h"

/* Low level register read/write header */
#include "rza_io_regrw.h"

/* Interchangeable compiler specific header */
#include "compiler_settings.h"


/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
void R_INIT_PeripheralBasic(void);

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
static void cpg_init(void);

/*******************************************************************************
* Function Name: R_INIT_PeripheralBasic
* Description  : Calls the initialisation routine for the clock pulse generator.
*              : setting.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void R_INIT_PeripheralBasic(void)
{
    /* ==== CPG setting ====*/
    cpg_init() ;
}
/*******************************************************************************
 End of function R_INIT_PeripheralBasic
 *******************************************************************************/


/*******************************************************************************
* Function Name: cpg_init
* Description  : Executes initial setting for the CPG.
*              : In the sample code, the internal clock ratio is set to be
*              : I:G:B:P1:P0 = 30:20:10:5:5/2 in the state that the
*              : clock mode is 0. The frequency is set to be as below when the
*              : input clock is 13.33MHz.
*              : CPU clock (I clock)              : 400MHz
*              : Image processing clock (G clock) : 266.67MHz
*              : Internal bus clock (B clock)     : 133.33MHz
*              : Peripheral clock1 (P1 clock)     : 66.67MHz
*              : Peripheral clock0 (P0 clock)     : 33.33MHz
*              : Sets the data-retention RAM area (H'2000 0000 to H'2001 FFFF)
*              : to be enabled for writing.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void cpg_init (void)
{
    volatile uint32_t dummy = 0;

    UNUSED_VARIABLE(dummy);

    /* standby_mode_en bit of Power Control Register setting */
    L2C.REG15_POWER_CTRL = 0x00000001;
    dummy = L2C.REG15_POWER_CTRL ;

    /* ==== CPG Settings ==== */
    /* 0x1035 = 0001 0000 0011 0101
      CKOEN[1:0] = 01 - CKIO output low during standby
      IFC[1:0] = 00 - '/1' PLL OUTPUT = CPU Clock

      Default Stream it! v2: MD_CLK = 1 (USB_X1) used @ 48MHz

      PLL(x32), I:B:P1:P0 = 8:8/3:4/3:2/3
            I = 384MHz (400MHz maximum)
            B = 128MHz (133.33MHz maximum)
            P1 = 64MHz (66.67MHz maximum)
            P0 = 32MHz (33.33MHz maximum)
*/
    CPG.FRQCR  = 0x1035u;
    
/* ----  Writing to On-Chip Data-Retention RAM is enabled. ---- */
    CPG.SYSCR3 = 0x0Fu;
    dummy = CPG.SYSCR3;
}
/*******************************************************************************
 End of function cpg_init
 *******************************************************************************/









