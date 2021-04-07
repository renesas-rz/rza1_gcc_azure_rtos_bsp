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
 * Copyright (C) 2014-2017 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/**************************************************************************//**
 * @file         sound_max9856.c
 * $Rev: 8475 $
 * $Date:: 2018-06-18 11:21:41 +0100#$
 * @brief        sound driver max9856 depended functions
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "sound.h"
#include "mcu_board_select.h"
#include "r_os_abstraction_api.h"
#include "dev_drv.h"

#include "riic_max9856_if.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* MIC Gain settings */
#define MAX9856_MICR_GAIN_30DB_PRV_  (0 | MAX9856_AUDIOINP_BIT_PAENR1_PRV_ | MAX9856_AUDIOINP_BIT_PAENR0_PRV_)
#define MAX9856_MICL_GAIN_30DB_PRV_  (0 | MAX9856_AUDIOINP_BIT_PAENL1_PRV_ | MAX9856_AUDIOINP_BIT_PAENL0_PRV_)
#define MAX9856_MICR_GAIN_20DB_PRV_  (0 | MAX9856_AUDIOINP_BIT_PAENR1_PRV_)
#define MAX9856_MICL_GAIN_20DB_PRV_  (0 | MAX9856_AUDIOINP_BIT_PAENL1_PRV_)
#define MAX9856_MICR_GAIN_10DB_PRV_  (0 | MAX9856_AUDIOINP_BIT_PAENR0_PRV_)
#define MAX9856_MICL_GAIN_10DB_PRV_  (0 | MAX9856_AUDIOINP_BIT_PAENL0_PRV_)
#define MAX9856_MICR_OFF_PRV_        (0)
#define MAX9856_MICL_OFF_PRV_        (0)

/* Selected MIC Gain */
#define MAX9856_MICL_GAIN_PRV_       (MAX9856_MICL_GAIN_10DB_PRV_)
#define MAX9856_MICR_GAIN_PRV_       (MAX9856_MICR_GAIN_10DB_PRV_)

/* MAX9856 Register Address */
#define MAX9856_REGADR_00_STATUS_PRV_        (0x00u)
#define MAX9856_REGADR_01_STATUS_PRV_        (0x01u)
#define MAX9856_REGADR_02_INTEN_PRV_         (0x02u)
#define MAX9856_REGADR_03_CLKCTL_PRV_        (0x03u)
#define MAX9856_REGADR_04_DACIF_PRV_         (0x04u)
#define MAX9856_REGADR_05_DACIF_PRV_         (0x05u)
#define MAX9856_REGADR_06_DACIF_PRV_         (0x06u)
#define MAX9856_REGADR_07_ADCIF_PRV_         (0x07u)
#define MAX9856_REGADR_08_ADCIF_PRV_         (0x08u)
#define MAX9856_REGADR_09_ADCIF_PRV_         (0x09u)
#define MAX9856_REGADR_0A_ADCIF_PRV_         (0x0Au)
#define MAX9856_REGADR_0B_DIGFILT_PRV_       (0x0Bu)
#define MAX9856_REGADR_0C_AGCTL_PRV_         (0x0Cu)
#define MAX9856_REGADR_0D_AGCTL_PRV_         (0x0Du)
#define MAX9856_REGADR_0E_ANAMIX_PRV_        (0x0Eu)
#define MAX9856_REGADR_0F_ANAMIX_PRV_        (0x0Fu)
#define MAX9856_REGADR_10_ANAMIX_PRV_        (0x10u)
#define MAX9856_REGADR_11_AUDIOINP_PRV_      (0x11u)
#define MAX9856_REGADR_12_AUDIOINP_PRV_      (0x12u)
#define MAX9856_REGADR_13_AUDIOINP_PRV_      (0x13u)
#define MAX9856_REGADR_14_AUDIOINP_PRV_      (0x14u)
#define MAX9856_REGADR_15_AUDIOINP_PRV_      (0x15u)
#define MAX9856_REGADR_16_AUDIOINP_PRV_      (0x16u)
#define MAX9856_REGADR_17_AUDIOINP_PRV_      (0x17u)
#define MAX9856_REGADR_18_AUDIOOUT_PRV_      (0x18u)
#define MAX9856_REGADR_19_AUDIOOUT_PRV_      (0x19u)
#define MAX9856_REGADR_1A_AUDIOOUT_PRV_      (0x1Au)
#define MAX9856_REGADR_1B_HDETECT_PRV_       (0x1Bu)
#define MAX9856_REGADR_1C_POWMN_PRV_         (0x1Cu)

/* MAX9856 Register Bits */
/* R00: STATUS */
#define MAX9856_STATUS_BIT_GPI_PRV_          (0x01u <<0u)
#define MAX9856_STATUS_BIT_JDET_PRV_         (0x01u <<1u)
#define MAX9856_STATUS_BIT_HPOCR_PRV_        (0x01u <<2u)
#define MAX9856_STATUS_BIT_HPOCL_PRV_        (0x01u <<3u)
#define MAX9856_STATUS_BIT_JKMIC_PRV_        (0x01u <<4u)
#define MAX9856_STATUS_BIT_ULK_PRV_          (0x01u <<5u)
#define MAX9856_STATUS_BIT_SLD_PRV_          (0x01u <<6u)
#define MAX9856_STATUS_BIT_CLD_PRV_          (0x01u <<7u)

/* R01: STATUS */
#define MAX9856_STATUS_BIT_JSDET0_PRV_       (0x01u <<0u)
#define MAX9856_STATUS_BIT_JSDET1_PRV_       (0x01u <<1u)
#define MAX9856_STATUS_BIT_HSDETR0_PRV_      (0x01u <<2u)
#define MAX9856_STATUS_BIT_HSDETR1_PRV_      (0x01u <<3u)
#define MAX9856_STATUS_BIT_HSDETL0_PRV_      (0x01u <<4u)
#define MAX9856_STATUS_BIT_HSDETL1_PRV_      (0x01u <<5u)
#define MAX9856_STATUS_BIT_JKSNS_PRV_        (0x01u <<6u)
#define MAX9856_STATUS_BIT_LSNS_PRV_         (0x01u <<7u)

/* R02: INTEN */
#define MAX9856_INTEN_BIT_IGPI_PRV_          (0x01u <<0u)
#define MAX9856_INTEN_BIT_IJDET_PRV_         (0x01u <<1u)
#define MAX9856_INTEN_BIT_IHPOCR_PRV_        (0x01u <<2u)
#define MAX9856_INTEN_BIT_IHPOCL_PRV_        (0x01u <<3u)

#define MAX9856_INTEN_BIT_IULK_PRV_          (0x01u <<5u)
#define MAX9856_INTEN_BIT_ISLD_PRV_          (0x01u <<6u)
#define MAX9856_INTEN_BIT_ICLD_PRV_          (0x01u <<7u)

/* R03: CLOCK CONTROL */
#define MAX9856_CLKCTL_BIT_BSEL0_PRV_        (0x01u <<0u)
#define MAX9856_CLKCTL_BIT_BSEL1_PRV_        (0x01u <<1u)
#define MAX9856_CLKCTL_BIT_BSEL2_PRV_        (0x01u <<2u)
#define MAX9856_CLKCTL_BIT_MAS_PRV_          (0x01u <<3u)
#define MAX9856_CLKCTL_BIT_PSCLK0_PRV_       (0x01u <<4u)
#define MAX9856_CLKCTL_BIT_PSCLK1_PRV_       (0x01u <<5u)
#define MAX9856_CLKCTL_BIT_PSCLK2_PRV_       (0x01u <<6u)

