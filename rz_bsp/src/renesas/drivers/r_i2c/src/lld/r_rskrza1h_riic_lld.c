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
 * File Name     : r_rskrza1h_riic_lld.c
 * Device(s)     : RZ/A1L
 * Tool-Chain    : GNUARM-NONEv16.01-EABI
 * H/W Platform  : Stream It! v2 board
 * Description   : RIIC driver (User define function)
 *******************************************************************************/
/*******************************************************************************
 * History       : DD.MM.YYYY Version Description
 *               : 21.10.2014 1.00
 *******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "dev_drv.h"                /* Device driver header */
#include "iodefine_cfg.h"           /* I/O Register root header */
#include "rza_io_regrw.h"           /* Low level register read/write header */
#include "r_intc.h"       /* Interrupt Controller driver header */
#include "r_rskrza1h_riic_lld.h"    /* RIIC Low level Driver Header */
#include "r_riic_drv_link.h"        /* high-low driver link */
#include "r_riic_drv_sc_cfg.h"          /* smart configurator / driver variables */

/* Common configuration file for all ISR's */
#include "r_task_priority.h"
#include "r_compiler_abstraction_api.h"

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
/* ==== CKS frequency setting (Clock(IIC clock) source selection) ==== */
#define RIIC_CKS_DIVISION_1_PRV_     (0) /* P0 clock divided by 1   */
#define RIIC_CKS_DIVISION_2_PRV_     (1) /* P0 clock divided by 2   */
#define RIIC_CKS_DIVISION_4_PRV_     (2) /* P0 clock divided by 4   */
#define RIIC_CKS_DIVISION_8_PRV_     (3) /* P0 clock divided by 8   */
#define RIIC_CKS_DIVISION_16_PRV_    (4) /* P0 clock divided by 16  */
#define RIIC_CKS_DIVISION_32_PRV_    (5) /* P0 clock divided by 32  */
#define RIIC_CKS_DIVISION_64_PRV_    (6) /* P0 clock divided by 64  */
#define RIIC_CKS_DIVISION_128_PRV_   (7) /* P0 clock divided by 128 */

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void init_i2c_peripheral (int_t channel, uint32_t cks, uint32_t brl, uint32_t brh);
static void enable_clock_supply (int_t channel);
static void disable_clock_supply (int_t channel);
static void setup_peripheral (int_t channel, uint32_t cks, uint32_t brl, uint32_t brh);
static void register_interrupts (int_t channel);
static void channel0_port_init (void);
static void channel1_port_init (void);
static void channel3_port_init (void);

/* An array of pointers to the riic peripheral for easy access. */
static const volatile struct st_riic *gsp_riic[] =
{ ( &RIIC0), ( &RIIC1), ( &RIIC2), ( &RIIC3) };

static const st_drv_info_t gs_lld_info =
{
{ ((STDIO_RIIC_RZ_LLD_VERSION_MAJOR << 16) + STDIO_RIIC_RZ_LLD_VERSION_MINOR) }, STDIO_RIIC_RZ_LLD_BUILD_NUM,
STDIO_RIIC_RZ_LLD_DRV_NAME };

/**
 * @brief initialise the specified i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    DEVDRV_SUCCESS       : Success of RIIC operation
 *            DEVDRV_ERROR         : Failure of RIIC operation
 **/
int_t R_RIIC_InitChannel (int_t channel, e_clk_frequency_riic_t frequency)
{
    int_t ret_value = DEVDRV_SUCCESS;

    uint32_t cks = RIIC_CKS_DIVISION_8_PRV_;
    uint32_t brl = 19;
    uint32_t brh = 16;

    /* configure the ports */
    switch ((e_sc_id_t) channel)
    {
        case R_SC0:
        {
            channel0_port_init();
        }
        break;

        case R_SC1:
        {
            channel1_port_init();
        }
        break;

        case R_SC3:
        {
            channel3_port_init();
        }
        break;

        default:
        {
            /* invalid port number */
            ret_value = DEVDRV_ERROR;
        }
        break;
    }

    if (DEVDRV_SUCCESS == ret_value)
    {
        /* determine clock settings for selected frequency */
        switch (frequency)
        {
            case RIIC_FREQUENCY_100KHZ:
            {
                /* 100 kHz */

                /* End configuration register settings */
                cks = RIIC_CKS_DIVISION_8_PRV_;
                brl = 19;
                brh = 16;
            }
            break;

            case RIIC_FREQUENCY_400KHZ:
            {
                /* 400 KHz */

                /* End configuration register settings */
                cks = RIIC_CKS_DIVISION_8_PRV_;
                brl = 19;
                brh = 16;
            }
            break;

            case RIIC_FREQUENCY_1MHZ:
            {
                /* 1MHz */

                /* End configuration register settings */
                cks = RIIC_CKS_DIVISION_8_PRV_;
                brl = 19;
                brh = 16;
            }
            break;

            default:
            {
                /* Assume 100kHz */
                /* End configuration register settings */
                cks = RIIC_CKS_DIVISION_8_PRV_;
                brl = 19;
                brh = 16;
            }
            break;
        }

        /* configure the riic registers for channel */
        init_i2c_peripheral(channel, cks, brl, brh);

        /* Register active interrupts */
        register_interrupts(channel);
    }

    /* return status */
    return (ret_value);
}
/*******************************************************************************
 End of function R_RIIC_InitChannel
 ******************************************************************************/

