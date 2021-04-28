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
* Copyright (C) 2016-2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name   : ssif_portsetting.c
* $Rev: 8524 $
* $Date:: 2018-06-20 10:40:27 +0100#$
* @brief        SSIF Driver port setting functions
******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include "ssif_if.h"
#include "gpio_iodefine.h"
#include "mcu_board_select.h"
#include "Renesas_RZ_A1.h"
#include "r_port_sc_cfg.h"
#include "r_gpio_if.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/

/* SSIF channel number */
#define SSIF_CHNUM_0    (0u)
#define SSIF_CHNUM_1    (1u)
#define SSIF_CHNUM_2    (2u)
#define SSIF_CHNUM_3    (3u)
#if (TARGET_RZA1 <= TARGET_RZA1LU)
#else /* TARGET_RZA1H */
#define SSIF_CHNUM_4    (4u)
#define SSIF_CHNUM_5    (5u)
#endif

/* misc constant value */
#define SSIF_I2S_LR_CH  (2u)

/*******************************************************************************
Exported global variables (to be accessed by other files)
*******************************************************************************/

/******************************************************************************
* Function Name: SSIF_PortSetting
* @brief         This function initialise pin multiplex settings.
*
*                Description:<br>
*                R7S72100 Boards depended pin connections bellow<br>
*                Clock settings<br>
*                  AUDIO_X1 : Private use pin(nothing to do)<br>
*                  AUDIO_X2 : No connection<br>
*                  AUDIO_CLK: Working with SSIF5<br>
*                Channel settings<br>
*                  SSIF0    : Fully connected to WM8978<br>
*                  SSIF1    : Read only (NC:SSITxD1) connected to CD Deck<br>
*                  SSIF2    : No connection<br>
*                  SSIF3    : Write only (NC:SSIRxD3) connected to AK4353<br>
*                  SSIF4    : Fully connected to AK4353<br>
*                  SSIF5    : Fully connected to HCI
* @param[in]     ssif_ch       :channel number.
* @retval        IOIF_ESUCCESS :Success.
* @retval        error code    :Failure.
******************************************************************************/
int_t SSIF_PortSetting(const uint32_t ssif_ch)
{
    int_t ercd = IOIF_ESUCCESS;

    /* -> IPA R2.4.2 : This is implicit type conversion that doesn't have bad effect on writing to 16bit register. */
    switch (ssif_ch)
    {
        case SSIF_CHNUM_0:
            /* SSITxD1:  no connection */
            set_pins_function(&GPIO_SC_INIT_ssi0);

            break;

        case SSIF_CHNUM_1:

            /* SSITxD1:  no connection */
            set_pins_function(&GPIO_SC_INIT_ssi1);

            break;

        case SSIF_CHNUM_2:
            /* SSISCK2:  no connection */
            /* SSIWS2:   no connection */
            /* SSIDATA2: no connection */
        	set_pins_function(&GPIO_SC_INIT_ssi2);
            break;

        case SSIF_CHNUM_3:

        	/* SSITxD1:  no connection */
        	set_pins_function(&GPIO_SC_INIT_ssi3);

            break;

#if (TARGET_RZA1 == TARGET_RZA1H)
        case SSIF_CHNUM_4:
            /* SSISCK4(P11_4, Alternative Mode 3,InputOutput) */
            /* SSIWS4(P11_5, Alternative Mode 3,InputOutput) */
            /* SSIDATA4(P11_6, Alternative Mode 3,InputOutput) */
        	/* SSITxD1:  no connection */
        	set_pins_function(&GPIO_SC_INIT_ssi4);
            break;

        case SSIF_CHNUM_5:
            /* SSISCK5(P2_4, Alternative Mode 4,InputOutput) */
            /* SSIWS5(P2_5, Alternative Mode 4,InputOutput) */
            /* SSIRxD5(P2_6, Alternative Mode 4,Input) */
            /* SSITxD5(P2_7, Alternative Mode 4,Output) */
            /* AUDIO_CLK(P3_1, Alternative Mode 6,Input) */
        	/* SSITxD1:  no connection */
        	set_pins_function(&GPIO_SC_INIT_ssi5);
            break;
#else
#endif

        default:
            ercd = IOIF_EINVAL;
            break;
    }
    /* <- IPA R2.4.2 */

    return ercd;
}
/* End of File */
