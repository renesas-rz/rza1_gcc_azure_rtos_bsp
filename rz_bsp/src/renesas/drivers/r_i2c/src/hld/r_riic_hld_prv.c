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
 * File Name     : r_riic_api.c
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream it! v2 board
 * Description   : RIIC driver support for all 4 available i2c channels
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 21.10.2014 1.00
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/

#include "dev_drv.h"            /* Device Driver common header */
#include "r_riic_hld_prv.h"     /* RIIC High level Driver Header */
#include "r_riic_api.h"         /* defines and lld include */
#include "r_riic_drv_link.h"    /* Link include to low level driver */

/*******************************************************************************
 Macro definitions
 *******************************************************************************/
/* IIC data read/write bit position */
#define SAMPLE_RIIC_RWCODE_PRV_        (0x01)


/* defines for channel enable status */
#define CHANNEL_ENABLED_PRV_    (1)
#define CHANNEL_DISABLED_PRV_   (0)

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef enum riic_rx_mode
{
    RIIC_RX_MODE_NORMAL, RIIC_RX_MODE_ACK, RIIC_RX_MODE_LOW_HOLD, RIIC_RX_MODE_NACK, RIIC_RX_MODE_STOP
} e_riic_rx_mode_t;

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static int_t validate_channel (int_t channel);
static int_t riic_single_write (int_t channel, uint8_t data, uint32_t mode);
static int_t riic_write (int_t channel, const uint8_t *pbuffer, uint32_t byte);
static int_t riic_read (int_t channel, uint8_t *pbuffer, uint32_t byte);
///static int_t riic_read_dummy (int_t channel);
static int_t riic_receive (int_t channel, e_riic_rx_mode_t mode, uint8_t *pdata);
static int_t riic_transmit (int_t channel, uint8_t data);
static int_t transmit_start (int_t channel);
static int_t transmit_restart (int_t channel);

/* wait functions for completion of i2c operations */
static int_t wait_rx_full (int_t channel);
static int_t wait_tx_empty (int_t channel);
static int_t wait_tx_end (int_t channel, uint32_t mode);
static int_t wait_bus_master (int_t channel, uint32_t mode);
static int_t wait_stop (int_t channel);

/* static array variable holding initialisation status of each low level channel */
static int_t gs_channel_initialise_state[RIIC_LLD_NUM_CHANNELS];

/* static variable holding overall initialisation status of all i2c channels */
static int_t gs_overall_initialise_state = 0u;

/* static array variables holding flag statuses for each channel */
static volatile uint8_t gs_riic_receive_full_flg[RIIC_LLD_NUM_CHANNELS];
static volatile uint8_t gs_riic_transmit_empty_flg[RIIC_LLD_NUM_CHANNELS];
static volatile uint8_t gs_riic_transmit_end_flg[RIIC_LLD_NUM_CHANNELS];

/**
 * @brief open the specified i2c peripheral channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    DEVDRV_SUCCESS       : Success of RIIC operation
 *            DEVDRV_ERROR         : Failure of RIIC operation
 *            DEVDRV_ERROR_TIMEOUT : IIC operation timed out
 **/
int_t open_channel (int_t channel, st_r_drv_riic_create_t *p_channel_config)
{
    uint32_t loop;

    /* ensure that channel is valid */
    int_t ret = validate_channel(channel);

    if (DEVDRV_SUCCESS == ret)
    {
        /* Initialise flags if this is the first entry */
        if (CHANNEL_ENABLED_PRV_ != gs_overall_initialise_state)
        {
            /* initialise flags */
            for (loop = 0; loop < RIIC_LLD_NUM_CHANNELS; loop++)
            {
                gs_channel_initialise_state[loop] = CHANNEL_DISABLED_PRV_;
            }

            gs_overall_initialise_state = CHANNEL_ENABLED_PRV_;
        }

        /* only initialise if not already done so */
        if (CHANNEL_DISABLED_PRV_ == gs_channel_initialise_state[channel])
        {
            /* call to low level driver to turn channel on */
            ret = R_RIIC_InitChannel(channel, p_channel_config->frequency);

            if (DEVDRV_SUCCESS == ret)
            {
                /* Initialise flags */
                gs_riic_receive_full_flg[channel] = DEVDRV_FLAG_OFF;
                gs_riic_transmit_empty_flg[channel] = DEVDRV_FLAG_OFF;
                gs_riic_transmit_end_flg[channel] = DEVDRV_FLAG_OFF;

                /* mark channel as initialised */
                gs_channel_initialise_state[channel] = CHANNEL_ENABLED_PRV_;
            }
        }
        else
        {
            /* channel is already open */
            ret = DEVDRV_ERROR;
        }
    }

    /* return status */
    return (ret);
}
/*******************************************************************************
 End of function open_channel
 ******************************************************************************/