/**
 * @brief Close the specified i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_CloseChannel (int_t channel)
{
    /* turn peripheral clock to i2c channel off to disable */
    disable_clock_supply(channel);
}
/*******************************************************************************
 End of function R_RIIC_CloseChannel
 ******************************************************************************/

/**
 * @brief Make the request to issue a stop condition.
 *        Perform the following sequence of actions
 *        Clear Receive data full flag
 *        Requests to issue a stop condition
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitStop (int_t channel)
{
    /* get pointer to selected I2C peripheral register set */
    volatile struct st_riic *p_riic = (volatile struct st_riic *) gsp_riic[channel];

    /* Stop condition detection flag initialisation */
    rza_io_reg_write_8( &(p_riic->RIICnSR2.UINT8[0]), 0,
    RIICn_RIICnSR2_STOP_SHIFT,
    RIICn_RIICnSR2_STOP);

    /* Stop condition issuance request */
    rza_io_reg_write_8( &(p_riic->RIICnCR2.UINT8[0]), 1,
    RIICn_RIICnCR2_SP_SHIFT,
    RIICn_RIICnCR2_SP);
}
/*******************************************************************************
 End of function R_RIIC_TransmitStop
 *******************************************************************************/

/**
 * @brief Make the request to issue a start condition.
 *        Perform the following sequence of actions
 *        Clear Receive data full flag
 *        Requests to issue a start condition
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitStart (int_t channel)
{
    /* get pointer to selected I2C peripheral register set */
    volatile struct st_riic *p_riic = (volatile struct st_riic *) gsp_riic[channel];

    /* Stop condition issuance request */
    rza_io_reg_write_8( &(p_riic->RIICnCR2.UINT8[0]), 1,
    RIICn_RIICnCR2_ST_SHIFT,
    RIICn_RIICnCR2_ST);
}
/*******************************************************************************
 End of function R_RIIC_TransmitStart
 *******************************************************************************/

/**
 * @brief Make the request to issue a restart condition.
 *        Perform the following sequence of actions
 *        Clear Receive data full flag
 *        Requests to issue a restart condition
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitRestart (int_t channel)
{
    /* get pointer to selected I2C peripheral register set */
    volatile struct st_riic *p_riic = (volatile struct st_riic *) gsp_riic[channel];

    /* restart condition issuance request */
    rza_io_reg_write_8( &(p_riic->RIICnCR2.UINT8[0]), 1,
    RIICn_RIICnCR2_RS_SHIFT,
    RIICn_RIICnCR2_RS);
}
/*******************************************************************************
 End of function R_RIIC_TransmitRestart
 *******************************************************************************/

/**
 * @brief Clear NACK detection flag of selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_ClearNack (int_t channel)
{
    /* cast to allow assignment to non-const variable */
    rza_io_reg_write_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnSR2.UINT8[0]), 0,
    RIICn_RIICnSR2_NACKF_SHIFT, RIICn_RIICnSR2_NACKF);
}
/*******************************************************************************
 End of function R_RIIC_ClearNack
 *******************************************************************************/

/**
 * @brief Determine ACK status of selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    ACK status : 0 - ACK
 *                       : 1 - NACK
 **/
uint8_t R_RIIC_GetAckStatus (int_t channel)
{
    return (rza_io_reg_read_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnSR2.UINT8[0]),
    RIICn_RIICnSR2_NACKF_SHIFT, RIICn_RIICnSR2_NACKF));
}
/*******************************************************************************
 End of function R_RIIC_GetAckStatus
 *******************************************************************************/

/**
 * @brief Set ACK flag of selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitAck (int_t channel)
{
    /* Enable Modification of the ACKBT bit */
    rza_io_reg_write_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnMR3.UINT8[0]), 1,
    RIICn_RIICnMR3_ACKWP_SHIFT, RIICn_RIICnMR3_ACKWP);

    /* Send '0' acknowledge bit NACK Transmission */
    rza_io_reg_write_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnMR3.UINT8[0]), 0,
    RIICn_RIICnMR3_ACKBT_SHIFT, RIICn_RIICnMR3_ACKBT);
}
/*******************************************************************************
 End of function R_RIIC_TransmitAck
 ******************************************************************************/

/**
 * @brief Set NACK flag of selected channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_TransmitNack (int_t channel)
{
    /* Enable Modification of the ACKBT bit */
    rza_io_reg_write_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnMR3.UINT8[0]), 1,
    RIICn_RIICnMR3_ACKWP_SHIFT, RIICn_RIICnMR3_ACKWP);

    /* Send '1' acknowledge bit NACK Transmission */
    rza_io_reg_write_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnMR3.UINT8[0]), 1,
    RIICn_RIICnMR3_ACKBT_SHIFT, RIICn_RIICnMR3_ACKBT);
}
/*******************************************************************************
 End of function R_RIIC_TransmitNack
 ******************************************************************************/

/**
 * @brief Write a byte on the selected i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] byte : byte to send
 **/
void R_RIIC_WriteByte (int_t channel, uint8_t byte)
{
    /* cast to allow assignment to non-const variable */
    ((volatile struct st_riic *) (gsp_riic[channel]))->RIICnDRT.UINT8[0] = byte;
}
/*******************************************************************************
 End of function R_RIIC_WriteByte
 ******************************************************************************/

/**
 * @brief Read a byte on the selected i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    byte read
 **/