/* R04: DAC INTERFACE */
#define MAX9856_DACIF_BIT_WS_PRV_            (0x01u <<0u)
#define MAX9856_DACIF_BIT_DHF_PRV_           (0x01u <<1u)
#define MAX9856_DACIF_BIT_PCM_PRV_           (0x01u <<2u)
#define MAX9856_DACIF_BIT_DDLY_PRV_          (0x01u <<3u)
#define MAX9856_DACIF_BIT_DRATE0_PRV_        (0x01u <<4u)
#define MAX9856_DACIF_BIT_DRATE1_PRV_        (0x01u <<5u)
#define MAX9856_DACIF_BIT_DBCI_PRV_          (0x01u <<6u)
#define MAX9856_DACIF_BIT_DWCI_PRV_          (0x01u <<7u)

/* R05: DAC INTERFACE */
#define MAX9856_DACIF_BIT_DACNI8_PRV_        (0x01u <<0u)
#define MAX9856_DACIF_BIT_DACNI9_PRV_        (0x01u <<1u)
#define MAX9856_DACIF_BIT_DACNI10_PRV_       (0x01u <<2u)
#define MAX9856_DACIF_BIT_DACNI11_PRV_       (0x01u <<3u)
#define MAX9856_DACIF_BIT_DACNI12_PRV_       (0x01u <<4u)
#define MAX9856_DACIF_BIT_DACNI13_PRV_       (0x01u <<5u)
#define MAX9856_DACIF_BIT_DACNI14_PRV_       (0x01u <<6u)
#define MAX9856_DACIF_BIT_DPLLEN_PRV_        (0x01u <<7u)

/* R06: DAC INTERFACE */
#define MAX9856_DACIF_BIT_DACNI0_PRV_        (0x01u <<0u)
#define MAX9856_DACIF_BIT_DACNI1_PRV_        (0x01u <<1u)
#define MAX9856_DACIF_BIT_DACNI2_PRV_        (0x01u <<2u)
#define MAX9856_DACIF_BIT_DACNI3_PRV_        (0x01u <<3u)
#define MAX9856_DACIF_BIT_DACNI4_PRV_        (0x01u <<4u)
#define MAX9856_DACIF_BIT_DACNI5_PRV_        (0x01u <<5u)
#define MAX9856_DACIF_BIT_DACNI6_PRV_        (0x01u <<6u)
#define MAX9856_DACIF_BIT_DACNI7_PRV_        (0x01u <<7u)

/* R07: ADC INTERFACE */
#define MAX9856_ADCIF_BIT_ADLY_PRV_          (0x01u <<3u)
#define MAX9856_ADCIF_BIT_APIN0_PRV_         (0x01u <<4u)
#define MAX9856_ADCIF_BIT_APIN1_PRV_         (0x01u <<5u)
#define MAX9856_ADCIF_BIT_ABCI_PRV_          (0x01u <<6u)
#define MAX9856_ADCIF_BIT_AWCI_PRV_          (0x01u <<7u)

/* R08: ADC INTERFACE */
#define MAX9856_ADCIF_BIT_ADCNI8_PRV_        (0x01u <<0u)
#define MAX9856_ADCIF_BIT_ADCNI9_PRV_        (0x01u <<1u)
#define MAX9856_ADCIF_BIT_ADCNI10_PRV_       (0x01u <<2u)
#define MAX9856_ADCIF_BIT_ADCNI11_PRV_       (0x01u <<3u)
#define MAX9856_ADCIF_BIT_ADCNI12_PRV_       (0x01u <<4u)
#define MAX9856_ADCIF_BIT_ADCNI13_PRV_       (0x01u <<5u)
#define MAX9856_ADCIF_BIT_ADCNI14_PRV_       (0x01u <<6u)
#define MAX9856_ADCIF_BIT_APLLEN_PRV_        (0x01u <<7u)

/* R09: ADC INTERFACE */
#define MAX9856_ADCIF_BIT_ADCNI0_PRV_        (0x01u <<0u)
#define MAX9856_ADCIF_BIT_ADCNI1_PRV_        (0x01u <<1u)
#define MAX9856_ADCIF_BIT_ADCNI2_PRV_        (0x01u <<2u)
#define MAX9856_ADCIF_BIT_ADCNI3_PRV_        (0x01u <<3u)
#define MAX9856_ADCIF_BIT_ADCNI4_PRV_        (0x01u <<4u)
#define MAX9856_ADCIF_BIT_ADCNI5_PRV_        (0x01u <<5u)
#define MAX9856_ADCIF_BIT_ADCNI6_PRV_        (0x01u <<6u)
#define MAX9856_ADCIF_BIT_ADCNI7_PRV_        (0x01u <<7u)

/* R0A: ADC INTERFACE */
#define MAX9856_ADCIF_BIT_ANTH0_PRV_         (0x01u <<0u)
#define MAX9856_ADCIF_BIT_ANTH1_PRV_         (0x01u <<1u)
#define MAX9856_ADCIF_BIT_ANTH2_PRV_         (0x01u <<2u)
#define MAX9856_ADCIF_BIT_ANTH3_PRV_         (0x01u <<3u)
#define MAX9856_ADCIF_BIT_AGAIN0_PRV_        (0x01u <<4u)
#define MAX9856_ADCIF_BIT_AGAIN1_PRV_        (0x01u <<5u)
#define MAX9856_ADCIF_BIT_AGAIN2_PRV_        (0x01u <<6u)
#define MAX9856_ADCIF_BIT_AGAIN3_PRV_        (0x01u <<7u)

/* R0B: DIGITAL FILTERS */
#define MAX9856_DIGFILT_BIT_DACHP0_PRV_      (0x01u <<0u)
#define MAX9856_DIGFILT_BIT_DACHP1_PRV_      (0x01u <<1u)
#define MAX9856_DIGFILT_BIT_DACHP2_PRV_      (0x01u <<2u)
#define MAX9856_DIGFILT_BIT_ADCHP0_PRV_      (0x01u <<4u)
#define MAX9856_DIGFILT_BIT_ADCHP1_PRV_      (0x01u <<5u)
#define MAX9856_DIGFILT_BIT_ADCHP2_PRV_      (0x01u <<6u)

/* R0C: AUTOMATIC GAIN CONTROL */
#define MAX9856_AGCTL_BIT_AGCHLD0_PRV_       (0x01u <<0u)
#define MAX9856_AGCTL_BIT_AGCHLD1_PRV_       (0x01u <<1u)
#define MAX9856_AGCTL_BIT_AGCATK0_PRV_       (0x01u <<2u)
#define MAX9856_AGCTL_BIT_AGCATK1_PRV_       (0x01u <<3u)
#define MAX9856_AGCTL_BIT_AGCRLS0_PRV_       (0x01u <<4u)
#define MAX9856_AGCTL_BIT_AGCRLS1_PRV_       (0x01u <<5u)
#define MAX9856_AGCTL_BIT_AGCRLS2_PRV_       (0x01u <<6u)