/**
 * @brief Close the specified i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void close_channel (int_t channel)
{
    uint32_t loop;
    uint32_t count = 0u;

    /* ensure that channel is valid */
    int_t ret = validate_channel(channel);

    if (DEVDRV_SUCCESS == ret)
    {
        /* mark channel as un-initialised */
        gs_channel_initialise_state[channel] = CHANNEL_DISABLED_PRV_;

        /* call to low level driver to turn channel off */
        R_RIIC_CloseChannel(channel);

        /* test if any channels are still active */
        for (loop = 0; loop < RIIC_LLD_NUM_CHANNELS; loop++)
        {
            if (CHANNEL_DISABLED_PRV_ != gs_channel_initialise_state[loop])
            {
                count++;
            }
        }

        /* If no channels are open, set the overall initialisation as zero */
        if (0u == count)
        {
            gs_overall_initialise_state = CHANNEL_DISABLED_PRV_;
        }
    }
}
/*******************************************************************************
 End of function close_channel
 ******************************************************************************/

/**
 * @brief Determine if the specified i2c channel is valid
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    DEVDRV_SUCCESS       : Success of RIIC operation
 *            DEVDRV_ERROR         : Failure of RIIC operation
 **/
static int_t validate_channel (int_t channel)
{
    int_t ret_value = DEVDRV_ERROR;

    /* test if the channel is present */
    if (0 != ((1 << channel) & RIIC_LLD_SUPPORTED_CHANNELS))
    {
        ret_value = DEVDRV_SUCCESS;
    }

    /* return status */
    return (ret_value);
}
/*******************************************************************************
 End of function validate_channel
 ******************************************************************************/

/**
 * @brief Writes 1 byte only of data to specified I2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] data    : data byte
 * @param[in] mode    : RIIC_TEND_WAIT_TRANSMIT : Transmission Mode
 *                    : RIIC_TEND_WAIT_RECEIVE  : Receive Mode
 * @return    DEVDRV_SUCCESS         : Success of RIIC operation
 *            DEVDRV_ERROR           : Failure of RIIC operation
 *            DEVDRV_ERROR_TIMEOUT   : RIIC operation timed out
 **/
static int_t riic_single_write (int_t channel, uint8_t data, uint32_t mode)
{
    int_t ret;

    /* Write single byte */
    ret = riic_transmit(channel, data);

    if (DEVDRV_SUCCESS == ret)
    {
        /* wait for end of transmission */
        ret = wait_tx_end(channel, mode);

        if (DEVDRV_SUCCESS == ret)
        {
            /* Check for transmission status */
            if (0u == R_RIIC_GetAckStatus(channel))
            {
                /* ACK */
                ret = DEVDRV_SUCCESS;
            }
            else
            {
                /* NACK */
                ret = DEVDRV_ERROR;
            }
        }
    }

    return (ret);
}
/******************************************************************************
 * End of Function riic_single_write
 ******************************************************************************/

/**
 * @brief Writes data to an already addressed device
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] pbuffer : pointer to allocated space for data
 * @param[in] byte    : number of bytes to receive
 * @return    DEVDRV_SUCCESS         : Success of RIIC operation
 *            DEVDRV_ERROR           : Failure of RIIC operation
 *            DEVDRV_ERROR_RIIC_NACK : Failure of RIIC transmission due to
 *                                     NACK reception
 *            DEVDRV_ERROR_TIMEOUT   : RIIC operation timed out
 **/