uint8_t R_RIIC_ReadByte (int_t channel)
{
    /* cast to allow assignment to non-const variable */
    return ((uint8_t) ((volatile struct st_riic *) (gsp_riic[channel]))->RIICnDRR.UINT8[0]);
}
/*******************************************************************************
 End of function R_RIIC_ReadByte
 ******************************************************************************/

/**
 * @brief Assert wait period by holding low during the period between 9th and 1st clock cycles
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_AssertLowHold (int_t channel)
{
    /* Period between ninth clock cycle and first
     * clock cycle is held low */
    rza_io_reg_write_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnMR3.UINT8[0]), 1,
    RIICn_RIICnMR3_WAIT_SHIFT, RIICn_RIICnMR3_WAIT);
}
/*******************************************************************************
 End of function R_RIIC_AssertLowHold
 ******************************************************************************/

/**
 * @brief Release any wait period by holding low during the period between 9th and 1st clock cycles
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
void R_RIIC_ReleaseLowHold (int_t channel)
{
    /* Period between ninth clock cycle and first
     * clock cycle is held low */
    rza_io_reg_write_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnMR3.UINT8[0]), 0,
    RIICn_RIICnMR3_WAIT_SHIFT, RIICn_RIICnMR3_WAIT);
}
/*******************************************************************************
 End of function R_RIIC_ReleaseLowHold
 ******************************************************************************/

/**
 * @brief Determine if bus is busy
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return    bus status 1: busy
 *                       0: not busy
 **/
uint8_t R_RIIC_IsBusBusy (int_t channel)
{
    /* return status */
    return (rza_io_reg_read_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnCR2.UINT8[0]),
    RIICn_RIICnCR2_BBSY_SHIFT, RIICn_RIICnCR2_BBSY));
}
/*******************************************************************************
 End of function R_RIIC_IsBusBusy
 ******************************************************************************/

/**
 * @brief Determine if stop is asserted on the selected i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @return          0 : stop not asserted
 *                  1 : stop asserted
 **/
uint8_t R_RIIC_IsStopAsserted (int_t channel)
{
    /* return status */
    return (rza_io_reg_read_8( &(((volatile struct st_riic *) (gsp_riic[channel]))->RIICnSR2.UINT8[0]),
    RIICn_RIICnSR2_STOP_SHIFT, RIICn_RIICnSR2_STOP));
}
/*******************************************************************************
 End of function R_RIIC_IsStopAsserted
 ******************************************************************************/

/**
 * @brief Return version of low level driver
 * @param[in] pinfo - pointer to detailed driver information structure
 * @param[in] cks  : I2c Clock Selection
 * @return    DRV_SUCCESS
 **/
int_t R_RIIC_GetVersion (st_ver_info_t *pinfo)
{
    pinfo->lld.version.sub.major = gs_lld_info.version.sub.major;
    pinfo->lld.version.sub.minor = gs_lld_info.version.sub.minor;
    pinfo->lld.build = gs_lld_info.build;
    pinfo->lld.p_szdriver_name = gs_lld_info.p_szdriver_name;
    return (DRV_SUCCESS);

}
/**********************************************************************************************************************
 End of function R_RIIC_GetVersion
 **********************************************************************************************************************/

/**
 * @brief Initialises the selected channel's peripheral register set
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 * @param[in] cks  : I2c Clock Selection
 *                   (Settings in the CKS bit of RIICnMR1 register)
 *                   RIIC_CKS_DIVISION_1   : P0 phi/1 clock
 *                   RIIC_CKS_DIVISION_2   : P0 phi/2 clock
 *                   RIIC_CKS_DIVISION_4   : P0 phi/4 clock
 *                   RIIC_CKS_DIVISION_8   : P0 phi/8 clock
 *                   RIIC_CKS_DIVISION_16  : P0 phi/16 clock
 *                   RIIC_CKS_DIVISION_32  : P0 phi/32 clock
 *                   RIIC_CKS_DIVISION_64  : P0 phi/64 clock
 *                   RIIC_CKS_DIVISION_128 : P0 phi/128 clock
 * @param[in] brl  : Lower byte value of the width of the SCL clock
 * @param[in] brh  : High byte value of the width of the SCL clock
 **/
static void init_i2c_peripheral (int_t channel, uint32_t cks, uint32_t brl, uint32_t brh)
{
    /* enable clock supply to peripheral */
    enable_clock_supply(channel);

    /* setup the i2c hardware now that clock is enabled */
    setup_peripheral(channel, cks, brl, brh);
}
/*******************************************************************************
 End of function init_i2c_peripheral
 *******************************************************************************/