/* R0D: AUTOMATIC GAIN CONTROL */
#define MAX9856_AGCTL_BIT_AGCSTH0_PRV_       (0x01u <<0u)
#define MAX9856_AGCTL_BIT_AGCSTH1_PRV_       (0x01u <<1u)
#define MAX9856_AGCTL_BIT_AGCSTH2_PRV_       (0x01u <<2u)
#define MAX9856_AGCTL_BIT_AGCSTH3_PRV_       (0x01u <<3u)
#define MAX9856_AGCTL_BIT_AGCSRC_PRV_        (0x01u <<4u)

/* R0E: ANALOG MIXERS */
#define MAX9856_ANAMIX_BIT_MXINL0_PRV_       (0x01u <<0u)
#define MAX9856_ANAMIX_BIT_MXINL1_PRV_       (0x01u <<1u)
#define MAX9856_ANAMIX_BIT_MXINL2_PRV_       (0x01u <<2u)
#define MAX9856_ANAMIX_BIT_MXINL3_PRV_       (0x01u <<3u)
#define MAX9856_ANAMIX_BIT_MXINL4_PRV_       (0x01u <<4u)

/* R0F: ANALOG MIXERS */
#define MAX9856_ANAMIX_BIT_MXINR0_PRV_       (0x01u <<0u)
#define MAX9856_ANAMIX_BIT_MXINR1_PRV_       (0x01u <<1u)
#define MAX9856_ANAMIX_BIT_MXINR2_PRV_       (0x01u <<2u)
#define MAX9856_ANAMIX_BIT_MXINR3_PRV_       (0x01u <<3u)
#define MAX9856_ANAMIX_BIT_MXINR4_PRV_       (0x01u <<4u)

/* R10: ANALOG MIXERS */
#define MAX9856_ANAMIX_BIT_MXOUTR0_PRV_      (0x01u <<0u)
#define MAX9856_ANAMIX_BIT_MXOUTR1_PRV_      (0x01u <<1u)
#define MAX9856_ANAMIX_BIT_MXOUTR2_PRV_      (0x01u <<2u)
#define MAX9856_ANAMIX_BIT_MXOUTR3_PRV_      (0x01u <<3u)
#define MAX9856_ANAMIX_BIT_MXOUTL0_PRV_      (0x01u <<4u)
#define MAX9856_ANAMIX_BIT_MXOUTL1_PRV_      (0x01u <<5u)
#define MAX9856_ANAMIX_BIT_MXOUTL2_PRV_      (0x01u <<6u)
#define MAX9856_ANAMIX_BIT_MXOUTL3_PRV_      (0x01u <<7u)

/* R11: AUDIO INPUTS */
#define MAX9856_AUDIOINP_BIT_PGADS0_PRV_     (0x01u <<0u)
#define MAX9856_AUDIOINP_BIT_PGADS1_PRV_     (0x01u <<1u)
#define MAX9856_AUDIOINP_BIT_PGADS2_PRV_     (0x01u <<2u)
#define MAX9856_AUDIOINP_BIT_PGADS3_PRV_     (0x01u <<3u)
#define MAX9856_AUDIOINP_BIT_PGADS4_PRV_     (0x01u <<4u)
#define MAX9856_AUDIOINP_BIT_PGADS5_PRV_     (0x01u <<5u)
#define MAX9856_AUDIOINP_BIT_PGADS6_PRV_     (0x01u <<6u)
#define MAX9856_AUDIOINP_BIT_PGADS7_PRV_     (0x01u <<7u)
#define MAX9856_AUDIOINP_PGADS_MAX_PRV_      (0xE5u)

/* R12: AUDIO INPUTS */
#define MAX9856_AUDIOINP_BIT_PGAAUX0_PRV_    (0x01u <<0u)
#define MAX9856_AUDIOINP_BIT_PGAAUX1_PRV_    (0x01u <<1u)
#define MAX9856_AUDIOINP_BIT_PGAAUX2_PRV_    (0x01u <<2u)
#define MAX9856_AUDIOINP_BIT_PGAAUX3_PRV_    (0x01u <<3u)
#define MAX9856_AUDIOINP_BIT_PGAAUX4_PRV_    (0x01u <<4u)

/* R13: AUDIO INPUTS */
#define MAX9856_AUDIOINP_BIT_PGAL10_PRV_     (0x01u <<0u)
#define MAX9856_AUDIOINP_BIT_PGAL11_PRV_     (0x01u <<1u)
#define MAX9856_AUDIOINP_BIT_PGAL12_PRV_     (0x01u <<2u)
#define MAX9856_AUDIOINP_BIT_PGAL13_PRV_     (0x01u <<3u)
#define MAX9856_AUDIOINP_BIT_PGAL14_PRV_     (0x01u <<4u)
#define MAX9856_AUDIOINP_PGAL1_MAX_PRV_      (0x1Fu)

/* R14: AUDIO INPUTS */
#define MAX9856_AUDIOINP_BIT_PGAL20_PRV_     (0x01u <<0u)
#define MAX9856_AUDIOINP_BIT_PGAL21_PRV_     (0x01u <<1u)
#define MAX9856_AUDIOINP_BIT_PGAL22_PRV_     (0x01u <<2u)
#define MAX9856_AUDIOINP_BIT_PGAL23_PRV_     (0x01u <<3u)
#define MAX9856_AUDIOINP_BIT_PGAL24_PRV_     (0x01u <<4u)
#define MAX9856_AUDIOINP_PGAL2_MAX_PRV_      (0x1Fu)

/* R15: AUDIO INPUTS */
#define MAX9856_AUDIOINP_BIT_PGAML0_PRV_     (0x01u <<0u)
#define MAX9856_AUDIOINP_BIT_PGAML1_PRV_     (0x01u <<1u)
#define MAX9856_AUDIOINP_BIT_PGAML2_PRV_     (0x01u <<2u)
#define MAX9856_AUDIOINP_BIT_PGAML3_PRV_     (0x01u <<3u)
#define MAX9856_AUDIOINP_BIT_PGAML4_PRV_     (0x01u <<4u)
#define MAX9856_AUDIOINP_BIT_PAENL0_PRV_     (0x01u <<5u)
#define MAX9856_AUDIOINP_BIT_PAENL1_PRV_     (0x01u <<6u)

/* R16: AUDIO INPUTS */
#define MAX9856_AUDIOINP_BIT_PGAMR0_PRV_     (0x01u <<0u)
#define MAX9856_AUDIOINP_BIT_PGAMR1_PRV_     (0x01u <<1u)
#define MAX9856_AUDIOINP_BIT_PGAMR2_PRV_     (0x01u <<2u)
#define MAX9856_AUDIOINP_BIT_PGAMR3_PRV_     (0x01u <<3u)
#define MAX9856_AUDIOINP_BIT_PGAMR4_PRV_     (0x01u <<4u)
#define MAX9856_AUDIOINP_BIT_PAENR0_PRV_     (0x01u <<5u)
#define MAX9856_AUDIOINP_BIT_PAENR1_PRV_     (0x01u <<6u)

/* R17: AUDIO INPUTS */
#define MAX9856_AUDIOINP_BIT_LMICDIF_PRV_    (0x01u <<0u)
#define MAX9856_AUDIOINP_BIT_MBSEL_PRV_      (0x01u <<2u)
#define MAX9856_AUDIOINP_BIT_MMIC_PRV_       (0x01u <<3u)

