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
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     vdc_portsetting.h
 * @brief          VDC port setting functions
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZA1H
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 30.06.2018 1.00 First Release
 *****************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef VDC_PORTSETTING_H
#define VDC_PORTSETTING_H

/*******************************************************************************
 Includes <System Includes>, "Project Includes"
 *******************************************************************************/

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Structures
 ******************************************************************************/

/******************************************************************************
 Global Vaiables
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/******************************************************************************
 Function prototypes
 ******************************************************************************/

/**
 * @brief LCD panel I/O port setup (VDC5 channel 0)
 * @param param
 */
void VDC_LcdPortSetting(uint32_t param);

/**
 * @brief       CMOS CAMERA I/O port setup (VDC5 channel 0)
 * @param param
 */
void VDC_CameraPortSetting(uint32_t param);
#endif  /* VDC_PORTSETTING_H */