/**
 * @brief Enable peripheral clock for specific i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
static void enable_clock_supply (int_t channel)
{
    volatile uint8_t dummy_buf = 0u;

    switch ((e_sc_id_t) channel)
    {
        case R_SC0:
        {
            /* ==== Module standby clear ==== */
            /* ---- Supply clock to the RIIC(channel 0) ---- */
            rza_io_reg_write_8((volatile uint8_t *) &CPG.STBCR9, 0,
            CPG_STBCR9_MSTP97_SHIFT, CPG_STBCR9_MSTP97);

            /* cast register address to volatile uint8_t pointer */
            dummy_buf = rza_io_reg_read_8((volatile uint8_t *) &CPG.STBCR9,
            CPG_STBCR9_MSTP97_SHIFT, CPG_STBCR9_MSTP97);
        }
        break;

        case R_SC1:
        {
            /* ==== Module standby clear ==== */
            /* ---- Supply clock to the RIIC(channel 1) ---- */
            rza_io_reg_write_8((volatile uint8_t *) &CPG.STBCR9, 0,
            CPG_STBCR9_MSTP96_SHIFT, CPG_STBCR9_MSTP96);

            /* cast register address to volatile uint8_t pointer */
            dummy_buf = rza_io_reg_read_8((volatile uint8_t *) &CPG.STBCR9,
            CPG_STBCR9_MSTP96_SHIFT, CPG_STBCR9_MSTP96);
        }
        break;

        default:
        {
            /* Do not reach here based on the assumption */
            R_COMPILER_Nop();
        }
        break;

        (void) dummy_buf;
    }
}
/*******************************************************************************
 End of function enable_clock_supply
 ******************************************************************************/

/**
 * @brief Disable peripheral clock for specific i2c channel
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
static void disable_clock_supply (int_t channel)
{
    volatile uint8_t dummy_buf = 0u;

    switch ((e_sc_id_t) channel)
    {
        case R_SC0:
        {
            /* ==== Module standby clear ==== */
            /* ---- Supply clock to the RIIC(channel 0) ---- */
            rza_io_reg_write_8((volatile uint8_t *) &CPG.STBCR9, 1,
            CPG_STBCR9_MSTP97_SHIFT, CPG_STBCR9_MSTP97);

            /* cast register address to volatile uint8_t pointer */
            dummy_buf = rza_io_reg_read_8((volatile uint8_t *) &CPG.STBCR9,
            CPG_STBCR9_MSTP97_SHIFT, CPG_STBCR9_MSTP97);
        }
        break;

        case R_SC1:
        {
            /* ==== Module standby clear ==== */
            /* ---- Supply clock to the RIIC(channel 1) ---- */
            rza_io_reg_write_8((volatile uint8_t *) &CPG.STBCR9, 1,
            CPG_STBCR9_MSTP96_SHIFT, CPG_STBCR9_MSTP96);

            /* cast register address to volatile uint8_t pointer */
            dummy_buf = rza_io_reg_read_8((volatile uint8_t *) &CPG.STBCR9,
            CPG_STBCR9_MSTP96_SHIFT, CPG_STBCR9_MSTP96);
        }
        break;

        default:
        {
            /* Do not reach here based on the assumption */
            R_COMPILER_Nop();
        }
        break;

        (void) dummy_buf;
    }
}
/*******************************************************************************
 End of function disable_clock_supply
 ******************************************************************************/

/**
 * @brief Configure specific i2c channel peripheral registers
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
static void setup_peripheral (int_t channel, uint32_t cks, uint32_t brl, uint32_t brh)
{
    volatile struct st_riic *p_riic = (volatile struct st_riic *) (gsp_riic[channel]);

    /* ==== RIIC initial setting ==== */
    /* Clear the RIICn reset bit */
    rza_io_reg_write_8( &(p_riic->RIICnCR1.UINT8[0]), 0,
    RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set I2C Bus Interface Internal Reset */
    rza_io_reg_write_8( &(p_riic->RIICnCR1.UINT8[0]), 1,
    RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);

    /* Set the RIICn reset bit */
    rza_io_reg_write_8( &(p_riic->RIICnCR1.UINT8[0]), 1,
    RIICn_RIICnCR1_ICE_SHIFT, RIICn_RIICnCR1_ICE);

    /* Set Internal Reference Clock (IIC) Selection */
    rza_io_reg_write_8( &(p_riic->RIICnMR1.UINT8[0]), (uint8_t) cks,
    RIICn_RIICnMR1_CKS_SHIFT, RIICn_RIICnMR1_CKS);

    /* Set Bit Rate Low-Level Period */
    rza_io_reg_write_8( &(p_riic->RIICnBRL.UINT8[0]), (uint8_t) brl,
    RIICn_RIICnBRL_BRL_SHIFT, RIICn_RIICnBRL_BRL);

    /* Set Bit Rate High-Level Period */
    rza_io_reg_write_8( &(p_riic->RIICnBRH.UINT8[0]), (uint8_t) brh,
    RIICn_RIICnBRH_BRH_SHIFT, RIICn_RIICnBRH_BRH);

    /* Set NACK Reception Transfer Suspension Enable */
    rza_io_reg_write_8( &(p_riic->RIICnFER.UINT8[0]), 1,
    RIICn_RIICnFER_NACKE_SHIFT, RIICn_RIICnFER_NACKE);

    /* Set Receive Data Full Interrupt Enable */
    rza_io_reg_write_8( &(p_riic->RIICnIER.UINT8[0]), 1,
    RIICn_RIICnIER_RIE_SHIFT, RIICn_RIICnIER_RIE);

    /* Set Transmit Data Empty Interrupt Enable */
    rza_io_reg_write_8( &(p_riic->RIICnIER.UINT8[0]), 1,
    RIICn_RIICnIER_TIE_SHIFT, RIICn_RIICnIER_TIE);

    /*Set Transmit End Interrupt Enable */
    rza_io_reg_write_8( &(p_riic->RIICnIER.UINT8[0]), 1,
    RIICn_RIICnIER_TEIE_SHIFT, RIICn_RIICnIER_TEIE);

    /* Clear I2C Bus Interface Internal Reset */
    rza_io_reg_write_8( &(p_riic->RIICnCR1.UINT8[0]), 0,
    RIICn_RIICnCR1_IICRST_SHIFT, RIICn_RIICnCR1_IICRST);
}
/*******************************************************************************
 End of function setup_peripheral
 ******************************************************************************/

