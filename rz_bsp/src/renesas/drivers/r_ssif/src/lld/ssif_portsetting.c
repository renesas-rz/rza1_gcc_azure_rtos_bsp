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
#if (TARGET_BOARD == TARGET_BOARD_RSK)
            /* SSISCK0(P4_4, Alternative Mode 5,InputOutput) */
            GPIO.PIBC4  &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PM4    |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PMC4   &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PIPC4  &= (uint16_t)~(GPIO_BIT_N4);

            GPIO.PBDC4  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PFC4   &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PFCE4  &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PFCAE4 |= (uint16_t) (GPIO_BIT_N4);

            GPIO.PIPC4  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PMC4   |= (uint16_t) (GPIO_BIT_N4);

            /* SSIWS0(P4_5, Alternative Mode 5,InputOutput) */
            GPIO.PIBC4  &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PM4    |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PMC4   &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PIPC4  &= (uint16_t)~(GPIO_BIT_N5);

            GPIO.PBDC4  |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PFC4   &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PFCE4  &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PFCAE4 |= (uint16_t) (GPIO_BIT_N5);

            GPIO.PIPC4  |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PMC4   |= (uint16_t) (GPIO_BIT_N5);

            /* SSIRxD0(P4_6, Alternative Mode 5,Input) */
            GPIO.PIBC4  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PM4    |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PMC4   &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PIPC4  &= (uint16_t)~(GPIO_BIT_N6);

            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PFC4   &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PFCE4  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PFCAE4 |= (uint16_t) (GPIO_BIT_N6);

            GPIO.PIPC4  |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PMC4   |= (uint16_t) (GPIO_BIT_N6);

            /* SSITxD0(P4_7, Alternative Mode 5,Output) */
            GPIO.PIBC4  &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PM4    |= (uint16_t) (GPIO_BIT_N7);
            GPIO.PMC4   &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PIPC4  &= (uint16_t)~(GPIO_BIT_N7);

            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PFC4   &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PFCE4  &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PFCAE4 |= (uint16_t) (GPIO_BIT_N7);

            GPIO.PMC4   |= (uint16_t) (GPIO_BIT_N7);
            GPIO.PM4    &= (uint16_t)~(GPIO_BIT_N7);

