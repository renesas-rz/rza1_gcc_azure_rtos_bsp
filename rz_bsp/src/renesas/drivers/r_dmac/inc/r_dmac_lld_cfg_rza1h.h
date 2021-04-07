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
 ******************************************************************************/
/*******************************************************************************
 * @file         r_dmac_lld_cfg_rza1h.h
 * @version      1.0
 * @description  DMAC Low layer device driver definitions
 ******************************************************************************
 * History      : DD.MM.YYYY Version Description
 *              : 31.08.2018 1.00    First Release
 ******************************************************************************/

#include <limits.h>

#ifndef R_DMAC_LLD_CFG_RZA1H_H_
#define R_DMAC_LLD_CFG_RZA1H_H_

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* This define specifies what channels are supported by this driver */
#define DMAC_CFG_LLD_SUPPORTED_CHANNELS     (R_CH0 | R_CH1 | R_CH2 | R_CH3 | R_CH4 | R_CH5 | R_CH6 | R_CH7 | R_CH8 | R_CH9 | R_CH10 | R_CH11 | R_CH12 | R_CH13 | R_CH14 | R_CH15)
#define DMAC_NUMBER_OF_CHANNELS             (16)


typedef enum
{
	DMA_RS_OSTM0TINT,
	DMA_RS_OSTM1TINT,
	DMA_RS_TGIA_0,
	DMA_RS_TGIA_1,
	DMA_RS_TGIA_2,
	DMA_RS_TGIA_3,
	DMA_RS_TGIA_4,
	DMA_RS_SCIF_TXI0,
	DMA_RS_SCIF_RXI0,
	DMA_RS_SCIF_TXI1,
	DMA_RS_SCIF_RXI1,
	DMA_RS_SCIF_TXI2,
	DMA_RS_SCIF_RXI2,
	DMA_RS_SCIF_TXI3,
	DMA_RS_SCIF_RXI3,
	DMA_RS_SCIF_TXI4,
	DMA_RS_SCIF_RXI4,
	DMA_RS_SCIF_TXI5,
	DMA_RS_SCIF_RXI5,
	DMA_RS_SCIF_TXI6,
	DMA_RS_SCIF_RXI6,
	DMA_RS_SCIF_TXI7,
	DMA_RS_SCIF_RXI7,
	DMA_RS_USB0_DMA0_TX,
	DMA_RS_USB0_DMA0_RX,
	DMA_RS_USB0_DMA1_TX,
	DMA_RS_USB0_DMA1_RX,
	DMA_RS_ADI,
	DMA_RS_IEBBTD,
	DMA_RS_IEBBTV,
	DMA_RS_IREADY,
	DMA_RS_FLDT_TX,                         /*!< NAND controller */
	DMA_RS_FLDT_RX,
	DMA_RS_SDHI_0_TX,
	DMA_RS_SDHI_0_RX,
	DMA_RS_SDHI_1_TX,
	DMA_RS_SDHI_1_RX,
	DMA_RS_MMC_TX,
	DMA_RS_MMC_RX,
	DMA_RS_SSITXI0,
	DMA_RS_SSIRXI0,
	DMA_RS_SSITXI1,
	DMA_RS_SSIRXI1,
	DMA_RS_SSIRTI2,
	DMA_RS_SSITXI3,
	DMA_RS_SSIRXI3,
    DMA_RS_SSIRTI4,
    DMA_RS_SSITXI5,
    DMA_RS_SSIRXI5,
	DMA_RS_SCUTXI0,
	DMA_RS_SCURXI0,
	DMA_RS_SCUTXI1,
	DMA_RS_SCURXI1,
	DMA_RS_SCUTXI2,
	DMA_RS_SCURXI2,
	DMA_RS_SCUTXI3,
	DMA_RS_SCURXI3,
	DMA_RS_SPTI0,
	DMA_RS_SPRI0,
	DMA_RS_SPTI1,
	DMA_RS_SPRI1,
	DMA_RS_SPTI2,
	DMA_RS_SPRI2,
    DMA_RS_SPTI3,
    DMA_RS_SPRI3,
    DMA_RS_SPTI4,
    DMA_RS_SPRI4,
	DMA_RS_SPDIFTXI,
	DMA_RS_SPDIFRXI,
	DMA_RS_CMI1,
	DMA_RS_CMI2,
	DMA_RS_MLB_CINT_W,
	DMA_RS_MLB_CINT_R,
	DMA_RS_SGDEI0,
	DMA_RS_SGDEI1,
	DMA_RS_SGDEI2,
	DMA_RS_SGDEI3,
	DMA_RS_TXI0,
	DMA_RS_RXI0,
	DMA_RS_TXI1,
	DMA_RS_RXI1,
	DMA_RS_INTRIIC_TI0,
	DMA_RS_INTRIIC_RI0,
	DMA_RS_INTRIIC_TI1,
	DMA_RS_INTRIIC_RI1,
	DMA_RS_INTRIIC_TI2,
	DMA_RS_INTRIIC_RI2,
	DMA_RS_INTRIIC_TI3,
	DMA_RS_INTRIIC_RI3,
	DMA_RS_LIN0_INT_T,
	DMA_RS_LIN0_INT_R,
    DMA_RS_LIN1_INT_T,
    DMA_RS_LIN1_INT_R,
    DMA_RS_IFEI0,
    DMA_RS_OFFI0,
    DMA_RS_IFEI1,
    DMA_RS_OFFI1,
    DREQ0,                                  /*!< External Request */

    DMA_DRV_MEM_2_MEM,                      /*!< memory to memory */

    LAST_RESOURCE_MARKER,
    MAKE_ENUM_32BIT = INT_MAX               /*!< force enum to 32-bit type for struct alignment */
} e_r_drv_dmac_xfer_resource_t;

#endif /* R_DMAC_LLD_CFG_RZA1H_H_ */