static int_t riic_write (int_t channel, const uint8_t *pbuffer, uint32_t byte)
{
    uint32_t offset;
    int_t ret = DEVDRV_SUCCESS;

    if (0 < byte)
    {
        for (offset = 0; offset < byte; offset++)
        {
            /* Single byte transmission */
            ret = riic_transmit(channel, *(pbuffer + offset));

            if (DEVDRV_SUCCESS != ret)
            {
                /* Exit the loop */
                offset = byte;
            }
        }

        if (DEVDRV_SUCCESS == ret)
        {
            ret = wait_tx_end(channel, RIIC_TEND_WAIT_TRANSMIT);
            if (DEVDRV_SUCCESS == ret)
            {
                /* clear flag */
                gs_riic_transmit_end_flg[channel] = DEVDRV_FLAG_OFF;

                /* Check for transmission status */
                /* ACK */
                if (0u == R_RIIC_GetAckStatus(channel))
                {
                    ret = DEVDRV_SUCCESS;
                }

                /* NACK */
                else
                {
                    ret = DEVDRV_ERROR_RIIC_NACK;
                }
            }
        }
    }

    /* return status */
    return (ret);
}
/******************************************************************************
 * End of Function riic_write
 ******************************************************************************/

/**
 * @brief Reads data from an already addressed device
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] pbuffer : pointer to allocated space for data
 * @param[in] byte    : number of bytes to receive
 * @return    DEVDRV_SUCCESS       : Success of RIIC operation
 *            DEVDRV_ERROR         : Failure of RIIC operation
 *            DEVDRV_ERROR_TIMEOUT : RIIC operation timed out
 **/
static int_t riic_read (int_t channel, uint8_t *pbuffer, uint32_t byte)
{
    uint8_t *p_buffer2;
    int_t ret;

    /* Ensure at least 1 byte is specified to receive */
    if (0 == byte)
    {
        return (DEVDRV_SUCCESS);
    }

    p_buffer2 = pbuffer;

    /* 1 byte random read does not need a dummy read */
    if (1 == byte)
    {
        /* (In the case of only 1 byte) reception start */
        ret = riic_receive(channel, RIIC_RX_MODE_NACK, p_buffer2);

        if (DEVDRV_SUCCESS != ret)
        {
            return (ret);
        }

        /* Last byte read */
        ret = riic_receive(channel, RIIC_RX_MODE_STOP, p_buffer2);

        if (DEVDRV_SUCCESS != ret)
        {
            return (ret);
        }
    }
    else
    {
        /* Start reading data */
        /* dummy read for read trigger, p_buffer2 is written by next data */
        if (2 == byte)
        {
            /* set wait mode on read of the (last - 2) byte */
            ret = riic_receive(channel, RIIC_RX_MODE_LOW_HOLD, p_buffer2);
        }
        else
        {
            /* normal start trigger */
            ret = riic_receive(channel, RIIC_RX_MODE_ACK, p_buffer2);
        }

        if (DEVDRV_SUCCESS != ret)
        {
            return (ret);
        }

        /* Reads data bytes */
        while (2 < byte)
        {
            if (3 == byte)
            {
                /* set wait mode on read of the (last - 2) byte */
                ret = riic_receive(channel, RIIC_RX_MODE_LOW_HOLD, p_buffer2);
            }
            else
            {
                /* request size is 3byte or more */
                ret = riic_receive(channel, RIIC_RX_MODE_ACK, p_buffer2);
            }

            if (DEVDRV_SUCCESS != ret)
            {
                return (ret);
            }

            p_buffer2++;
            byte--;
        }

        /* transfer NACK on read of the (last - 1) byte */
        ret = riic_receive(channel, RIIC_RX_MODE_NACK, p_buffer2);
        if (DEVDRV_SUCCESS != ret)
        {
            return (ret);
        }

        p_buffer2++;
        byte--;

        if (DEVDRV_SUCCESS == ret)
        {
            /* Data read of the last byte */
            ret = riic_receive(channel, RIIC_RX_MODE_STOP, p_buffer2);
        }
    }

    if (DEVDRV_SUCCESS == ret)
    {

        /* Process next Read */
        R_RIIC_ReleaseLowHold(channel);

        /* Stop condition detection confirmation */
        ret = wait_stop(channel);
    }

    return (ret);
}
/******************************************************************************
 * End of Function riic_read
 ******************************************************************************/

///**
// * @brief Dummy-reads the RIIC receive data register RIICnDRR (n=0 to 3) of
// *        the channel specified by the argument channel and outputs the clock
// *        to the I2C bus to start the reception operation.
// * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
// * @return    DEVDRV_SUCCESS         : Success of RIIC operation
// *            DEVDRV_ERROR_TIMEOUT   : RIIC operation timed out
// **/
//static int_t riic_read_dummy (int_t channel)
//{
//    uint8_t dummy_buf_8b;
//    int_t ret;
//
//    /* Perform Dummy byte read */
//    ret = riic_receive(channel, RIIC_RX_MODE_NORMAL, &dummy_buf_8b);
//
//    return (ret);
//}
///******************************************************************************
// * End of Function riic_read_dummy
// ******************************************************************************/