#elif (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
            /* SSISCK0:  P6_8, Alt3(010), Input/Output */
            GPIO.PIBC6  &= (uint16_t)~(GPIO_BIT_N8);
            GPIO.PBDC6  &= (uint16_t)~(GPIO_BIT_N8);
            GPIO.PM6    |= (uint16_t) (GPIO_BIT_N8);
            GPIO.PMC6   &= (uint16_t)~(GPIO_BIT_N8);
            GPIO.PIPC6  &= (uint16_t)~(GPIO_BIT_N8);

            GPIO.PBDC6  |= (uint16_t) (GPIO_BIT_N8);
            GPIO.PFC6   &= (uint16_t)~(GPIO_BIT_N8);
            GPIO.PFCE6  |= (uint16_t) (GPIO_BIT_N8);
            GPIO.PFCAE6 &= (uint16_t)~(GPIO_BIT_N8);

            GPIO.PIPC6  |= (uint16_t) (GPIO_BIT_N8);
            GPIO.PMC6   |= (uint16_t) (GPIO_BIT_N8);

            /* SSIWS0:   P6_9,Alt3(010), Input/Output */
            GPIO.PIBC6  &= (uint16_t)~(GPIO_BIT_N9);
            GPIO.PBDC6  &= (uint16_t)~(GPIO_BIT_N9);
            GPIO.PM6    |= (uint16_t) (GPIO_BIT_N9);
            GPIO.PMC6   &= (uint16_t)~(GPIO_BIT_N9);
            GPIO.PIPC6  &= (uint16_t)~(GPIO_BIT_N9);

            GPIO.PBDC6  |= (uint16_t) (GPIO_BIT_N9);
            GPIO.PFC6   &= (uint16_t)~(GPIO_BIT_N9);
            GPIO.PFCE6  |= (uint16_t) (GPIO_BIT_N9);
            GPIO.PFCAE6 &= (uint16_t)~(GPIO_BIT_N9);

            GPIO.PIPC6  |= (uint16_t) (GPIO_BIT_N9);
            GPIO.PMC6   |= (uint16_t) (GPIO_BIT_N9);

            /* SSIRxD0:  P6_11,Alt3(010), Input */
            GPIO.PIBC6  &= (uint16_t)~(GPIO_BIT_N11);
            GPIO.PBDC6  &= (uint16_t)~(GPIO_BIT_N11);
            GPIO.PM6    |= (uint16_t) (GPIO_BIT_N11);
            GPIO.PMC6   &= (uint16_t)~(GPIO_BIT_N11);
            GPIO.PIPC6  &= (uint16_t)~(GPIO_BIT_N11);

            GPIO.PBDC6  &= (uint16_t)~(GPIO_BIT_N11);
            GPIO.PFC6   &= (uint16_t)~(GPIO_BIT_N11);
            GPIO.PFCE6  |= (uint16_t) (GPIO_BIT_N11);
            GPIO.PFCAE6 &= (uint16_t)~(GPIO_BIT_N11);

            GPIO.PIPC6  |= (uint16_t) (GPIO_BIT_N11);
            //GPIO.PMC6   &= (uint16_t)~(GPIO_BIT_N11);
            GPIO.PMC6   |= (uint16_t) (GPIO_BIT_N11);

            /* SSITxD0:  P6_10,Alt3(010), Output */
            GPIO.PIBC6  &= (uint16_t)~(GPIO_BIT_N10);
            GPIO.PBDC6  &= (uint16_t)~(GPIO_BIT_N10);
            GPIO.PM6    |= (uint16_t) (GPIO_BIT_N10);
            GPIO.PMC6   &= (uint16_t)~(GPIO_BIT_N10);
            GPIO.PIPC6  &= (uint16_t)~(GPIO_BIT_N10);

            GPIO.PBDC6  &= (uint16_t)~(GPIO_BIT_N10);
            GPIO.PFC6   &= (uint16_t)~(GPIO_BIT_N10);
            GPIO.PFCE6  |= (uint16_t) (GPIO_BIT_N10);
            GPIO.PFCAE6 &= (uint16_t)~(GPIO_BIT_N10);

            GPIO.PMC6   |= (uint16_t) (GPIO_BIT_N10);
            GPIO.PM6    &= (uint16_t)~(GPIO_BIT_N10);
#else
            /* no connection */
#endif
            break;

        case SSIF_CHNUM_1:
#if (TARGET_BOARD == TARGET_BOARD_RSK)
            /* SSISCK1(P3_4, Alternative Mode 3,InputOutput) */
            GPIO.PIBC3  &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PM3    |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PMC3   &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PIPC3  &= (uint16_t)~(GPIO_BIT_N4);

            GPIO.PBDC3  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PFC3   &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PFCE3  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PFCAE3 &= (uint16_t)~(GPIO_BIT_N4);

            GPIO.PIPC3  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PMC3   |= (uint16_t) (GPIO_BIT_N4);

            /* SSIWS1(P3_5, Alternative Mode 3,InputOutput) */
            GPIO.PIBC3  &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PM3    |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PMC3   &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PIPC3  &= (uint16_t)~(GPIO_BIT_N5);

            GPIO.PBDC3  |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PFC3   &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PFCE3  |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PFCAE3 &= (uint16_t)~(GPIO_BIT_N5);

            GPIO.PIPC3 |=  (uint16_t) (GPIO_BIT_N5);
            GPIO.PMC3   |= (uint16_t) (GPIO_BIT_N5);

            /* SSIRxD1(P3_6, Alternative Mode 3,Input) */
            GPIO.PIBC3  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PM3    |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PMC3   &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PIPC3  &= (uint16_t)~(GPIO_BIT_N6);

            GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PFC3   &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PFCE3  |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PFCAE3 &= (uint16_t)~(GPIO_BIT_N6);

            GPIO.PIPC3  |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PMC3   |= (uint16_t) (GPIO_BIT_N6);

            /* SSITxD1:  no connection */
#else
            /* no connection */
#endif
            break;

        case SSIF_CHNUM_2:
            /* SSISCK2:  no connection */
            /* SSIWS2:   no connection */
            /* SSIDATA2: no connection */
            break;

        case SSIF_CHNUM_3:
#if (TARGET_BOARD == TARGET_BOARD_RSK)
            /* SSISCK3(P4_12, Alternative Mode 6,InputOutput) */
            GPIO.PIBC4  &= (uint16_t)~(GPIO_BIT_N12);
            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N12);
            GPIO.PM4    |= (uint16_t) (GPIO_BIT_N12);
            GPIO.PMC4   &= (uint16_t)~(GPIO_BIT_N12);
            GPIO.PIPC4  &= (uint16_t)~(GPIO_BIT_N12);

            GPIO.PBDC4  |= (uint16_t) (GPIO_BIT_N12);
            GPIO.PFC4   |= (uint16_t) (GPIO_BIT_N12);
            GPIO.PFCE4  &= (uint16_t)~(GPIO_BIT_N12);
            GPIO.PFCAE4 |= (uint16_t) (GPIO_BIT_N12);

            GPIO.PIPC4  |= (uint16_t) GPIO_BIT_N12;
            GPIO.PMC4   |= (uint16_t) (GPIO_BIT_N12);

            /* SSIWS3(P4_13, Alternative Mode 6,InputOutput) */
            GPIO.PIBC4  &= (uint16_t)~(GPIO_BIT_N13);
            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N13);
            GPIO.PM4    |= (uint16_t) (GPIO_BIT_N13);
            GPIO.PMC4   &= (uint16_t)~(GPIO_BIT_N13);
            GPIO.PIPC4  &= (uint16_t)~(GPIO_BIT_N13);

            GPIO.PBDC4  |= (uint16_t) (GPIO_BIT_N13);
            GPIO.PFC4   |= (uint16_t) (GPIO_BIT_N13);
            GPIO.PFCE4  &= (uint16_t)~(GPIO_BIT_N13);
            GPIO.PFCAE4 |= (uint16_t) (GPIO_BIT_N13);

            GPIO.PIPC4  |= (uint16_t) (GPIO_BIT_N13);
            GPIO.PMC4   |= (uint16_t) (GPIO_BIT_N13);

            /* SSIRxD3:  no connection */

            /* SSITxD3(P4_15, Alternative Mode 6,Output) */
            GPIO.PIBC4  &= (uint16_t)~(GPIO_BIT_N15);
            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N15);
            GPIO.PM4    |= (uint16_t) (GPIO_BIT_N15);
            GPIO.PMC4   &= (uint16_t)~(GPIO_BIT_N15);
            GPIO.PIPC4  &= (uint16_t)~(GPIO_BIT_N15);

            GPIO.PBDC4  &= (uint16_t)~(GPIO_BIT_N15);
            GPIO.PFC4   |= (uint16_t) (GPIO_BIT_N15);
            GPIO.PFCE4  &= (uint16_t)~(GPIO_BIT_N15);
            GPIO.PFCAE4 |= (uint16_t) (GPIO_BIT_N15);

            GPIO.PMC4   |= (uint16_t) (GPIO_BIT_N15);
            GPIO.PM4    &= (uint16_t)~(GPIO_BIT_N15);
#else
            /* no connection */
#endif
            break;