/**
 * @brief Initialises channel 0 i2c port registers for this board
 **/
static void channel0_port_init (void)
{
    /* ---- P1_0 : SCL0 ---- */
    /* Port initialise */
    rza_io_reg_write_16( &GPIO.PIBC1, 0, GPIO_PIBC1_PIBC10_SHIFT, GPIO_PIBC1_PIBC10);
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC10_SHIFT, GPIO_PBDC1_PBDC10);
    rza_io_reg_write_16( &GPIO.PM1, 1, GPIO_PM1_PM10_SHIFT, GPIO_PM1_PM10);
    rza_io_reg_write_16( &GPIO.PMC1, 0, GPIO_PMC1_PMC10_SHIFT, GPIO_PMC1_PMC10);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC10_SHIFT, GPIO_PIPC1_PIPC10);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC10_SHIFT, GPIO_PBDC1_PBDC10);
    rza_io_reg_write_16( &GPIO.PFC1, 0, GPIO_PFC1_PFC10_SHIFT, GPIO_PFC1_PFC10);
    rza_io_reg_write_16( &GPIO.PFCE1, 0, GPIO_PFCE1_PFCE10_SHIFT, GPIO_PFCE1_PFCE10);
    rza_io_reg_write_16( &GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE10_SHIFT, GPIO_PFCAE1_PFCAE10);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC10_SHIFT, GPIO_PIPC1_PIPC10);
    rza_io_reg_write_16( &GPIO.PMC1, 1, GPIO_PMC1_PMC10_SHIFT, GPIO_PMC1_PMC10);

    /* ---- P1_1 : SDA0 ---- */
    /* Port initialise */
    rza_io_reg_write_16( &GPIO.PIBC1, 0, GPIO_PIBC1_PIBC11_SHIFT, GPIO_PIBC1_PIBC11);
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC11_SHIFT, GPIO_PBDC1_PBDC11);
    rza_io_reg_write_16( &GPIO.PM1, 1, GPIO_PM1_PM11_SHIFT, GPIO_PM1_PM11);
    rza_io_reg_write_16( &GPIO.PMC1, 0, GPIO_PMC1_PMC11_SHIFT, GPIO_PMC1_PMC11);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC11_SHIFT, GPIO_PIPC1_PIPC11);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC11_SHIFT, GPIO_PBDC1_PBDC11);
    rza_io_reg_write_16( &GPIO.PFC1, 0, GPIO_PFC1_PFC11_SHIFT, GPIO_PFC1_PFC11);
    rza_io_reg_write_16( &GPIO.PFCE1, 0, GPIO_PFCE1_PFCE11_SHIFT, GPIO_PFCE1_PFCE11);
    rza_io_reg_write_16( &GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE11_SHIFT, GPIO_PFCAE1_PFCAE11);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC11_SHIFT, GPIO_PIPC1_PIPC11);
    rza_io_reg_write_16( &GPIO.PMC1, 1, GPIO_PMC1_PMC11_SHIFT, GPIO_PMC1_PMC11);
}
/*******************************************************************************
 End of function channel0_port_init
 ******************************************************************************/

/**
 * @brief Initialises channel 1 i2c port registers for this board
 **/
static void channel1_port_init (void)
{
    /* ---- P1_2 : SCL1 ---- */
    /* Port initialise */
    rza_io_reg_write_16( &GPIO.PIBC1, 0, GPIO_PIBC1_PIBC12_SHIFT, GPIO_PIBC1_PIBC12);
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC12_SHIFT, GPIO_PBDC1_PBDC12);
    rza_io_reg_write_16( &GPIO.PM1, 1, GPIO_PM1_PM12_SHIFT, GPIO_PM1_PM12);
    rza_io_reg_write_16( &GPIO.PMC1, 0, GPIO_PMC1_PMC12_SHIFT, GPIO_PMC1_PMC12);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC12_SHIFT, GPIO_PIPC1_PIPC12);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC12_SHIFT, GPIO_PBDC1_PBDC12);
    rza_io_reg_write_16( &GPIO.PFC1, 0, GPIO_PFC1_PFC12_SHIFT, GPIO_PFC1_PFC12);
    rza_io_reg_write_16( &GPIO.PFCE1, 0, GPIO_PFCE1_PFCE12_SHIFT, GPIO_PFCE1_PFCE12);
    rza_io_reg_write_16( &GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE12_SHIFT, GPIO_PFCAE1_PFCAE12);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC12_SHIFT, GPIO_PIPC1_PIPC12);
    rza_io_reg_write_16( &GPIO.PMC1, 1, GPIO_PMC1_PMC12_SHIFT, GPIO_PMC1_PMC12);

    /* ---- P1_3 : SDA1 ---- */
    /* Port initialise */
    rza_io_reg_write_16( &GPIO.PIBC1, 0, GPIO_PIBC1_PIBC13_SHIFT, GPIO_PIBC1_PIBC13);
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC13_SHIFT, GPIO_PBDC1_PBDC13);
    rza_io_reg_write_16( &GPIO.PM1, 1, GPIO_PM1_PM13_SHIFT, GPIO_PM1_PM13);
    rza_io_reg_write_16( &GPIO.PMC1, 0, GPIO_PMC1_PMC13_SHIFT, GPIO_PMC1_PMC13);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC13_SHIFT, GPIO_PIPC1_PIPC13);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC13_SHIFT, GPIO_PBDC1_PBDC13);
    rza_io_reg_write_16( &GPIO.PFC1, 0, GPIO_PFC1_PFC13_SHIFT, GPIO_PFC1_PFC13);
    rza_io_reg_write_16( &GPIO.PFCE1, 0, GPIO_PFCE1_PFCE13_SHIFT, GPIO_PFCE1_PFCE13);
    rza_io_reg_write_16( &GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE13_SHIFT, GPIO_PFCAE1_PFCAE13);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC13_SHIFT, GPIO_PIPC1_PIPC13);
    rza_io_reg_write_16( &GPIO.PMC1, 1, GPIO_PMC1_PMC13_SHIFT, GPIO_PMC1_PMC13);
}
/*******************************************************************************
 End of function channel1_port_init
 ******************************************************************************/