/**
 * @brief Receive data (single byte)
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] mode  : receive mode
 *                  : RIIC_RX_MODE_NORMAL: Normal mode
 *                  : RIIC_RX_MODE_ACK: ACK mode
 *                  : RIIC_RX_MODE_LOW_HOLD: Low hold mode
 *                  : RIIC_RX_MODE_NACK: NACK mode
 *                  : RIIC_RX_MODE_STOP: Stop condition mode
 *            pdata : received data
 * @return    DEVDRV_SUCCESS         : Success of RIIC operation
 *            DEVDRV_ERROR_TIMEOUT   : RIIC operation timed out
 **/
static int_t riic_receive (int_t channel, e_riic_rx_mode_t mode, uint8_t *pdata)
{
    int_t ret;

    /* Wait until Receive data full conditions and clear receive-data-full
     condition have been satisfied */
    ret = wait_rx_full(channel);

    if (DEVDRV_SUCCESS == ret)
    {
        /* reset rx full flag prior to reception */
        gs_riic_receive_full_flg[channel] = DEVDRV_FLAG_OFF;

        switch (mode)
        {
            case RIIC_RX_MODE_ACK:
            {
                /* send ACK */
                R_RIIC_TransmitAck(channel);
            }
            break;

            case RIIC_RX_MODE_LOW_HOLD:
            {
                /* Period between ninth clock cycle and first
                 * clock cycle is held low */
                R_RIIC_AssertLowHold(channel);
            }
            break;

            case RIIC_RX_MODE_NACK:
            {
                /* send NACK */
                R_RIIC_TransmitNack(channel);
            }
            break;

            case RIIC_RX_MODE_STOP:
            {
                /* Stop condition request */
                R_RIIC_TransmitStop(channel);
            }
            break;
            case RIIC_RX_MODE_NORMAL:
            {
                /* Do Nothing */
                ret = DEVDRV_SUCCESS;
            }
            break;
            default:
            {
                /* Invalid selection */
                ret = DEVDRV_ERROR;
            }
            break;
        }

        /* Read data from wire */
        ( *pdata) = R_RIIC_ReadByte(channel);
    }

    /* report status */
    return (ret);
}
/*******************************************************************************
 End of function riic_receive
 *******************************************************************************/

/**
 * @brief Transmit data (single byte)
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] data    : data to transmit
 * @return    DEVDRV_SUCCESS         : Success of RIIC operation
 *            DEVDRV_ERROR           : Failure of RIIC operation
 *            DEVDRV_ERROR_TIMEOUT   : RIIC operation timed out
 **/
static int_t riic_transmit (int_t channel, uint8_t data)
{
    int_t ret = DEVDRV_ERROR;

    /* test for ACK */
    if (0u == R_RIIC_GetAckStatus(channel))
    {
        /* ACK received */
        ret = DEVDRV_SUCCESS;
    }
    else
    {
        /* NACK */
        ret = DEVDRV_ERROR;
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* ensure no current traffic on bus */
        ret = wait_tx_empty(channel);

        if (DEVDRV_SUCCESS == ret)
        {
            /* clear tx empty flag prior to tx */
            gs_riic_transmit_empty_flg[channel] = DEVDRV_FLAG_OFF;

            /* Write data to wire */
            R_RIIC_WriteByte(channel, data);
        }
    }

    return (ret);
}
/*******************************************************************************
 End of function riic_transmit
 *******************************************************************************/

/**
 * @brief Make the request to issue a start condition for the selected channel
 *        Perform the following sequence of actions
 *              1: Wait till bus is free
 *              2: Clear Receive data full flag
 *              3: Clear transmit end flag
 *              4: Clear transmit-data-empty flag
 *              5: Request to issue a start condition
 * @param[in] channel   : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    DEVDRV_SUCCESS         : Success of RIIC operation
 *            DEVDRV_ERROR           : Failure of RIIC operation
 **/