/* R18: AUDIO OUTPUTS */
#define MAX9856_AUDIOOUT_BIT_HPVOLL0_PRV_    (0x01u <<0u)
#define MAX9856_AUDIOOUT_BIT_HPVOLL1_PRV_    (0x01u <<1u)
#define MAX9856_AUDIOOUT_BIT_HPVOLL2_PRV_    (0x01u <<2u)
#define MAX9856_AUDIOOUT_BIT_HPVOLL3_PRV_    (0x01u <<3u)
#define MAX9856_AUDIOOUT_BIT_HPVOLL4_PRV_    (0x01u <<4u)
#define MAX9856_AUDIOOUT_BIT_HPVOLL5_PRV_    (0x01u <<5u)
#define MAX9856_AUDIOOUT_BIT_HPMUTE_PRV_     (0x01u <<6u)

/* R19: AUDIO OUTPUTS */
#define MAX9856_AUDIOOUT_BIT_HPVOLR0_PRV_    (0x01u <<0u)
#define MAX9856_AUDIOOUT_BIT_HPVOLR1_PRV_    (0x01u <<1u)
#define MAX9856_AUDIOOUT_BIT_HPVOLR2_PRV_    (0x01u <<2u)
#define MAX9856_AUDIOOUT_BIT_HPVOLR3_PRV_    (0x01u <<3u)
#define MAX9856_AUDIOOUT_BIT_HPVOLR4_PRV_    (0x01u <<4u)
#define MAX9856_AUDIOOUT_BIT_HPVOLR5_PRV_    (0x01u <<5u)

/* R1A: AUDIO OUTPUTS */
#define MAX9856_AUDIOOUT_BIT_HPMODE0_PRV_    (0x01u <<0u)
#define MAX9856_AUDIOOUT_BIT_HPMODE1_PRV_    (0x01u <<1u)
#define MAX9856_AUDIOOUT_BIT_AUXMIX_PRV_     (0x01u <<4u)
#define MAX9856_AUDIOOUT_BIT_AUXDC_PRV_      (0x01u <<5u)
#define MAX9856_AUDIOOUT_BIT_VSEN_PRV_       (0x01u <<6u)

/* R1B: HEADSET DETECT */
#define MAX9856_HDETECT_BIT_EN0_PRV_         (0x01u <<0u)
#define MAX9856_HDETECT_BIT_EN1_PRV_         (0x01u <<1u)
#define MAX9856_HDETECT_BIT_EN2_PRV_         (0x01u <<2u)
#define MAX9856_HDETECT_BIT_JDETEN_PRV_      (0x01u <<3u)

/* R1C: POWER MANAGEMENT */
#define MAX9856_POWMN_BIT_ADREN_PRV_         (0x01u <<0u)
#define MAX9856_POWMN_BIT_ADLEN_PRV_         (0x01u <<1u)
#define MAX9856_POWMN_BIT_DAREN_PRV_         (0x01u <<2u)
#define MAX9856_POWMN_BIT_DALEN_PRV_         (0x01u <<3u)
#define MAX9856_POWMN_BIT_LOUTEN_PRV_        (0x01u <<4u)
#define MAX9856_POWMN_BIT_DIGEN_PRV_         (0x01u <<5u)
#define MAX9856_POWMN_BIT_SHDN_PRV_          (0x01u <<7u)

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef struct
{
    bool_t is_max9856_init;
} st_sound_max9856_ctrl_t;

/******************************************************************************
 Imported global variables and functions (from other files)
 ******************************************************************************/

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static int_t max9856_setup_default (void);
static int_t max9856_shutdown (void);
static uint32_t max9856_conv_vol_to_pgads (const uint32_t vol);
static uint32_t max9856_conv_vol_to_pgalx (uint32_t vol);

static st_sound_max9856_ctrl_t gs_sound_max9856_ctrl =
{
false /* is_max9856_init */
};

static const uint32_t gs_gb_vol_to_pgads_tbl[SOUND_VOL_MAX + 1] =
{
/* vol,  PGADS,   GAIN(dB) */
/*   0 */0xE5, /* Dummy(SOUND_VOL_MUTE) */
/*   1 */0xE5, /* -40.0(min) */
/*   2 */0xE2, /* -38.0 */
/*   3 */0xE0, /* -36.x */
/*   4 */0xDE, /* -35.x */
/*   5 */0xDB, /* -34.0 */
/*   6 */0xD9, /* -33.0 */
/*   7 */0xD7, /* -32.x */
/*   8 */0xD4, /* -31.0 */
/*   9 */0xD2, /* -30.0 */
/*  10 */0xD0, /* -29.x */
/*  11 */0xCD, /* -28.x */
/*  12 */0xCB, /* -27.x */
/*  13 */0xC9, /* -27.0 */
/*  14 */0xC6, /* -26.0 */
/*  15 */0xC4, /* -25.x */
/*  16 */0xC2, /* -25.0 */
/*  17 */0xBF, /* -24.0 */
/*  18 */0xBD, /* -23.x */
/*  19 */0xBB, /* -23.0 */
/*  20 */0xB9, /* -22.x */
/*  21 */0xB6, /* -21.x */
/*  22 */0xB4, /* -21.x */
/*  23 */0xB2, /* -20.x */
/*  24 */0xAF, /* -20.x */
/*  25 */0xAD, /* -19.x */
/*  26 */0xAB, /* -19.x */
/*  27 */0xA8, /* -18.x */
/*  28 */0xA6, /* -18.x */
/*  29 */0xA4, /* -17.x */
/*  30 */0xA1, /* -17.x */
/*  31 */0x9F, /* -17.0 */
/*  32 */0x9D, /* -16.x */
/*  33 */0x9A, /* -16.x */
/*  34 */0x98, /* -15.x */
/*  35 */0x96, /* -15.5 */
/*  36 */0x94, /* -15.x */
/*  37 */0x91, /* -14.x */
/*  38 */0x8F, /* -14.x */
/*  39 */0x8D, /* -14.0 */
/*  40 */0x8A, /* -13.5 */
/*  41 */0x88, /* -13.x */
/*  42 */0x86, /* -13.0 */
/*  43 */0x83, /* -12.5 */
/*  44 */0x81, /* -12.x */
/*  45 */0x7F, /* -12.0 */
/*  46 */0x7C, /* -11.5 */
/*  47 */0x7A, /* -11.x */
/*  48 */0x78, /* -11.0 */
/*  49 */0x75, /* -10.x */
/*  50 */0x73, /* -10.x */
/*  51 */0x71, /* -10.x */
/*  52 */0x6F, /* -09.x */
/*  53 */0x6C, /* -09.x */
/*  54 */0x6A, /* -09.x */
/*  55 */0x68, /* -09.x */
/*  56 */0x65, /* -08.x */
/*  57 */0x63, /* -08.5 */
/*  58 */0x61, /* -08.x */
/*  59 */0x5E, /* -08.0 */
/*  60 */0x5C, /* -07.x */
/*  61 */0x5A, /* -07.x */
/*  62 */0x57, /* -07.x */
/*  63 */0x55, /* -07.0 */
/*  64 */0x53, /* -06.x */
/*  65 */0x50, /* -06.5 */
/*  66 */0x4E, /* -06.x */
/*  67 */0x4C, /* -06.x */
/*  68 */0x4A, /* -06.0 */
/*  69 */0x47, /* -05.x */
/*  70 */0x45, /* -05.5 */
/*  71 */0x43, /* -05.x */
/*  72 */0x40, /* -05.0 */
/*  73 */0x3E, /* -04.x */
/*  74 */0x3C, /* -04.x */
/*  75 */0x39, /* -04.x */
/*  76 */0x37, /* -04.x */
/*  77 */0x35, /* -04.0 */
/*  78 */0x32, /* -03.x */
/*  79 */0x30, /* -03.x */
/*  80 */0x2E, /* -03.x */
/*  81 */0x2B, /* -03.x */
/*  82 */0x29, /* -03.0 */
/*  83 */0x27, /* -02.x */
/*  84 */0x25, /* -02.x */
/*  85 */0x22, /* -02.5 */
/*  86 */0x20, /* -02.x */
/*  87 */0x1E, /* -02.x */
/*  88 */0x1B, /* -01.x */
/*  89 */0x19, /* -01.x */
/*  90 */0x17, /* -01.x */
/*  91 */0x14, /* -01.x */
/*  92 */0x12, /* -01.x */
/*  93 */0x10, /* -01.x */
/*  94 */0x0D, /* -00.x */
/*  95 */0x0B, /* -00.x */
/*  96 */0x09, /* -00.x */
/*  97 */0x06, /* -00.x */
/*  98 */0x04, /* -00.x */
/*  99 */0x02, /* -00.x */
/* 100 */0x00 /* -00.0(SOUND_VOL_MAX) */
};