/**
 * @brief Initialises channel 3 i2c port registers for this board
 **/
static void channel3_port_init (void)
{
    /* ---- P1_6 : SCL3 ---- */
    /* Port initialise */
    rza_io_reg_write_16( &GPIO.PIBC1, 0, GPIO_PIBC1_PIBC16_SHIFT, GPIO_PIBC1_PIBC16);
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC16_SHIFT, GPIO_PBDC1_PBDC16);
    rza_io_reg_write_16( &GPIO.PM1, 1, GPIO_PM1_PM16_SHIFT, GPIO_PM1_PM16);
    rza_io_reg_write_16( &GPIO.PMC1, 0, GPIO_PMC1_PMC16_SHIFT, GPIO_PMC1_PMC16);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC16_SHIFT, GPIO_PIPC1_PIPC16);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC16_SHIFT, GPIO_PBDC1_PBDC16);
    rza_io_reg_write_16( &GPIO.PFC1, 0, GPIO_PFC1_PFC16_SHIFT, GPIO_PFC1_PFC16);
    rza_io_reg_write_16( &GPIO.PFCE1, 0, GPIO_PFCE1_PFCE16_SHIFT, GPIO_PFCE1_PFCE16);
    rza_io_reg_write_16( &GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE16_SHIFT, GPIO_PFCAE1_PFCAE16);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC16_SHIFT, GPIO_PIPC1_PIPC16);
    rza_io_reg_write_16( &GPIO.PMC1, 1, GPIO_PMC1_PMC16_SHIFT, GPIO_PMC1_PMC16);

    /* ---- P1_7 : SDA3 ---- */
    /* Port initialise */
    rza_io_reg_write_16( &GPIO.PIBC1, 0, GPIO_PIBC1_PIBC17_SHIFT, GPIO_PIBC1_PIBC17);
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC17_SHIFT, GPIO_PBDC1_PBDC17);
    rza_io_reg_write_16( &GPIO.PM1, 1, GPIO_PM1_PM17_SHIFT, GPIO_PM1_PM17);
    rza_io_reg_write_16( &GPIO.PMC1, 0, GPIO_PMC1_PMC17_SHIFT, GPIO_PMC1_PMC17);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC17_SHIFT, GPIO_PIPC1_PIPC17);

    /* Port mode : Multiplex mode                     */
    /* Port function setting : 1st multiplex function */
    /* I/O control mode : Peripheral function         */
    /* Bidirectional mode : Enable                    */
    rza_io_reg_write_16( &GPIO.PBDC1, 1, GPIO_PBDC1_PBDC17_SHIFT, GPIO_PBDC1_PBDC17);
    rza_io_reg_write_16( &GPIO.PFC1, 0, GPIO_PFC1_PFC17_SHIFT, GPIO_PFC1_PFC17);
    rza_io_reg_write_16( &GPIO.PFCE1, 0, GPIO_PFCE1_PFCE17_SHIFT, GPIO_PFCE1_PFCE17);
    rza_io_reg_write_16( &GPIO.PFCAE1, 0, GPIO_PFCAE1_PFCAE17_SHIFT, GPIO_PFCAE1_PFCAE17);
    rza_io_reg_write_16( &GPIO.PIPC1, 1, GPIO_PIPC1_PIPC17_SHIFT, GPIO_PIPC1_PIPC17);
    rza_io_reg_write_16( &GPIO.PMC1, 1, GPIO_PMC1_PMC17_SHIFT, GPIO_PMC1_PMC17);
}
/*******************************************************************************
 End of function channel3_port_init
 ******************************************************************************/

/*******************************************************************************
 Channel Specific ISRs
 *******************************************************************************/

/**
 * @brief Channel 0's receive data full interrupt handler
 * @param[in] int_sense : Interrupt detection
 *                        INTC_LEVEL_SENSITIVE : Level sense
 *                        INTC_EDGE_TRIGGER    : Edge trigger
 **/
static void sample_riic_ri0_interrupt (uint32_t int_sense)
{
    /* prevent unused parameter compiler warning */
    UNUSED_PARAM(int_sense);

    /* set receive full flag */
    r_riic_lld_set_rx_full(R_SC0);
}
/*******************************************************************************
 End of function sample_riic_ri0_interrupt
 *******************************************************************************/