#if (TARGET_BOARD == TARGET_BOARD_RSK)
        case SSIF_CHNUM_4:
            /* SSISCK4(P11_4, Alternative Mode 3,InputOutput) */
            GPIO.PIBC11  &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PBDC11  &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PM11    |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PMC11   &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PIPC11  &= (uint16_t)~(GPIO_BIT_N4);

            GPIO.PBDC11  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PFC11   &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PFCE11  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PFCAE11 &= (uint16_t)~(GPIO_BIT_N4);

            GPIO.PIPC11  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PMC11   |= (uint16_t) (GPIO_BIT_N4);

            /* SSIWS4(P11_5, Alternative Mode 3,InputOutput) */
            GPIO.PIBC11  &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PBDC11  &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PM11    |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PMC11   &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PIPC11  &= (uint16_t)~(GPIO_BIT_N5);

            GPIO.PBDC11  |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PFC11   &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PFCE11  |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PFCAE11 &= (uint16_t)~(GPIO_BIT_N5);

            GPIO.PIPC11  |= (uint16_t) GPIO_BIT_N5;
            GPIO.PMC11   |= (uint16_t) (GPIO_BIT_N5);

            /* SSIDATA4(P11_6, Alternative Mode 3,InputOutput) */
            GPIO.PIBC11  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PBDC11  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PM11    |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PMC11   &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PIPC11  &= (uint16_t)~(GPIO_BIT_N6);

            GPIO.PBDC11  |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PFC11   &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PFCE11  |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PFCAE11 &= (uint16_t)~(GPIO_BIT_N6);

            GPIO.PIPC11  |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PMC11   |= (uint16_t) (GPIO_BIT_N6);
            break;

        case SSIF_CHNUM_5:
            /* SSISCK5(P2_4, Alternative Mode 4,InputOutput) */
            GPIO.PIBC2  &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PBDC2  &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PM2    |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PMC2   &= (uint16_t)~(GPIO_BIT_N4);
            GPIO.PIPC2  &= (uint16_t)~(GPIO_BIT_N4);

            GPIO.PBDC2  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PFC2   |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PFCE2  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PFCAE2 &= (uint16_t)~(GPIO_BIT_N4);

            GPIO.PIPC2  |= (uint16_t) (GPIO_BIT_N4);
            GPIO.PMC2   |= (uint16_t) (GPIO_BIT_N4);

            /* SSIWS5(P2_5, Alternative Mode 4,InputOutput) */
            GPIO.PIBC2  &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PBDC2  &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PM2    |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PMC2   &= (uint16_t)~(GPIO_BIT_N5);
            GPIO.PIPC2  &= (uint16_t)~(GPIO_BIT_N5);

            GPIO.PBDC2  |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PFC2   |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PFCE2  |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PFCAE2 &= (uint16_t)~(GPIO_BIT_N5);

            GPIO.PIPC2  |= (uint16_t) (GPIO_BIT_N5);
            GPIO.PMC2   |= (uint16_t) (GPIO_BIT_N5);

            /* SSIRxD5(P2_6, Alternative Mode 4,Input) */
            GPIO.PIBC2  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PBDC2  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PM2    |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PMC2   &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PIPC2  &= (uint16_t)~(GPIO_BIT_N6);

            GPIO.PBDC2  &= (uint16_t)~(GPIO_BIT_N6);
            GPIO.PFC2   |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PFCE2  |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PFCAE2 &= (uint16_t)~(GPIO_BIT_N6);

            GPIO.PIPC2  |= (uint16_t) (GPIO_BIT_N6);
            GPIO.PMC2   |= (uint16_t) (GPIO_BIT_N6);

            /* SSITxD5(P2_7, Alternative Mode 4,Output) */
            GPIO.PIBC2  &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PBDC2  &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PM2    |= (uint16_t) (GPIO_BIT_N7);
            GPIO.PMC2   &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PIPC2  &= (uint16_t)~(GPIO_BIT_N7);

            GPIO.PBDC2  &= (uint16_t)~(GPIO_BIT_N7);
            GPIO.PFC2   |= (uint16_t) (GPIO_BIT_N7);
            GPIO.PFCE2  |= (uint16_t) (GPIO_BIT_N7);
            GPIO.PFCAE2 &= (uint16_t)~(GPIO_BIT_N7);

            GPIO.PMC2   |= (uint16_t) (GPIO_BIT_N7);
            GPIO.PM2    &= (uint16_t)~(GPIO_BIT_N7);

            /* AUDIO_CLK(P3_1, Alternative Mode 6,Input) */
            GPIO.PIBC3  &= (uint16_t)~(GPIO_BIT_N1);
            GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N1);
            GPIO.PM3    |= (uint16_t) (GPIO_BIT_N1);
            GPIO.PMC3   &= (uint16_t)~(GPIO_BIT_N1);
            GPIO.PIPC3  &= (uint16_t)~(GPIO_BIT_N1);

            GPIO.PBDC3  &= (uint16_t)~(GPIO_BIT_N1);
            GPIO.PFC3   |= (uint16_t) (GPIO_BIT_N1);
            GPIO.PFCE3  &= (uint16_t)~(GPIO_BIT_N1);
            GPIO.PFCAE3 |= (uint16_t) (GPIO_BIT_N1);

            GPIO.PIPC3  |= (uint16_t) (GPIO_BIT_N1);
            GPIO.PMC3   |= (uint16_t) (GPIO_BIT_N1);
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