/******************************************************************************
 Exported global functions (to be accessed by other files)
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: sound_max9856_open
 * @brief
 *                Description:<br>
 *
 * @param         none
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
int_t sound_max9856_open (void)
{
    int_t ercd = DEVDRV_SUCCESS;
    bool_t is_init;

    /* check status */
    is_init = gs_sound_max9856_ctrl.is_max9856_init;
    if (false != is_init)
    {
        /* already opend */
        ercd = DEVDRV_ERROR;
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* setup defaults */
        ercd = max9856_setup_default();
    }

    /* update status */
    if (DEVDRV_SUCCESS == ercd)
    {
        gs_sound_max9856_ctrl.is_max9856_init = true;
    }

    return ercd;
}

/**************************************************************************//**
 * Function Name: sound_max9856_close
 * @brief
 *                Description:<br>
 *
 * @param         none
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
int_t sound_max9856_close (void)
{
    int_t ercd = DEVDRV_SUCCESS;
    bool_t is_init;

    /* check status */
    is_init = gs_sound_max9856_ctrl.is_max9856_init;
    if (false == is_init)
    {
        /* already closed */
        ercd = DEVDRV_ERROR;
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* Shutdown */
        ercd = max9856_shutdown();
    }

    /* update status */
    if (DEVDRV_SUCCESS == ercd)
    {
        gs_sound_max9856_ctrl.is_max9856_init = false;
    }

    return ercd;
}

/**************************************************************************//**
 * Function Name: sound_max9856_set_mute
 * @brief         Set to mute MAX9856 volume.
 *                Description:<br>
 *
 * @param         none
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
int_t sound_max9856_set_mute (void)
{
    int_t ercd = DEVDRV_SUCCESS;
    bool_t is_init;

    is_init = gs_sound_max9856_ctrl.is_max9856_init;
    if (false == is_init)
    {
        ercd = DEVDRV_ERROR;
    }

    /* set MXOUTL/MXOUTR */
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R10: Audio Mixer Control Registers */
        /* MXOUTL : 0000 - No input source selected */
        /* MXOUTR : 0000 - No input source selected */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_10_ANAMIX_PRV_, 0x00);
    }

    return ercd;
}

/**************************************************************************//**
 * Function Name: sound_max9856_set_mic_mute
 * @brief         Set to mute MAX9856 MIC volume.
 *                Description:<br>
 *
 * @param         none
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
int_t sound_max9856_set_mic_mute (void)
{
    int_t ercd = DEVDRV_SUCCESS;
    bool_t is_init;

    is_init = gs_sound_max9856_ctrl.is_max9856_init;
    if (false == is_init)
    {
        ercd = DEVDRV_ERROR;
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0E: Audio Mixer Control Registers */
        /* MXINL : 00000 - No input source selected */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0E_ANAMIX_PRV_, 0x00);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0F: Audio Mixer Control Registers */
        /* MXINR : 00000 - No input source selected */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0F_ANAMIX_PRV_, 0x00);
    }

    return ercd;
}