/**
 * @brief Channel 0's transmit data empty interrupt handler
 * @param[in] int_sense : Interrupt detection
 *                        INTC_LEVEL_SENSITIVE : Level sense
 *                        INTC_EDGE_TRIGGER    : Edge trigger
 **/
static void sample_riic_ti0_interrupt (uint32_t int_sense)
{
    /* prevent unused parameter compiler warning */
    UNUSED_PARAM(int_sense);

    /* Set transmit empty flag */
    r_riic_lld_set_tx_empty(R_SC0);
}
/*******************************************************************************
 End of function sample_riic_ti0_interrupt
 *******************************************************************************/

/**
 * @brief Channel 0's transmission end interrupt handler
 * @param[in] int_sense : Interrupt detection
 *                        INTC_LEVEL_SENSITIVE : Level sense
 *                        INTC_EDGE_TRIGGER    : Edge trigger
 **/
static void sample_riic_tei0_interrupt (uint32_t int_sense)
{
    volatile uint8_t dummy_buf_8b = 0u;
    volatile struct st_riic *priic = ((volatile struct st_riic *) (gsp_riic[R_SC0]));

    /* prevent unused parameter compiler warning */
    UNUSED_PARAM(int_sense);

    /* Suppresses the 'variable set but not used' warning */
    UNUSED_VARIABLE(dummy_buf_8b);

    /* set transmit end s/w flag */
    r_riic_lld_set_tx_end(R_SC0);

    /* Transmit end flag clear */
    rza_io_reg_write_8( &(priic->RIICnSR2.UINT8[0]), 0,
    RIICn_RIICnSR2_TEND_SHIFT, RIICn_RIICnSR2_TEND);

    dummy_buf_8b = rza_io_reg_read_8( &(priic->RIICnSR2.UINT8[0]),
    RIICn_RIICnSR2_TEND_SHIFT, RIICn_RIICnSR2_TEND);
}
/*******************************************************************************
 End of function sample_riic_tei0_interrupt
 *******************************************************************************/

/**
 * @brief Channel 1's receive data full interrupt handler
 * @param[in] int_sense : Interrupt detection
 *                        INTC_LEVEL_SENSITIVE : Level sense
 *                        INTC_EDGE_TRIGGER    : Edge trigger
 **/
static void sample_riic_ri1_interrupt (uint32_t int_sense)
{
    /* prevent unused parameter compiler warning */
    UNUSED_PARAM(int_sense);

    /* set receive full flag */
    r_riic_lld_set_rx_full(R_SC1);
}
/*******************************************************************************
 End of function sample_riic_ri1_interrupt
 *******************************************************************************/

/**
 * @brief Channel 1's transmit data empty interrupt handler
 * @param[in] int_sense : Interrupt detection
 *                        INTC_LEVEL_SENSITIVE : Level sense
 *                        INTC_EDGE_TRIGGER    : Edge trigger
 **/
static void sample_riic_ti1_interrupt (uint32_t int_sense)
{
    /* prevent unused parameter compiler warning */
    UNUSED_PARAM(int_sense);

    /* Set transmit empty flag */
    r_riic_lld_set_tx_empty(R_SC1);
}
/*******************************************************************************
 End of function sample_riic_ti1_interrupt
 *******************************************************************************/

/**
 * @brief Channel 1's transmission end interrupt handler
 * @param[in] int_sense : Interrupt detection
 *                        INTC_LEVEL_SENSITIVE : Level sense
 *                        INTC_EDGE_TRIGGER    : Edge trigger
 **/
static void sample_riic_tei1_interrupt (uint32_t int_sense)
{
    volatile uint8_t dummy_buf_8b = 0u;
    volatile struct st_riic *priic = ((volatile struct st_riic *) (gsp_riic[R_SC1]));

    /* prevent unused parameter compiler warning */
    UNUSED_PARAM(int_sense);

    /* Suppresses the 'variable set but not used' warning */
    UNUSED_VARIABLE(dummy_buf_8b);

    /* set transmit end s/w flag */
    r_riic_lld_set_tx_end(R_SC1);

    /* Transmit end flag clear */
    rza_io_reg_write_8( &(priic->RIICnSR2.UINT8[0]), 0,
    RIICn_RIICnSR2_TEND_SHIFT, RIICn_RIICnSR2_TEND);

    dummy_buf_8b = rza_io_reg_read_8( &(priic->RIICnSR2.UINT8[0]),
    RIICn_RIICnSR2_TEND_SHIFT, RIICn_RIICnSR2_TEND);

}
/*******************************************************************************
 End of function sample_riic_tei1_interrupt
 *******************************************************************************/

/**
 * @brief Channel 3's receive data full interrupt handler
 * @param[in] int_sense : Interrupt detection
 *                        INTC_LEVEL_SENSITIVE : Level sense
 *                        INTC_EDGE_TRIGGER    : Edge trigger
 **/
static void sample_riic_ri3_interrupt (uint32_t int_sense)
{
    /* prevent unused parameter compiler warning */
    UNUSED_PARAM(int_sense);

    /* set receive full flag */
    r_riic_lld_set_rx_full(R_SC3);
}
/*******************************************************************************
 End of function sample_riic_ri3_interrupt
 *******************************************************************************/

/**
 * @brief Channel 3's transmit data empty interrupt handler
 * @param[in] int_sense : Interrupt detection
 *                        INTC_LEVEL_SENSITIVE : Level sense
 *                        INTC_EDGE_TRIGGER    : Edge trigger
 **/
