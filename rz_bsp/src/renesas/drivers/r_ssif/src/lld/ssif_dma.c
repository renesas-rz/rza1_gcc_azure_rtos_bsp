/******************************************************************************
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
 * Copyright (C) 2013-2017 Renesas Electronics Corporation.
 * All rights reserved.
 *****************************************************************************/

/******************************************************************************
 * File Name   : ssif_dma.c
 * Description : SSIF driver DMA functions
 *****************************************************************************/

/******************************************************************************
 Includes <System Includes>, "Project Includes"
 *****************************************************************************/
#include "mcu_board_select.h"
#include "ssif.h"
#include "ssif_int.h"
#include "fcntl.h"
#include "Renesas_RZ_A1.h"
#include "r_dmac_drv_api.h"

/******************************************************************************
 Macro definitions
 *****************************************************************************/
#define SSIF_DUMMY_DMA_BUF_SIZE_PRV_        (4096u)

/*
 * The common multiple of 2, 4, 6, 8, 12, 24 and 32
 * which are all sampling sizes
 */
#define SSIF_DUMMY_DMA_TRN_SIZE_PRV_        (4032u)

#if (TARGET_RZA1 <= TARGET_RZA1LU)
#else /* TARGET_RZA1H */
#define SSIF_ROMDEC_DMA_SIZE_PRV_           (2352u)
#endif /* (TARGET_RZA1 <= TARGET_RZA1LU) */

/******************************************************************************
 Typedef definitions
 *****************************************************************************/

/******************************************************************************
 Exported global variables (to be accessed by other files)
 *****************************************************************************/

/******************************************************************************
 Private global variables and functions
 *****************************************************************************/

static int_t enable_ssif_transfer(ssif_info_ch_t * const p_info_ch);
static int_t open_dma_driver(ssif_info_ch_t * const p_info_ch);
static void SSIF_DMA_TxCallback(void);
static void SSIF_DMA_RxCallback(void);

static const e_r_drv_dmac_xfer_resource_t
        s_ssif_dma_tx_resource[SSIF_NUM_CHANS] =
{
    DMA_RS_SSITXI0,
    DMA_RS_SSITXI1,
#if (TARGET_RZA1 <= TARGET_RZA1LU)
	DMA_RS_SSITXI2,
    DMA_RS_SSITXI3
#else /* TARGET_RZA1H */
	DMA_RS_SSIRTI2,
    DMA_RS_SSITXI3,
    DMA_RS_SSIRTI4,
    DMA_RS_SSITXI5
#endif /* (TARGET_RZA1 <= TARGET_RZA1LU) */
};

static const e_r_drv_dmac_xfer_resource_t
        s_ssif_dma_rx_resource[SSIF_NUM_CHANS] =
{
#if (TARGET_RZA1 <= TARGET_RZA1LU)
    DMA_RS_SSIRXI0,
    DMA_RS_SSIRXI1,
    DMA_RS_SSIRXI2,
    DMA_RS_SSIRXI3
#else /* TARGET_RZA1H */
    DMA_RS_SSIRXI0,
    DMA_RS_SSIRXI1,
	DMA_RS_SSIRTI2,
    DMA_RS_SSIRXI3,
    DMA_RS_SSIRTI4,
    DMA_RS_SSIRXI5
#endif /* (TARGET_RZA1 <= TARGET_RZA1LU) */
};

static AIOCB s_ssif_dma_tx_end_aiocb[SSIF_NUM_CHANS];
static AIOCB s_ssif_dma_rx_end_aiocb[SSIF_NUM_CHANS];

static st_r_drv_dmac_next_transfer_t s_ssif_txdma_dummy_trparam[SSIF_NUM_CHANS];
static st_r_drv_dmac_next_transfer_t s_ssif_rxdma_dummy_trparam[SSIF_NUM_CHANS];

static uint32_t s_ssif_tx_dummy_buf[SSIF_DUMMY_DMA_BUF_SIZE_PRV_ / sizeof(uint32_t)];
static uint32_t s_ssif_rx_dummy_buf[SSIF_DUMMY_DMA_BUF_SIZE_PRV_ / sizeof(uint32_t)];