static int_t transmit_start (int_t channel)
{
    /* Wait for the bus to be free */
    int_t ret = wait_bus_master(channel, RIIC_BUS_MASTERSHIP_WAIT_FREE);

    if (DEVDRV_SUCCESS == ret)
    {
        /* reset flags */
        gs_riic_receive_full_flg[channel] = DEVDRV_FLAG_OFF;
        gs_riic_transmit_empty_flg[channel] = DEVDRV_FLAG_OFF;
        gs_riic_transmit_end_flg[channel] = DEVDRV_FLAG_OFF;

        /* initiate start condition */
        R_RIIC_TransmitStart(channel);
    }

    /* report status */
    return (ret);
}
/*******************************************************************************
 End of function transmit_start
 *******************************************************************************/

/**
 * @brief Make the request to issue a restart condition for the selected channel.
 *        Perform the following sequence of actions
 *              1: Wait till bus is busy
 *              2: Clear Receive data full flag
 *              3: Clear transmit end flag
 *              4: Clear transmit-data-empty flag
 *              5: Request to issue a restart condition
 * @param[in] channel   : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    DEVDRV_SUCCESS         : Success of RIIC operation
 *            DEVDRV_ERROR           : Failure of RIIC operation
 **/
static int_t transmit_restart (int_t channel)
{
    /* Wait for the bus to be busy */
    int_t ret = wait_bus_master(channel, RIIC_BUS_MASTERSHIP_WAIT_BUSY);

    if (DEVDRV_SUCCESS == ret)
    {
        /* reset flags */
        gs_riic_receive_full_flg[channel] = DEVDRV_FLAG_OFF;
        gs_riic_transmit_empty_flg[channel] = DEVDRV_FLAG_OFF;
        gs_riic_transmit_end_flg[channel] = DEVDRV_FLAG_OFF;

        /* set restart condition on bus */
        R_RIIC_TransmitRestart(channel);
    }

    return (ret);
}
/*******************************************************************************
 End of function transmit_restart
 *******************************************************************************/

/**
 * @brief Read data from slave in single byte addressing mode
 * @param[in] channel   : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] d_adr     : Slave device address
 *            r_adr     : Slave sub-address
 *            r_byte    : Number of bytes
 *            pr_buffer : buffer for data
 * @return    DEVDRV_SUCCESS         : Success of RIIC operation
 *            DEVDRV_ERROR           : Failure of RIIC operation
 **/
int_t read_data (int_t channel, uint8_t d_adr, uint16_t r_adr, uint32_t r_byte, uint8_t *pr_buffer)
{
    int_t ret;
    uint8_t pw_buffer[3];
    uint8_t dummy_data;

    /* ensure that channel is valid */
    ret = validate_channel(channel);

    /* Adding slave device addressing and write control (for slave sub-address) */
    pw_buffer[0] = (uint8_t) (d_adr & ((uint8_t)(~SAMPLE_RIIC_RWCODE_PRV_)));

    /* Adding slave sub-address */
    pw_buffer[1] = (uint8_t) (r_adr & 0x00ff);

    /* Setting slave device address and read control */
    pw_buffer[2] = (uint8_t) (d_adr | ((uint8_t)SAMPLE_RIIC_RWCODE_PRV_));

    if (DEVDRV_SUCCESS == ret)
    {
        /* wait for bus to be free */
        ret = wait_bus_master(channel, RIIC_BUS_MASTERSHIP_WAIT_FREE);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* start condition */
        ret = transmit_start(channel);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* For EEPROM devices with 1 byte addressing */
        ret = riic_write(channel, pw_buffer, 2);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        ret = transmit_restart(channel);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        ret = riic_single_write(channel, pw_buffer[2], RIIC_TEND_WAIT_RECEIVE);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* read data */
        riic_read(channel, pr_buffer, r_byte);
    }
    else
    {
        /* exit cleanly in event of an error */
        R_RIIC_TransmitStop(channel);
        riic_receive(channel, RIIC_RX_MODE_NORMAL, &dummy_data);
        wait_stop(channel);
    }

    return (ret);
}
/*******************************************************************************
 End of function read_data
 *******************************************************************************/

/**
 * @brief Read data from slave
 * @param[in] channel   : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] d_adr     : Slave device address
 *            r_byte    : Number of bytes
 *            pr_buffer : buffer for data
 * @return    DEVDRV_SUCCESS         : Success of RIIC operation
 *            DEVDRV_ERROR           : Failure of RIIC operation
 **/