/**************************************************************************//**
 * Function Name: sound_max9856_set_volume
 * @brief         Set MAX9856 volume.
 *                Description:<br>
 *                Convert volume range from (1 to 100) to (0xE5 to 0x00)
 * @param[in]     lvol        : left  volume value 1(min) to 100(max)
 * @param[in]     rvol        : right volume value 1(min) to 100(max)
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
int_t sound_max9856_set_volume (const int32_t lvol, const int32_t rvol)
{
    int_t ercd = DEVDRV_SUCCESS;
    bool_t is_init;
    uint32_t pgads;

    is_init = gs_sound_max9856_ctrl.is_max9856_init;
    if (false == is_init)
    {
        ercd = DEVDRV_ERROR;
    }

    /* check param */
    if (DEVDRV_SUCCESS == ercd)
    {
        if (lvol != rvol)
        {
            ercd = DEVDRV_ERROR;
        }
    }

    /* convert vol param to programmable gain adjust for digital audio input */
    pgads = max9856_conv_vol_to_pgads((uint32_t) lvol);

    /* check volume range */
    if (DEVDRV_SUCCESS == ercd)
    {
        if (pgads > MAX9856_AUDIOINP_PGADS_MAX_PRV_)
        {
            ercd = DEVDRV_ERROR;
        }
    }

    /* set MXOUTL/MXOUTR */
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R10: Audio Mixer Control Registers */
        /* MXOUTL : 0001 - DAC output selected */
        /* MXOUTR : 0001 - DAC output selected */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_10_ANAMIX_PRV_,
                0 | MAX9856_ANAMIX_BIT_MXOUTL0_PRV_ | MAX9856_ANAMIX_BIT_MXOUTR0_PRV_);
    }

    /* set PGADS */
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R11: Audio Input Control Registers */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_11_AUDIOINP_PRV_, (uint8_t) pgads);
    }

    return (ercd);
}
/*******************************************************************************
 End of function sound_max9856_set_volume
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: sound_max9856_set_mic_volume
 * @brief         Set MAX9856 Line In volume.
 *                Description:<br>
 *                set PGAL1, PGAL2
 *                Convert volume range (1-100) to (0x1F-0)
 * @param[in]     lvol        : LINE INPUT 1 volume value 1 to 100(max)
 * @param[in]     rvol        : LINE INPUT 2 volume value 1 to 100(max)
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
int_t sound_max9856_set_mic_volume (const int32_t lvol, const int32_t rvol)
{
    int_t ercd = DEVDRV_SUCCESS;
    bool_t is_init;
    uint32_t pgal1_val;
    uint32_t pgal2_val;

    is_init = gs_sound_max9856_ctrl.is_max9856_init;
    if (false == is_init)
    {
        ercd = DEVDRV_ERROR;
    }

    /* convert volume value to programmable gain adjust for line input value */
    pgal1_val = max9856_conv_vol_to_pgalx((uint32_t) lvol);
    pgal2_val = max9856_conv_vol_to_pgalx((uint32_t) rvol);

    /* check volume range */
    if (DEVDRV_SUCCESS == ercd)
    {
        if ((pgal1_val > MAX9856_AUDIOINP_PGAL1_MAX_PRV_) || (pgal2_val > MAX9856_AUDIOINP_PGAL2_MAX_PRV_))
        {
            ercd = DEVDRV_ERROR;
        }
    }

    /*************** setup LINE IN volume ***************/
    /* set MXINL */
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0E: Audio Mixer Control Registers */
        /* For RSK+RZA1H : */
        /* MXINL : 01000 - LINEIN1 selected */
        /* For Stream It! RZA1LU : */
        /* MXINL : 00010 - MIC selected */
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0E_ANAMIX_PRV_, 0 | MAX9856_ANAMIX_BIT_MXINL1_PRV_); /* MIC input on stream it */
#else
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0E_ANAMIX_PRV_, 0 | MAX9856_ANAMIX_BIT_MXINL3_PRV_); /* LINEIN1 input on RSK */
#endif

    }

    /* set MXINR */
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0F: Audio Mixer Control Registers */
        /* For RSK+RZA1H : */
        /* MXINR : 01000 - LINEIN2 selected */
        /* For Stream It! RZA1LU : */
        /* MXINR : 00001 - MIC selected */
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0F_ANAMIX_PRV_, 0 | MAX9856_ANAMIX_BIT_MXINR0_PRV_); /* MIC input on stream it */
#else
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0F_ANAMIX_PRV_, 0 | MAX9856_ANAMIX_BIT_MXINR2_PRV_); /* LINEIN2 input on RSK */
#endif
    }

    /* set PGAL1 */
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R13: Audio Input Control Registers */
        /* For RSK+RZA1H : */
        /* PGAL1 :   pgal2_val Linein1 gain */
        /* For Stream It! RZA1LU : */
        /* PGAL1 :   0         LINE IN is disabled (uses MIC) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_13_AUDIOINP_PRV_, (uint8_t)pgal1_val);
    }

    /* set PGAL2 */
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R14: Audio Input Control Registers */
        /* For RSK+RZA1H : */
        /* PGAL2 :   pgal2_val Linein2 gain */
        /* For Stream It! RZA1LU : */
        /* PGAL2 :   0         LINE IN is disabled (uses MIC) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_14_AUDIOINP_PRV_, (uint8_t)pgal2_val);
    }

#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R15: Audio Input Control Registers */
        /* For RSK+RZA1H : */
        /* PGAENL :   00 - Left Microphone disabled (Default input is LINEIN) */
        /* PGAML  : 0x00 - ignored (Microphone disabled) */
        /* For Stream It! RZA1LU : */
        /* PGAENL : Microphone enable and Preamp Gain set by MAX9856_MICL_GAIN (Default input is MIC) */
        /* PGAML  : 0x00 - ignored (MIC gain is controlled by AGC) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_15_AUDIOINP_PRV_, MAX9856_MICL_GAIN_PRV_);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R16: Audio Input Control Registers */
        /* For RSK+RZA1H : */
        /* PGAENL :   00 - Right Microphone disabled (Default input is LINEIN) */
        /* PGAML  : 0x00 - ignored (Microphone disabled) */
        /* For Stream It! RZA1LU : */
        /* PGAENR : Microphone enable and Preamp Gain set by MAX9856_MICR_GAIN (Default input is MIC) */
        /* PGAMR  : 0x00 - ignored (MIC gain is controlled by AGC) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_16_AUDIOINP_PRV_, MAX9856_MICR_GAIN_PRV_);
    }
#endif
    return (ercd);
}
/*******************************************************************************
 End of function sound_max9856_set_mic_volume
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: sound_max9856_set_sampling_rate
 * @brief         Set MAX9856 sampling frequency
 *                Description:<br>
 *                MAX9856 features are DAC(max96kHz) and ADC(max48kHz).
 *                This board configured ADC and DAC are at the same sample rate.
 *                So maximum samplingrate is 48kHz.
 * @param[in]     smpfreq     : sampling rate(8k-48k)
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
int_t sound_max9856_set_sampling_rate (const uint32_t smpfreq)
{
    int_t ercd = DEVDRV_SUCCESS;
    bool_t is_init;

    is_init = gs_sound_max9856_ctrl.is_max9856_init;
    if (false == is_init)
    {
        ercd = DEVDRV_ERROR;
    }

    /* check smpfreq param */
    if (DEVDRV_SUCCESS == ercd)
    {
        switch (smpfreq)
        {
            case SOUND_FREQ_8000:

                /* fall through */
            case SOUND_FREQ_11025:

                /* fall through */
            case SOUND_FREQ_12000:

                /* fall through */
            case SOUND_FREQ_16000:

                /* fall through */
            case SOUND_FREQ_22050:

                /* fall through */
            case SOUND_FREQ_24000:

                /* fall through */
            case SOUND_FREQ_32000:

                /* fall through */
            case SOUND_FREQ_44100:

                /* fall through */
            case SOUND_FREQ_48000:

                /* do nothing */
            break;
            default:
            {
                ercd = DEVDRV_ERROR;
            }
            break;
        }
    }

    /* Nothing to do to write registers(when DPLLEN=0) */

    return ercd;
}
/*******************************************************************************
 End of function sound_max9856_set_sampling_rate
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: sound_max9856_set_audio_format
 * @brief         Set MAX9856 Audio interface format.
 *                Description:<br>
 *
 * @param[in]     fmt         : data format<br>
 *                              SOUND_FMT_I2S(0)<br>
 *                              SOUND_FMT_LEFT(2)
 * @param[in]     bits        : bit width<br>
 *                              SOUND_DWORD_16(16)<br>
 *                              SOUND_DWORD_18(18)
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
int_t sound_max9856_set_audio_format (const uint32_t fmt, const uint32_t bits)
{
    int_t ercd = DEVDRV_SUCCESS;
    bool_t is_init;
    uint32_t r4_dwci = 0u;
    uint32_t r4_ddly = 0u;
    uint32_t r4_ws = 0u;

    is_init = gs_sound_max9856_ctrl.is_max9856_init;
    if (false == is_init)
    {
        ercd = DEVDRV_ERROR;
    }

    /* check bits param and determine (R04:DWCI, R07:AWCI) value */
    /* check bits param and determine (R04:DDLY, R07:ADLY) value */
    if (DEVDRV_SUCCESS == ercd)
    {
        switch (fmt)
        {
            case SOUND_FMT_I2S:
            {
                /* Left-channel data is transmitted while LRCLK is low */
                r4_dwci = 0;
                /* The most significant bit of an audio word is valid
                 at the second BCLK edge after the LRCLK transition. */
                r4_ddly = MAX9856_DACIF_BIT_DDLY_PRV_;
            }
            break;
            case SOUND_FMT_LEFT:
            {
                /* Right-channel data is transmitted while LRCLK is low */
                r4_dwci = MAX9856_DACIF_BIT_DWCI_PRV_;

                /* The most significant bit of an audio word is valid
                 at the first BCLK edge after the LRCLK transition. */
                r4_ddly = 0;
            }
            break;
            default:
            {
                ercd = DEVDRV_ERROR;
            }
            break;
        }
    }

    /* check bits param and determine R04:WS value */
    if (DEVDRV_SUCCESS == ercd)
    {
        switch (bits)
        {
            case SOUND_DWORD_16:
            {
                r4_ws = 0;
            }
            break;

            case SOUND_DWORD_18:
            {
                r4_ws = MAX9856_DACIF_BIT_WS_PRV_;
            }
            break;

            default:
            {
                ercd = DEVDRV_ERROR;
            }
            break;
        }
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R04: DAC Interface Register */
        /* DWCI :  r4_dwci */
        /* DBCI :  0 - SDIN is accepted on the rising edge of BCLK */
        /* DRATE: 00 - Low-power mode */
        /* DDLY :  r4_ddly */
        /* PCM  :  0 - The LRCLK_D and LRCLK_A signals have a 50% duty cycle */
        /* DHF  :  0 - LRCLK_D is less than 50kHz */
        /* WS   :  r4_ws */

        /* cast to uint8_t to fit width of register */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_04_DACIF_PRV_,
                (0 | (uint8_t) r4_dwci | (uint8_t) r4_ddly | (uint8_t) r4_ws));
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R07: ADC Interface Register */
        /* AWCI :  same as r4_dwci */
        /* ABCI :  0 - SDOUT is valid on the rising edge of BCLK */
        /* APIN : 00 - LRCLK_A is not connected to SSIWK0 */
        /* ADLY :  same as r4_ddly */

        /* cast to fit width of register */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_07_ADCIF_PRV_, (0 | (uint8_t) r4_dwci | (uint8_t) r4_ddly));
    }

    return (ercd);
}
/*******************************************************************************
 End of function sound_max9856_set_audio_format
 ******************************************************************************/