static void sample_riic_ti3_interrupt (uint32_t int_sense)
{
    /* prevent unused parameter compiler warning */
    UNUSED_PARAM(int_sense);

    /* Set transmit empty flag */
    r_riic_lld_set_tx_empty(R_SC3);
}
/*******************************************************************************
 End of function sample_riic_ti3_interrupt
 *******************************************************************************/

/**
 * @brief Channel 3's transmission end interrupt handler
 * @param[in] int_sense : Interrupt detection
 *                        INTC_LEVEL_SENSITIVE : Level sense
 *                        INTC_EDGE_TRIGGER    : Edge trigger
 **/
static void sample_riic_tei3_interrupt (uint32_t int_sense)
{
    volatile uint8_t dummy_buf_8b = 0u;
    volatile struct st_riic *priic = ((volatile struct st_riic *) (gsp_riic[R_SC3]));

    /* prevent unused parameter compiler warning */
    UNUSED_PARAM(int_sense);

    /* Suppresses the 'variable set but not used' warning */
    UNUSED_VARIABLE(dummy_buf_8b);

    /* set transmit end s/w flag */
    r_riic_lld_set_tx_end(R_SC3);

    /* Transmit end flag clear */
    rza_io_reg_write_8( &(priic->RIICnSR2.UINT8[0]), 0,
    RIICn_RIICnSR2_TEND_SHIFT, RIICn_RIICnSR2_TEND);

    dummy_buf_8b = rza_io_reg_read_8( &(priic->RIICnSR2.UINT8[0]),
    RIICn_RIICnSR2_TEND_SHIFT, RIICn_RIICnSR2_TEND);

}
/*******************************************************************************
 End of function sample_riic_tei3_interrupt
 *******************************************************************************/

/**
 * @brief Register interrupts for the specified channel.
 * @param[in] channel : the device specific channel number (< RIIC_LLD_NUM_CHANNELS)
 **/
static void register_interrupts (int_t channel)
{
    /* Register active interrupts */
    switch ((e_sc_id_t) channel)
    {
        case R_SC0:
        {
            /* Register active interrupts */
            R_INTC_RegistIntFunc(INTC_ID_INTIICRI0, sample_riic_ri0_interrupt);
            R_INTC_RegistIntFunc(INTC_ID_INTIICTI0, sample_riic_ti0_interrupt);
            R_INTC_RegistIntFunc(INTC_ID_INTIICTEI0, sample_riic_tei0_interrupt);

            /* Set active interrupt priorities */
            R_INTC_SetPriority(INTC_ID_INTIICRI0, ISR_IIC0_PRIORITY);
            R_INTC_SetPriority(INTC_ID_INTIICTI0, ISR_IIC0_PRIORITY);
            R_INTC_SetPriority(INTC_ID_INTIICTEI0, ISR_IIC0_PRIORITY);

            /* Enable active interrupts */
            R_INTC_Enable(INTC_ID_INTIICRI0);
            R_INTC_Enable(INTC_ID_INTIICTI0);
            R_INTC_Enable(INTC_ID_INTIICTEI0);
        }
        break;

        case R_SC1:
        {
            /* Register active interrupts */
            R_INTC_RegistIntFunc(INTC_ID_INTIICRI1, sample_riic_ri1_interrupt);
            R_INTC_RegistIntFunc(INTC_ID_INTIICTI1, sample_riic_ti1_interrupt);
            R_INTC_RegistIntFunc(INTC_ID_INTIICTEI1, sample_riic_tei1_interrupt);

            /* Set active interrupt priorities */
            R_INTC_SetPriority(INTC_ID_INTIICRI1, 9);
            R_INTC_SetPriority(INTC_ID_INTIICTI1, 9);
            R_INTC_SetPriority(INTC_ID_INTIICTEI1, 9);

            /* Enable active interrupts */
            R_INTC_Enable(INTC_ID_INTIICRI1);
            R_INTC_Enable(INTC_ID_INTIICTI1);
            R_INTC_Enable(INTC_ID_INTIICTEI1);
        }
        break;

        case R_SC3:
        {
            /* Register active interrupts */
            R_INTC_RegistIntFunc(INTC_ID_INTIICRI3, sample_riic_ri3_interrupt);
            R_INTC_RegistIntFunc(INTC_ID_INTIICTI3, sample_riic_ti3_interrupt);
            R_INTC_RegistIntFunc(INTC_ID_INTIICTEI3, sample_riic_tei3_interrupt);

            /* Set active interrupt priorities */
            R_INTC_SetPriority(INTC_ID_INTIICRI3, ISR_IIC3_PRIORITY);
            R_INTC_SetPriority(INTC_ID_INTIICTI3, ISR_IIC3_PRIORITY);
            R_INTC_SetPriority(INTC_ID_INTIICTEI3, ISR_IIC3_PRIORITY);

            /* Enable active interrupts */
            R_INTC_Enable(INTC_ID_INTIICRI3);
            R_INTC_Enable(INTC_ID_INTIICTI3);
            R_INTC_Enable(INTC_ID_INTIICTEI3);
        }
        break;

        default:
        {
            /* nothing can be done */
            R_COMPILER_Nop();
        }
        break;
    }
}
/*******************************************************************************
 End of function register_interrupts
 ******************************************************************************/

/* End of File */