int_t read_next_data (int_t channel, uint8_t d_adr, uint32_t r_byte, uint8_t *pr_buffer)
{
    int_t ret;
    uint8_t pw_buffer;
    uint8_t dummy_data;

    /* ensure that channel is valid */
    ret = validate_channel(channel);

    /* Setting slave device address and read control */
    pw_buffer = (uint8_t) (d_adr | ((uint8_t)SAMPLE_RIIC_RWCODE_PRV_));

    if (DEVDRV_SUCCESS == ret)
    {
        /* wait for bus to be free */
        ret = wait_bus_master(channel, RIIC_BUS_MASTERSHIP_WAIT_FREE);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* start condition */
        ret = transmit_start(channel);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        ret = riic_single_write(channel, pw_buffer, RIIC_TEND_WAIT_RECEIVE);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* read data */
        riic_read(channel, pr_buffer, r_byte);
    }
    else
    {
        /* exit cleanly in event of an error */
        R_RIIC_TransmitStop(channel);
        riic_receive(channel, RIIC_RX_MODE_NORMAL, &dummy_data);
        wait_stop(channel);
    }

    return (ret);
}
/*******************************************************************************
 End of function read_next_data
 *******************************************************************************/

/**
 * @brief Write data to slave in single byte addressing mode
 * @param[in] channel   : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] d_adr     : Slave device address
 *            w_adr     : Slave sub-address
 *            w_byte    : Number of bytes
 *            pw_buffer : buffer for data
 * @return    DEVDRV_SUCCESS         : Success of RIIC operation
 *            DEVDRV_ERROR           : Failure of RIIC operation
 *            DEVDRV_ERROR_RIIC_NACK : Failure of RIIC transmission due to
 *                                     NACK reception
 *            DEVDRV_ERROR_TIMEOUT   : IIC operation timed out
 **/
int_t write_data (int_t channel, uint8_t d_adr, uint16_t w_adr, uint32_t w_byte, uint8_t *pw_buffer)
{
    int_t ret;
    uint8_t buf[2];

    /* Ensure channel is valid */
    ret = validate_channel(channel);

    /* Adding slave device addressing and read/write control */
    buf[0] = (uint8_t) (d_adr & ((uint8_t)(~SAMPLE_RIIC_RWCODE_PRV_)));

    /* Adding slave sub-address */
    buf[1] = (uint8_t) (w_adr & 0x00ff);

    if (DEVDRV_SUCCESS == ret)
    {
        /* wait for bus to be free */
        ret = wait_bus_master(channel, RIIC_BUS_MASTERSHIP_WAIT_FREE);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* start condition */
        ret = transmit_start(channel);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* write slave address */
        ret = riic_write(channel, buf, 1);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* write slave sub-address */
        ret = riic_write(channel, (uint8_t *) &(buf[1]), 1);
    }

    if (DEVDRV_SUCCESS == ret)
    {
        /* write remaining bytes */
        ret = riic_write(channel, pw_buffer, w_byte);
    }

    /* stop condition */
    R_RIIC_TransmitStop(channel);

    /* wait for finish */
    wait_stop(channel);

    return (ret);
}
/*******************************************************************************
 End of function write_data
 *******************************************************************************/

/**
 * @brief Wait for transmit end flag set for selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] mode    : RIIC_TEND_WAIT_TRANSMIT
 *                    : RIIC_TEND_WAIT_RECEIVE
 * @return    DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 **/