/******************************************************************************
 Private functions
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: max9856_setup_default
 * @brief         Setup MAX9856 by defaults
 *                Description:<br>
 *
 * @param         none
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
static int_t max9856_setup_default (void)
{
    int_t ercd = DEVDRV_SUCCESS;

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R03: Clock Control Register */
        /* PSCLK: 011 - 20MHz <= (AUDIO_CLK:22.579200MHz) <= 32MHz */
        /* MAS  :   0 - Slave mode */
        /* BSEL : 000 - Off */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_03_CLKCTL_PRV_,
                0 | MAX9856_CLKCTL_BIT_PSCLK0_PRV_ | MAX9856_CLKCTL_BIT_PSCLK1_PRV_);
    }
    /************************************************************************
     * Caluculate 44.1kHz DACNI divider bits (only effective when DPLLEN=0)
     * 
     * MCLK = 22.5792MHz (AUDIO_CLK)
     * PCLK = MCLK/2 (PSCLK=011)
     * LRCLK_D frequency = 44.1kHz (DHF=0)
     * DACNI = (65536 x 96 x fLRCLK_D) / (fPCLK)
     *       = (65536 x 96 x 44100) / (22579200 / 2)
     *       = (65536 x 96 x 441) / (118296)
     *       = 23454 = 0x5B9E
     *
     ************************************************************************/

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R04: DAC Interface Register */
        /* DWCI :  0 - Left-channel data is transmitted while LRCLK_D is low */
        /* DBCI :  0 - SDIN is accepted on the rising edge of BCLK */
        /* DRATE: 00 - Low-power mode */
        /* DDLY :  1 - for I2S compatible mode */
        /* PCM  :  0 - The LRCLK_D and LRCLK_A signals have a 50% duty cycle */
        /* DHF  :  0 - LRCLK_D is less than 50kHz */
        /* WS   :  0 - Word size 16 bits */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_04_DACIF_PRV_, 0 | MAX9856_DACIF_BIT_DDLY_PRV_);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R05: DAC Interface Register */
        /* DPLLEN      : 1 - A digital PLL locks on to any externally supplied LRCLK_D signal */
        /* DACNI[14:8] : 0 - ignore (DAC LRCLK Divider disabled) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_05_DACIF_PRV_, 0 | MAX9856_DACIF_BIT_DPLLEN_PRV_);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R06: DAC Interface Register */
        /* DACNI[7:0] : 0 - ignore (DAC LRCLK Divider disabled) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_06_DACIF_PRV_, 0x00);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R07: ADC Interface Register */
        /* AWCI :  0 - Left-channel data is transmitted while LRCLK_D is low */
        /* ABCI :  0 - SDOUT is valid on the rising edge of BCLK */
        /* APIN : 00 - LRCLK_A is not connected to SSIWK0 */
        /* ADLY :  1 - for I2S compatible mode */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_07_ADCIF_PRV_, 0 | MAX9856_ADCIF_BIT_ADLY_PRV_);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R08: ADC Interface Register */
        /* APLLEN      : 1 - A digital PLL locks on to any externally supplied LRCLK_A signal */
        /* ADCNI[14:8] : 0 - ignore (ADC LRCLK Divider disabled) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_08_ADCIF_PRV_, 0 | MAX9856_ADCIF_BIT_APLLEN_PRV_);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R09: ADC Interface Register */
        /* ADCNI[7:0] : 0 - ignore (ADC LRCLK Divider disabled) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_09_ADCIF_PRV_, 0x00);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0A: ADC Interface Register */
        /* AGAIN : 0000 - Gain +3dB */
        /* ANTH  : 0000 - ADC Noise Gate Threashold Disabled */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0A_ADCIF_PRV_, 0x30);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0B: Digital Highpass Filters */
        /* ADCHP : 0000 - off */
        /* DACHP : 0000 - off */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0B_DIGFILT_PRV_, 0x00);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0C: Automatic Gain Control Registers */
        /* AGCRLS : 010 - 312ms */
        /* AGCATK :  10 - 50ms */
        /* AGCHLD :  10 - 100ms */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0C_AGCTL_PRV_,
                0 | MAX9856_AGCTL_BIT_AGCRLS1_PRV_ | MAX9856_AGCTL_BIT_AGCATK1_PRV_ | MAX9856_AGCTL_BIT_AGCHLD1_PRV_);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0D: Automatic Gain Control Registers */
        /* AGCSRC :    0 - Left-channel ADC output */
        /* AGCSTH : 0000 - -3dB */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0D_AGCTL_PRV_, 0x00);
    }

    /* set MXINL */
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0E: Audio Mixer Control Registers */
        /* For RSK+RZA1H : */
        /* MXINL : 01000 - LINEIN1 selected */
        /* For Stream It! RZA1LU : */
        /* MXINL : 00010 - MIC selected */
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0E_ANAMIX_PRV_, 0 | MAX9856_ANAMIX_BIT_MXINL1_PRV_); /* MIC input on stream it */
#else
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0E_ANAMIX_PRV_, 0 | MAX9856_ANAMIX_BIT_MXINL3_PRV_); /* LINEIN1 input on RSK */
#endif
    }

    /* set MXINR */
    if (DEVDRV_SUCCESS == ercd)
    {
        /* R0F: Audio Mixer Control Registers */
        /* For RSK+RZA1H : */
        /* MXINR : 01000 - LINEIN2 selected */
        /* For Stream It! RZA1LU : */
        /* MXINR : 00001 - MIC selected */
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0F_ANAMIX_PRV_, 0 | MAX9856_ANAMIX_BIT_MXINR0_PRV_); /* MIC input on stream it */
#else
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_0F_ANAMIX_PRV_, 0 | MAX9856_ANAMIX_BIT_MXINR2_PRV_); /* LINEIN2 input on RSK */
#endif
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R10: Audio Mixer Control Registers */
        /* MXOUTL : 0001 - DAC output selected */
        /* MXOUTR : 0001 - DAC output selected */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_10_ANAMIX_PRV_,
                0 | MAX9856_ANAMIX_BIT_MXOUTL0_PRV_ | MAX9856_ANAMIX_BIT_MXOUTR0_PRV_);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R11: Audio Input Control Registers */
        /* PGADS : 0x00 - Programmable Gain Adjust for Digital Audio Input is 0dB */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_11_AUDIOINP_PRV_, 0x00);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R12: Audio Input Control Registers */
        /* PGAAUX : 0x00 - AUX INPUT PGA settings +30dB */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_12_AUDIOINP_PRV_, 0x00);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R13: Audio Input Control Registers */
        /* PGAL1 : 0x00 - LINEIN1 INPUT PGA settings +30dB */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_13_AUDIOINP_PRV_, 0x00);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R14: Audio Input Control Registers */
        /* PGAL2 : 0x00 - LINEIN2 INPUT PGA settings +30dB */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_14_AUDIOINP_PRV_, 0x00);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R15: Audio Input Control Registers */
        /* For RSK+RZA1H : */
        /* PGAENL :   00 - Left Microphone disabled (Default input is LINEIN) */
        /* PGAML  : 0x00 - ignored (Microphone disabled) */
        /* For Stream It! RZA1LU : */
        /* PGAENL :   00 - Microphone disabled (Default input is MIC but initialise as OFF) */
        /* PGAML  : 0x00 - ignored (MIC gain is controlled by AGC) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_15_AUDIOINP_PRV_, 0x3F);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R16: Audio Input Control Registers */
        /* For RSK+RZA1H : */
        /* PGAENL :   00 - Right Microphone disabled (Default input is LINEIN) */
        /* PGAML  : 0x00 - ignored (Microphone disabled) */
        /* For Stream It! RZA1LU : */
        /* PGAENR : 00  - Microphone disabled (Default input is MIC but initialise as OFF) */
        /* PGAMR  : 0x00 - ignored (MIC gain is controlled by AGC) */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_16_AUDIOINP_PRV_, 0x3F);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R17: Audio Input Control Registers */
        /* MMIC    : 0 - Microphone Mute Disabled */
        /* MBSEL   : 0 - MICBIAS Voltage 1.5V */
        /* LMICDIF : 1 - Left Microphone Input Select is differential input */