static int_t s_dma_driver_write_handle = (-1);
static int_t s_dma_driver_read_handle = (-1);

static ssif_info_ch_t * sp_info_ch;

/******************************************************************************
 * Function Name: SSIF_InitDMA
 * @brief         Allocate and Setup DMA_CH for specified SSIF channel
 * @param[in,out] p_info_ch: channel object
 * @retval        IOIF_ESUCCESS: Success
 * @retval        error code: Failure
 *****************************************************************************/
int_t SSIF_InitDMA(ssif_info_ch_t * const p_info_ch)
{
    int_t ercd = IOIF_ESUCCESS;
    int_t dma_ret = 0;
    uint32_t ssif_ch;
    st_r_drv_dmac_config_t dma_config;
    int_t result;
    st_r_drv_dmac_next_transfer_t next_transfer;

    if (NULL == p_info_ch)
    {
        return (IOIF_EFAULT);
    }

    if (open_dma_driver(p_info_ch) != IOIF_ESUCCESS)
    {
        return (IOIF_EFAULT);
    }

    sp_info_ch = p_info_ch;
    ssif_ch = p_info_ch->channel;

    /* setup DMA channel for write (if necessary) */
    if (O_RDONLY != p_info_ch->openflag)
    {
        AIOCB * const p_tx_aio = &s_ssif_dma_tx_end_aiocb[ssif_ch];

        p_tx_aio->aio_sigevent.sigev_notify = 0;
        p_tx_aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
        p_tx_aio->aio_sigevent.sigev_value.sival_ptr = (void *) p_info_ch;
    }

    /* setup DMA channel for read (if necessary) */
    if (O_WRONLY != p_info_ch->openflag)
    {
        AIOCB * const p_rx_aio = &s_ssif_dma_rx_end_aiocb[ssif_ch];

        p_rx_aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
        p_rx_aio->aio_sigevent.sigev_value.sival_ptr = (void *) p_info_ch;
    }

    /* start DMA dummy transfer for write (if necessary) */
    if (O_RDONLY != p_info_ch->openflag)
    {
        /* setup short dummy transfer */
        s_ssif_txdma_dummy_trparam[ssif_ch].source_address = (void *) &s_ssif_tx_dummy_buf[0];
        s_ssif_txdma_dummy_trparam[ssif_ch].destination_address = (void *) &g_ssireg[ssif_ch]->SSIFTDR;
        s_ssif_txdma_dummy_trparam[ssif_ch].count = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

        dma_config.config.resource = s_ssif_dma_tx_resource[ssif_ch];                                    /* DMA transfer resource */
        dma_config.config.source_width = DMA_DATA_SIZE_4;                                                /* DMA transfer unit size (source) - 32 bits */
        dma_config.config.destination_width = DMA_DATA_SIZE_4;                                           /* DMA transfer unit size (destination) - 32 bits */
        dma_config.config.source_address_type = DMA_ADDRESS_INCREMENT;                                   /* DMA address type (source) */
        dma_config.config.destination_address_type = DMA_ADDRESS_FIX;                                    /* DMA address type (destination) */
        dma_config.config.direction = DMA_REQUEST_SOURCE;                                                /* DMA transfer direction will be set by the driver */
        dma_config.config.source_address = s_ssif_txdma_dummy_trparam[ssif_ch].source_address;           /* Source Address */
        dma_config.config.destination_address = s_ssif_txdma_dummy_trparam[ssif_ch].destination_address; /* Destination Address */
        dma_config.config.count = s_ssif_txdma_dummy_trparam[ssif_ch].count;                             /* length */
        dma_config.config.p_dmaComplete = SSIF_DMA_TxCallback;                                           /* set callback function (DMA end interrupt) */

        result = control(s_dma_driver_write_handle, CTL_DMAC_SET_CONFIGURATION, (void *) &dma_config);

        if (DRV_SUCCESS == result)
        {
            result = control(s_dma_driver_write_handle, CTL_DMAC_ENABLE, NULL);
        }

        if (DRV_SUCCESS == result)
        {
            next_transfer.source_address      = (void *) &s_ssif_tx_dummy_buf[0];
            next_transfer.destination_address = (void *) &g_ssireg[ssif_ch]->SSIFTDR;
            next_transfer.count               = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

            result = control(s_dma_driver_write_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
        }

        dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

        if (IOIF_EERROR == dma_ret)
        {
            ercd = IOIF_EFAULT;
        }
    }

    /* start DMA dummy transfer for read (if necessary) */
    if (IOIF_ESUCCESS == ercd)
    {
        if (O_WRONLY != p_info_ch->openflag)
        {
#if (TARGET_RZA1 <= TARGET_RZA1LU)
            /* setup short dummy transfer */
            s_ssif_rxdma_dummy_trparam[ssif_ch].source_address = (void *) &g_ssireg[ssif_ch]->SSIFRDR;
            s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address = (void *) &s_ssif_rx_dummy_buf[0];
            s_ssif_rxdma_dummy_trparam[ssif_ch].count = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

#else /* TARGET_RZA1H */
            if (SSIF_CFG_ENABLE_ROMDEC_DIRECT != p_info_ch->romdec_direct.mode)
            {
                /* setup short dummy transfer */
                s_ssif_rxdma_dummy_trparam[ssif_ch].source_address = (void *) &g_ssireg[ssif_ch]->SSIFRDR;
                s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address = (void *) &s_ssif_rx_dummy_buf[0];
                s_ssif_rxdma_dummy_trparam[ssif_ch].count = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;
            }
            else
            {
                /* setup ROMDEC direct input transfer */
                s_ssif_rxdma_dummy_trparam[ssif_ch].source_address = (void *) &g_ssireg[ssif_ch]->SSIFRDR;
                s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address = (void *) &ROMDEC.STRMDIN0;
                s_ssif_rxdma_dummy_trparam[ssif_ch].count = SSIF_ROMDEC_DMA_SIZE_PRV_;
            }
#endif /* (TARGET_RZA1 <= TARGET_RZA1LU) */

            st_r_drv_dmac_next_transfer_t next_data;

            next_data.source_address      = s_ssif_rxdma_dummy_trparam[ssif_ch].source_address;
            next_data.destination_address = s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address;
            next_data.count               = s_ssif_rxdma_dummy_trparam[ssif_ch].count;

            result = control(s_dma_driver_read_handle, CTL_DMAC_NEXT_TRANSFER, &next_data);

            dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

            if (IOIF_EERROR == dma_ret)
            {
                ercd = IOIF_EFAULT;
            }
            else
            {
                dma_config.config.resource = s_ssif_dma_rx_resource[ssif_ch];                                    /* DMA transfer resource */
                dma_config.config.source_width = DMA_DATA_SIZE_4;                                                /* DMA transfer unit size (source) - 32 bits */
                dma_config.config.destination_width = DMA_DATA_SIZE_4;                                           /* DMA transfer unit size (destination) - 32 bits */
                dma_config.config.source_address_type = DMA_ADDRESS_FIX;                                         /* DMA address type (source) */
                dma_config.config.destination_address_type = DMA_ADDRESS_INCREMENT;                              /* DMA address type (destination) */
                dma_config.config.direction = DMA_REQUEST_SOURCE;                                                /* DMA transfer direction will be set by the driver */
                dma_config.config.source_address = s_ssif_rxdma_dummy_trparam[ssif_ch].source_address;           /* Source Address */
                dma_config.config.destination_address = s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address; /* Destination Address */
                dma_config.config.count = s_ssif_rxdma_dummy_trparam[ssif_ch].count;                             /* length */
                dma_config.config.p_dmaComplete = SSIF_DMA_RxCallback;                                           /* set callback function (DMA end interrupt) */

                result = control(s_dma_driver_read_handle, CTL_DMAC_SET_CONFIGURATION, (void *) &dma_config);

                if (DRV_SUCCESS == result)
                {
                    result = control(s_dma_driver_read_handle, CTL_DMAC_ENABLE, NULL);
                }

                if (DRV_SUCCESS == result)
                {
                    next_transfer.source_address      = (void *) &g_ssireg[ssif_ch]->SSIFRDR;
                    next_transfer.destination_address = (void *) &s_ssif_rx_dummy_buf[0];
                    next_transfer.count               = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

                    result = control(s_dma_driver_read_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
                }

                dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

                if (IOIF_EERROR == dma_ret)
                {
                    ercd = IOIF_EFAULT;
                }
            }
        }
    }

    /* enable SSIF transfer */
    if (IOIF_ESUCCESS == ercd)
    {
        ercd = enable_ssif_transfer(p_info_ch);
    }

    /* cleanup DMA resources when error occurred */
    if (IOIF_ESUCCESS != ercd)
    {
        SSIF_CancelDMA(p_info_ch);
    }

    return (ercd);
}
/******************************************************************************
 End of function SSIF_InitDMA
 *****************************************************************************/

/******************************************************************************
 * Function Name: SSIF_UnInitDMA
 * @brief         Free DMA_CH for specified SSIF channel
 * @param[in,out] p_info_ch: channel object
 * @retval        none
 *****************************************************************************/
void SSIF_UnInitDMA(ssif_info_ch_t * const p_info_ch)
{
    uint32_t remain;

    if (NULL == p_info_ch)
    {
        return;
    }

    if (s_dma_driver_write_handle >= 0)
    {
        control(s_dma_driver_write_handle, CTL_DMAC_DISABLE, &remain);
    }

    if (s_dma_driver_read_handle >= 0)
    {
        control(s_dma_driver_read_handle, CTL_DMAC_DISABLE, &remain);
    }
}
/******************************************************************************
 End of function SSIF_UnInitDMA
 *****************************************************************************/

/******************************************************************************
 * Function Name: SSIF_RestartDMA
 * @brief         Setup DMA_CH for specified SSIF channel (without allocate)
 * @param[in,out] p_info_ch: channel object
 * @retval        IOIF_ESUCCESS: Success
 * @retval        error code: Failure
 *****************************************************************************/
int_t SSIF_RestartDMA(ssif_info_ch_t* const p_info_ch)
{
    int_t ercd = IOIF_ESUCCESS;
    int_t dma_ret;
    uint32_t ssif_ch;
    st_r_drv_dmac_config_t dma_config;
    int_t result;
    st_r_drv_dmac_next_transfer_t next_transfer;

    if (NULL == p_info_ch)
    {
        return (IOIF_EFAULT);
    }

    ssif_ch = p_info_ch->channel;

    /* setup DMA channel for write (if necessary) */
    if (O_RDONLY != p_info_ch->openflag)
    {
        AIOCB * const p_tx_aio = &s_ssif_dma_tx_end_aiocb[ssif_ch];
        p_tx_aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
        p_tx_aio->aio_sigevent.sigev_value.sival_ptr = (void *) p_info_ch;
    }

    /* setup DMA channel for read (if necessary) */
    if (O_WRONLY != p_info_ch->openflag)
    {
        AIOCB * const p_rx_aio = &s_ssif_dma_rx_end_aiocb[ssif_ch];
        p_rx_aio->aio_sigevent.sigev_notify = SIGEV_THREAD;
        p_rx_aio->aio_sigevent.sigev_value.sival_ptr = (void *) p_info_ch;
    }

    /* start DMA dummy transfer for write (if necessary) */
    if (O_RDONLY != p_info_ch->openflag)
    {
        /* setup short dummy transfer */
        s_ssif_txdma_dummy_trparam[ssif_ch].source_address = (void *) &s_ssif_tx_dummy_buf[0];
        s_ssif_txdma_dummy_trparam[ssif_ch].destination_address = (void *) &g_ssireg[ssif_ch]->SSIFTDR;
        s_ssif_txdma_dummy_trparam[ssif_ch].count = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

        dma_config.config.resource = s_ssif_dma_tx_resource[ssif_ch];                                    /* DMA transfer resource */
        dma_config.config.source_width = DMA_DATA_SIZE_4;                                                /* DMA transfer unit size (source) - 32 bits */
        dma_config.config.destination_width = DMA_DATA_SIZE_4;                                           /* DMA transfer unit size (destination) - 32 bits */
        dma_config.config.source_address_type = DMA_ADDRESS_INCREMENT;                                   /* DMA address type (source) */
        dma_config.config.destination_address_type = DMA_ADDRESS_FIX;                                    /* DMA address type (destination) */
        dma_config.config.direction = DMA_REQUEST_SOURCE;                                                /* DMA transfer direction will be set by the driver */
        dma_config.config.source_address = s_ssif_txdma_dummy_trparam[ssif_ch].source_address;           /* Source Address */
        dma_config.config.destination_address = s_ssif_txdma_dummy_trparam[ssif_ch].destination_address; /* Destination Address */
        dma_config.config.count = s_ssif_txdma_dummy_trparam[ssif_ch].count;                             /* length */
        dma_config.config.p_dmaComplete = SSIF_DMA_TxCallback;                                           /* set callback function (DMA end interrupt) */

        result = control(s_dma_driver_write_handle, CTL_DMAC_SET_CONFIGURATION, (void *) &dma_config);

        if (DRV_SUCCESS == result)
        {
            result = control(s_dma_driver_write_handle, CTL_DMAC_ENABLE, NULL);
        }

        if (DRV_SUCCESS == result)
        {
            next_transfer.source_address      = (void *) &s_ssif_tx_dummy_buf[0];
            next_transfer.destination_address = (void *) &g_ssireg[ssif_ch]->SSIFTDR;
            next_transfer.count               = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

            result = control(s_dma_driver_write_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
        }

        dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

        if (IOIF_EERROR == dma_ret)
        {
            ercd = IOIF_EFAULT;
        }
    }

    /* start DMA dummy transfer for read (if necessary) */
    if (IOIF_ESUCCESS == ercd)
    {
        if (O_WRONLY != p_info_ch->openflag)
        {
            /* setup short dummy transfer */
            s_ssif_rxdma_dummy_trparam[ssif_ch].source_address = (void *) &g_ssireg[ssif_ch]->SSIFRDR;
            s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address = (void *) &s_ssif_rx_dummy_buf[0];
            s_ssif_rxdma_dummy_trparam[ssif_ch].count = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

            dma_config.config.resource = s_ssif_dma_rx_resource[ssif_ch];                                    /* DMA transfer resource */
            dma_config.config.source_width = DMA_DATA_SIZE_4;                                                /* DMA transfer unit size (source) - 32 bits */
            dma_config.config.destination_width = DMA_DATA_SIZE_4;                                           /* DMA transfer unit size (destination) - 32 bits */
            dma_config.config.source_address_type = DMA_ADDRESS_FIX;                                         /* DMA address type (source) */
            dma_config.config.destination_address_type = DMA_ADDRESS_INCREMENT;                              /* DMA address type (destination) */
            dma_config.config.direction = DMA_REQUEST_SOURCE;                                                /* DMA transfer direction will be set by the driver */
            dma_config.config.source_address = s_ssif_rxdma_dummy_trparam[ssif_ch].source_address;           /* Source Address */
            dma_config.config.destination_address = s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address; /* Destination Address */
            dma_config.config.count = s_ssif_rxdma_dummy_trparam[ssif_ch].count;                             /* length */
            dma_config.config.p_dmaComplete = SSIF_DMA_RxCallback;                                           /* set callback function (DMA end interrupt) */

            result = control(s_dma_driver_read_handle, CTL_DMAC_SET_CONFIGURATION, (void *) &dma_config);

            if (DRV_SUCCESS == result)
            {
                result = control(s_dma_driver_read_handle, CTL_DMAC_ENABLE, NULL);
            }

            if (DRV_SUCCESS == result)
            {
                next_transfer.source_address      = (void *) &g_ssireg[ssif_ch]->SSIFRDR;
                next_transfer.destination_address = (void *) &s_ssif_rx_dummy_buf[0];
                next_transfer.count               = SSIF_DUMMY_DMA_TRN_SIZE_PRV_;

                result = control(s_dma_driver_read_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
            }

            dma_ret = (DRV_SUCCESS == result) ? IOIF_ESUCCESS : IOIF_EERROR;

            if (IOIF_EERROR == dma_ret)
            {
                ercd = IOIF_EFAULT;
            }
        }
    }

    /* enable SSIF transfer */
    if (IOIF_ESUCCESS == ercd)
    {
        ercd = enable_ssif_transfer(p_info_ch);
    }

    return (ercd);
}
/******************************************************************************
 End of function SSIF_RestartDMA
 *****************************************************************************/

/******************************************************************************
 * Function Name: SSIF_CancelDMA
 * @brief         Pause DMA transfer for specified SSIF channel
 * @param[in,out] p_info_ch: channel object
 * @retval        none
 *****************************************************************************/
void SSIF_CancelDMA(const ssif_info_ch_t * const p_info_ch)
{
    uint32_t remain;

    if (NULL == p_info_ch)
    {
        return;
    }

    if (s_dma_driver_write_handle >= 0)
    {
        control(s_dma_driver_write_handle, CTL_DMAC_DISABLE, &remain);
    }

    if (s_dma_driver_read_handle >= 0)
    {
        control(s_dma_driver_read_handle, CTL_DMAC_DISABLE, &remain);
    }
}
/******************************************************************************
 End of function SSIF_CancelDMA
 *****************************************************************************/

/******************************************************************************
 Private functions
 *****************************************************************************/

/******************************************************************************
 * Function Name: enable_ssif_transfer
 * @brief         Start the SSIF DMA transfer for read and / or write
 * @param[in,out] p_info_ch: channel object
 * @retval        IOIF_ESUCCESS: Success
 * @retval        error code: Failure
 *****************************************************************************/
static int_t enable_ssif_transfer(ssif_info_ch_t * const p_info_ch)
{
    /* clear status and enable error interrupt */
    SSIF_EnableErrorInterrupt(p_info_ch->channel);

    /* enable end interrupt */
    g_ssireg[p_info_ch->channel]->SSIFCR |= (SSIF_FCR_BIT_TIE | SSIF_FCR_BIT_RIE);

    if (O_RDWR == p_info_ch->openflag)
    {
        /* start write and read DMA at the same time */
        g_ssireg[p_info_ch->channel]->SSICR |= (SSIF_CR_BIT_TEN | SSIF_CR_BIT_REN);
    }
    else if (O_WRONLY == p_info_ch->openflag)
    {
        /* start write DMA only */
        g_ssireg[p_info_ch->channel]->SSICR |= SSIF_CR_BIT_TEN;
    }
    else if (O_RDONLY == p_info_ch->openflag)
    {
        /* start read DMA only */
        g_ssireg[p_info_ch->channel]->SSICR |= SSIF_CR_BIT_REN;
    }
    else
    {
        return (IOIF_EINVAL);
    }

    return (IOIF_ESUCCESS);
}
/******************************************************************************
 End of function enable_ssif_transfer
 *****************************************************************************/

/******************************************************************************
 * Function Name: open_dma_driver
 * @brief         Open the DMA driver for read and / or write
 * @param[in,out] p_info_ch: channel object
 * @retval        IOIF_ESUCCESS: Success
 * @retval        error code: Failure
 *****************************************************************************/
static int_t open_dma_driver(ssif_info_ch_t * const p_info_ch)
{
    /* open DMA driver for write (if necessary) */
    if (O_RDONLY == p_info_ch->openflag)
    {
        p_info_ch->dma_tx_ch = -1;
    }
    else
    {
        /* close the driver handle if already open */
        if ((-1) != s_dma_driver_write_handle)
        {
            close(s_dma_driver_write_handle);
        }

        s_dma_driver_write_handle = open(DEVICE_INDENTIFIER "dma_ssif_wr", O_WRONLY);

        /* we failed to open the DMA driver */
        if (s_dma_driver_write_handle < 0)
        {
            return (IOIF_EFAULT);
        }
    }

    /* open DMA driver for read (if necessary) */
    if (O_WRONLY == p_info_ch->openflag)
    {
        p_info_ch->dma_rx_ch = -1;
    }
    else
    {
        /* close the driver handle if already open */
        if ((-1) != s_dma_driver_read_handle)
        {
            close(s_dma_driver_read_handle);
        }

        s_dma_driver_read_handle = open(DEVICE_INDENTIFIER "dma_ssif_rd", O_RDONLY);

        /* we failed to open the DMA driver */
        if (s_dma_driver_read_handle < 0)
        {
            return (IOIF_EFAULT);
        }
    }

    return (IOIF_ESUCCESS);
}
/******************************************************************************
 End of function open_dma_driver
 *****************************************************************************/

/******************************************************************************
 * Function Name: SSIF_DMA_TxCallback
 * @brief         DMA callback function
 * @retval        none
 *****************************************************************************/
static void SSIF_DMA_TxCallback(void)
{
    ssif_info_ch_t * const p_info_ch = sp_info_ch;
    uint32_t ssif_ch;
    st_r_drv_dmac_next_transfer_t next_transfer;

    if (NULL == p_info_ch)
    {
        return;
    }

    ssif_ch = p_info_ch->channel;

    if (NULL != p_info_ch->p_aio_tx_curr)
    {
        /* now complete user request transfer, Signal to application */

        /* return aio complete */
        p_info_ch->p_aio_tx_curr->aio_return = (ssize_t) p_info_ch->p_aio_tx_curr->aio_nbytes;
        ahf_complete(&p_info_ch->tx_que, p_info_ch->p_aio_tx_curr);
    }

    /* copy next to curr (even if it's NULL) */
    p_info_ch->p_aio_tx_curr = p_info_ch->p_aio_tx_next;

    /* get next request (it could be NULL) */
    p_info_ch->p_aio_tx_next = ahf_removehead(&p_info_ch->tx_que);

    if (NULL != p_info_ch->p_aio_tx_next)
    {
        next_transfer.source_address      = (void *) p_info_ch->p_aio_tx_next->aio_buf;
        next_transfer.destination_address = (void *) &g_ssireg[ssif_ch]->SSIFTDR;
        next_transfer.count               = (uint32_t) p_info_ch->p_aio_tx_next->aio_nbytes;
    }
    else
    {
        next_transfer.source_address      = (void *) s_ssif_txdma_dummy_trparam[ssif_ch].source_address;
        next_transfer.destination_address = (void *) s_ssif_txdma_dummy_trparam[ssif_ch].destination_address;
        next_transfer.count               = (uint32_t) s_ssif_txdma_dummy_trparam[ssif_ch].count;
    }

    control(s_dma_driver_write_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
}
/******************************************************************************
 End of function SSIF_DMA_TxCallback
 *****************************************************************************/

/******************************************************************************
 * Function Name: SSIF_DMA_RxCallback
 * @brief         DMA callback function
 * @retval        none
 *****************************************************************************/
static void SSIF_DMA_RxCallback(void)
{
    ssif_info_ch_t * const p_info_ch = sp_info_ch;
    uint32_t ssif_ch;
    st_r_drv_dmac_next_transfer_t next_transfer;

    if (NULL == p_info_ch)
    {
        return;
    }

    ssif_ch = p_info_ch->channel;

    if (NULL != p_info_ch->p_aio_rx_curr)
    {
        /* now complete user request transfer, Signal to application */

        /* return aio complete */
        p_info_ch->p_aio_rx_curr->aio_return = (ssize_t) p_info_ch->p_aio_rx_curr->aio_nbytes;
        ahf_complete(&p_info_ch->rx_que, p_info_ch->p_aio_rx_curr);
    }

    /* copy next to curr (even if it's NULL) */
    p_info_ch->p_aio_rx_curr = p_info_ch->p_aio_rx_next;

    /* get next request (it could be NULL) */
    p_info_ch->p_aio_rx_next = ahf_removehead(&p_info_ch->rx_que);

    if (NULL != p_info_ch->p_aio_rx_next)
    {
        next_transfer.source_address      = (void *) &g_ssireg[ssif_ch]->SSIFRDR;
        next_transfer.destination_address = (void *) p_info_ch->p_aio_rx_next->aio_buf;
        next_transfer.count               = (uint32_t) p_info_ch->p_aio_rx_next->aio_nbytes;
    }
    else
    {
        next_transfer.source_address      = (void *) s_ssif_rxdma_dummy_trparam[ssif_ch].source_address;
        next_transfer.destination_address = (void *) s_ssif_rxdma_dummy_trparam[ssif_ch].destination_address;
        next_transfer.count               = (uint32_t) s_ssif_rxdma_dummy_trparam[ssif_ch].count;
    }

    control(s_dma_driver_read_handle, CTL_DMAC_NEXT_TRANSFER, &next_transfer);
}
/******************************************************************************
 End of function SSIF_DMA_RxCallback
 *****************************************************************************/