static int_t wait_tx_end (int_t channel, uint32_t mode)
{
    int32_t timeout = ICC_MAX_TIMEOUT;
    int_t ret = DEVDRV_ERROR;

    if (RIIC_TEND_WAIT_TRANSMIT == mode)
    {
        /* Wait */
        while ((DEVDRV_FLAG_OFF == gs_riic_transmit_end_flg[channel]) && (0 < timeout))
        {
            /* Wait */
            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        /* RIIC_TEND_WAIT_RECEIVE */
        while ((DEVDRV_FLAG_OFF == gs_riic_receive_full_flg[channel]) && (0 < timeout))
        {
            /* Wait */
            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    /* determine if operation has timed out */
    if (0 < timeout)
    {
        ret = DEVDRV_SUCCESS;
    }

    /* return status */
    return (ret);
}
/*******************************************************************************
 End of function wait_tx_end
 ******************************************************************************/

/**
 * @brief Wait for receive full flag set for selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 **/
static int_t wait_rx_full (int_t channel)
{
    int32_t timeout = ICC_MAX_TIMEOUT;
    int_t ret = DEVDRV_ERROR;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == gs_riic_receive_full_flg[channel]) && (0 < timeout))
    {
        /* Wait */
        if (timeout > 0)
        {
            timeout--;
        }
    }

    /* determine if operation has timed out */
    if (0 < timeout)
    {
        ret = DEVDRV_SUCCESS;
    }

    /* return status */
    return (ret);
}
/*******************************************************************************
 End of function wait_rx_full
 *******************************************************************************/

/**
 * @brief Wait for transmit empty flag set for selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 **/
static int_t wait_tx_empty (int_t channel)
{
    int32_t timeout = ICC_MAX_TIMEOUT;
    int_t ret = DEVDRV_ERROR;

    /* Wait */
    while ((DEVDRV_FLAG_OFF == gs_riic_transmit_empty_flg[channel]) && (0 < timeout))
    {
        /* Wait */
        if (timeout > 0)
        {
            timeout--;
        }
    }

    /* determine if operation has timed out */
    if (0 < timeout)
    {
        ret = DEVDRV_SUCCESS;
    }

    /* return status */
    return (ret);
}
/*******************************************************************************
 End of function wait_tx_empty
 ******************************************************************************/

/**
 * @brief Wait for bus busy or bus free state for selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] mode    : RIIC_BUS_MASTERSHIP_WAIT_FREE : wait till bus free
 *                    : RIIC_BUS_MASTERSHIP_WAIT_BUSY : wait till bus busy
 * @return    DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 **/
static int_t wait_bus_master (int_t channel, uint32_t mode)
{
    int32_t timeout = ICC_MAX_TIMEOUT;
    int_t ret = DEVDRV_ERROR;

    if (RIIC_BUS_MASTERSHIP_WAIT_FREE == mode)
    {
        while ((1 == R_RIIC_IsBusBusy(channel)) && (0 < timeout))
        {
            /* Wait */
            if (timeout > 0)
            {
                timeout--;
            }
        }
    }
    else
    {
        while ((0 == R_RIIC_IsBusBusy(channel)) && (0 < timeout))
        {
            /* Wait */
            if (timeout > 0)
            {
                timeout--;
            }
        }
    }

    /* determine if operation has timed out */
    if (0 < timeout)
    {
        ret = DEVDRV_SUCCESS;
    }

    /* return status */
    return (ret);
}
/*******************************************************************************
 End of function wait_bus_master
 ******************************************************************************/

/**
 * @brief Wait for stop bit state for selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    DEVDRV_ERROR_TIMEOUT or DEVDRV_SUCCESS
 **/
static int_t wait_stop (int_t channel)
{
    int32_t timeout = ICC_MAX_TIMEOUT;
    int_t ret = DEVDRV_ERROR;

    /* Wait */
    while ((0 == R_RIIC_IsStopAsserted(channel)) && (0 < timeout))
    {
        /* Wait */
        if (timeout > 0)
        {
            timeout--;
        }
    }

    /* determine if operation has timed out */
    if (0 < timeout)
    {
        ret = DEVDRV_SUCCESS;
    }

    /* return status */
    return (ret);
}
/*******************************************************************************
 End of function wait_stop
 *******************************************************************************/

/**
 * @brief Hook for low level driver to indicate receive full for selected
 *        channel to the high level driver
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void r_riic_lld_set_rx_full (int_t channel)
{
    gs_riic_receive_full_flg[channel] = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function r_riic_lld_set_rx_full
 *******************************************************************************/

/**
 * @brief Hook for the low level driver to indicate transmit empty for selected
 *        channel to the high level driver
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void r_riic_lld_set_tx_empty (int_t channel)
{
    gs_riic_transmit_empty_flg[channel] = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function r_riic_lld_set_tx_empty
 *******************************************************************************/

/**
 * @brief Hook for the low level driver to indicate transmit end for selected
 *        channel to the high level driver
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void r_riic_lld_set_tx_end (int_t channel)
{
    gs_riic_transmit_end_flg[channel] = DEVDRV_FLAG_ON;
}
/*******************************************************************************
 End of function r_riic_lld_set_tx_end
 *******************************************************************************/

/* End of File */