#if (TARGET_BOARD == TARGET_BOARD_STREAM_IT2)
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_17_AUDIOINP_PRV_, 0);
#else
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_17_AUDIOINP_PRV_, 0 | MAX9856_AUDIOINP_BIT_MMIC_PRV_ | MAX9856_AUDIOINP_BIT_LMICDIF_PRV_);
#endif
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R18: Audio Output Control Registers */
        /* HPMUTE :    0 - Headphone Mute disable */
        /* HPVOLL : 0x09 - +0dB Gain */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_18_AUDIOOUT_PRV_, 0x09);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R19: Audio Output Control Registers */
        /* HPVOLR : 0x09 - +0B Gain */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_19_AUDIOOUT_PRV_, 0x09);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R1A: Audio Output Control Registers */
        /* VSEN   :  0 - */
        /* AUXDC  :  0 - */
        /* AUXMIX :  0 - */
        /* HPMODE : 11 - Stereo Mode */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_1A_AUDIOOUT_PRV_, 0x03);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R1B: Headset Detect Control Register */
        /* JDETEN :   0 - Jack Detection Disable */
        /* EN     : 000 - Disabled */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_1B_HDETECT_PRV_, 0x00);
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R1C: Power-Management Register */
        /* SHDN   : 1 - Clear shutdown mode */
        /* DIGEN  : 1 - Digital Core Enable */
        /* LOUTEN : 1 - Line Output Enable */
        /* DALEN  : 1 - Left  DAC Enable */
        /* DAREN  : 1 - Right DAC Enable */
        /* ADLEN  : 1 - Left  ADC Enable */
        /* ADREN  : 1 - Right ADC Enable */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_1C_POWMN_PRV_,
                MAX9856_POWMN_BIT_SHDN_PRV_ | MAX9856_POWMN_BIT_DIGEN_PRV_ | MAX9856_POWMN_BIT_LOUTEN_PRV_
                        | MAX9856_POWMN_BIT_DALEN_PRV_ | MAX9856_POWMN_BIT_DAREN_PRV_ | MAX9856_POWMN_BIT_ADLEN_PRV_
                        | MAX9856_POWMN_BIT_ADREN_PRV_);
    }

    return (ercd);
}
/*******************************************************************************
 End of function max9856_setup_default
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: max9856_shutdown
 * @brief         Shutdown MAX9856
 *                Description:<br>
 *
 * @param         none
 * @retval        DEVDRV_SUCCESS    : Success
 * @retval        error code  : Failure
 ******************************************************************************/
static int_t max9856_shutdown (void)
{
    int_t ercd = DEVDRV_SUCCESS;
    bool_t is_init;

    is_init = gs_sound_max9856_ctrl.is_max9856_init;
    if (false == is_init)
    {
        ercd = DEVDRV_ERROR;
    }

    if (DEVDRV_SUCCESS == ercd)
    {
        /* R1C: Power-Management Register */
        /* SHDN   : 0 - Set shutdown mode */
        /* DIGEN  : 0 - Digital Core Disable */
        /* LOUTEN : 0 - Line Output Disable */
        /* DALEN  : 0 - Left  DAC Disable */
        /* DAREN  : 0 - Right DAC Disable */
        /* ADLEN  : 0 - Left  ADC Disable */
        /* ADREN  : 0 - Right ADC Disable */
        ercd = RIIC_MAX9856_Write(MAX9856_REGADR_1C_POWMN_PRV_, 0x00);
    }

    return (ercd);
}
/*******************************************************************************
 End of function max9856_shutdown
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: max9856_conv_vol_to_pgads
 * @brief         Calculate PGADS value
 *
 *                Description:<br>
 *
 * @param[in]     vol        : vol value 1(min) to 100(max)
 * @retval        PGADS      : 0xE5(-40dB) to 0x00(0dB)
 ******************************************************************************/
static uint32_t max9856_conv_vol_to_pgads (const uint32_t vol)
{
    uint32_t pgads = gs_gb_vol_to_pgads_tbl[vol];

    return (pgads);
}
/*******************************************************************************
 End of function max9856_conv_vol_to_pgads
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: max9856_conv_vol_to_pgalx
 * @brief         Calculate PGALx value
 *
 *                Description:<br>
 *
 * @param[in]     vol     : 1(min) to 100(max)
 * @retval        PGALx   : 0x1F(-32dB) to 0x0(+30dB)
 ******************************************************************************/
static uint32_t max9856_conv_vol_to_pgalx (const uint32_t vol)
{
    /* (vol - 1) = 0 to 99 */
    /* ((SOUND_VOL_MAX - 1) - (vol - 1)) = 99 - (0 to 99) */

    const uint32_t pgal =
            ((MAX9856_AUDIOINP_PGAL1_MAX_PRV_ * ((SOUND_VOL_MAX - 1u) - (vol - 1u))) / (SOUND_VOL_MAX - 1));

    return (pgal);
}
/*******************************************************************************
 End of function max9856_conv_vol_to_pgalx
 ******************************************************************************/
