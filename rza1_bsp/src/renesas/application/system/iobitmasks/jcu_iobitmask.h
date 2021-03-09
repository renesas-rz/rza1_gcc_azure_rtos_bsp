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
* Copyright (C) 2017 - 2018 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File: jcu_local.h
*    JPEG Codec Unit (JCU) driver. Internal functions.
*
* - $Version: $ (=JCU_VERSION)
* - $ModuleRevision: {Masters}\JCU\RZ2\01 $
******************************************************************************/

#ifndef JCU_IOBITMASK_H
#define JCU_IOBITMASK_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    "r_jcu.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

#define JCU_JCCMD_DSP               (0x08u)
#define JCU_JCCMD_DSP_SHIFT         (3u)
#define JCU_JCCMD_REDU              (0x07u)
#define JCU_JCCMD_REDU_SHIFT        (0u)
#define JCU_JCCMD_BRST              (0x80u)
#define JCU_JCCMD_BRST_SHIFT        (7u)
#define JCU_JCCMD_JEND              (0x04u)
#define JCU_JCCMD_JEND_SHIFT        (2u)
#define JCU_JCCMD_JRST              (0x02u)
#define JCU_JCCMD_JRST_SHIFT        (1u)
#define JCU_JCCMD_JSRT              (0x01u)
#define JCU_JCCMD_JSRT_SHIFT        (0u)
#define JCU_JCMOD_DSP               (0x08u)
#define JCU_JCMOD_DSP_SHIFT         (3u)
#define JCU_JCMOD_REDU              (0x07u)
#define JCU_JCMOD_REDU_SHIFT        (0u)
#define JCU_JCMOD_BRST              (0x80u)
#define JCU_JCMOD_BRST_SHIFT        (7u)
#define JCU_JCMOD_JEND              (0x04u)
#define JCU_JCMOD_JEND_SHIFT        (2u)
#define JCU_JCMOD_JRST              (0x02u)
#define JCU_JCMOD_JRST_SHIFT        (1u)
#define JCU_JCMOD_JSRT              (0x01u)
#define JCU_JCMOD_JSRT_SHIFT        (0u)
#define JCU_JCQTN_QT3               (0x30u)
#define JCU_JCQTN_QT3_SHIFT         (4u)
#define JCU_JCQTN_QT2               (0x0Cu)
#define JCU_JCQTN_QT2_SHIFT         (2u)
#define JCU_JCQTN_QT1               (0x03u)
#define JCU_JCQTN_QT1_SHIFT         (0u)
#define JCU_JCHTN_HTA3              (0x20u)
#define JCU_JCHTN_HTA3_SHIFT        (5u)
#define JCU_JCHTN_HTD3              (0x10u)
#define JCU_JCHTN_HTD3_SHIFT        (4u)
#define JCU_JCHTN_HTA2              (0x08u)
#define JCU_JCHTN_HTA2_SHIFT        (3u)
#define JCU_JCHTN_HTD2              (0x04u)
#define JCU_JCHTN_HTD2_SHIFT        (2u)
#define JCU_JCHTN_HTA1              (0x02u)
#define JCU_JCHTN_HTA1_SHIFT        (1u)
#define JCU_JCHTN_HTD1              (0x01u)
#define JCU_JCHTN_HTD1_SHIFT        (0u)
#define JCU_JINTE0_INT7             (0x80u)
#define JCU_JINTE0_INT7_SHIFT       (7u)
#define JCU_JINTE0_INT6             (0x40u)
#define JCU_JINTE0_INT6_SHIFT       (6u)
#define JCU_JINTE0_INT5             (0x20u)
#define JCU_JINTE0_INT5_SHIFT       (5u)
#define JCU_JINTE0_INT3             (0x08u)
#define JCU_JINTE0_INT3_SHIFT       (3u)
#define JCU_JINTS0_INS6             (0x40u)
#define JCU_JINTS0_INS6_SHIFT       (6u)
#define JCU_JINTS0_INS5             (0x20u)
#define JCU_JINTS0_INS5_SHIFT       (5u)
#define JCU_JINTS0_INS3             (0x08u)
#define JCU_JINTS0_INS3_SHIFT       (3u)
#define JCU_JCDERR_ERR              (0x0Fu)
#define JCU_JCDERR_ERR_SHIFT        (0u)
#define JCU_JIFECNT_JOUTRINI        (0x00004000u)
#define JCU_JIFECNT_JOUTRINI_SHIFT  (14u)
#define JCU_JIFECNT_JOUTRCMD        (0x00002000u)
#define JCU_JIFECNT_JOUTRCMD_SHIFT  (13u)
#define JCU_JIFECNT_JOUTC           (0x00001000u)
#define JCU_JIFECNT_JOUTC_SHIFT     (12u)
#define JCU_JIFECNT_JOUTSWAP        (0x00000700u)
#define JCU_JIFECNT_JOUTSWAP_SHIFT  (8u)
#define JCU_JIFECNT_DINRINI         (0x00000040u)
#define JCU_JIFECNT_DINRINI_SHIFT   (6u)
#define JCU_JIFECNT_DINRCMD         (0x00000020u)
#define JCU_JIFECNT_DINRCMD_SHIFT   (5u)
#define JCU_JIFECNT_DINLC           (0x00000010u)
#define JCU_JIFECNT_DINLC_SHIFT     (4u)
#define JCU_JIFECNT_DINSWAP         (0x00000007u)
#define JCU_JIFECNT_DINSWAP_SHIFT   (0u)
#define JCU_JIFESOFST_ESMW          (0x00007FFFu)
#define JCU_JIFESOFST_ESMW_SHIFT    (0u)
#define JCU_JIFESLC_LINES           (0x0000FFFFu)
#define JCU_JIFESLC_LINES_SHIFT     (0u)
#define JCU_JIFESLC_JDATAS          (0x0000FFFFu)
#define JCU_JIFESLC_JDATAS_SHIFT    (0u)
#define JCU_JIFDDLC_LINES           (0x0000FFFFu)
#define JCU_JIFDDLC_LINES_SHIFT     (0u)
#define JCU_JIFDSDC_JDATAS          (0x0000FFFFu)
#define JCU_JIFDSDC_JDATAS_SHIFT    (0u)
#define JCU_JIFEDDC_JDATAS          (0x0000FFFFu)
#define JCU_JIFEDDC_JDATAS_SHIFT    (0u)
#define JCU_JIFDCNT_VINTER          (0x3C000000u)
#define JCU_JIFDCNT_VINTER_SHIFT    (28u)
#define JCU_JIFDCNT_HINTER          (0x0C000000u)
#define JCU_JIFDCNT_HINTER_SHIFT    (26u)
#define JCU_JIFDCNT_OPF             (0x03000000u)
#define JCU_JIFDCNT_OPF_SHIFT       (24u)
#define JCU_JIFDCNT_JINRINI         (0x00004000u)
#define JCU_JIFDCNT_JINRINI_SHIFT   (14u)
#define JCU_JIFDCNT_JINRCMD         (0x00002000u)
#define JCU_JIFDCNT_JINRCMD_SHIFT   (13u)
#define JCU_JIFDCNT_JINC            (0x00001000u)
#define JCU_JIFDCNT_JINC_SHIFT      (12u)
#define JCU_JIFDCNT_JINSWAP         (0x00000700u)
#define JCU_JIFDCNT_JINSWAP_SHIFT   (8u)
#define JCU_JIFDCNT_DOUTRINI        (0x00000040u)
#define JCU_JIFDCNT_DOUTRINI_SHIFT  (6u)
#define JCU_JIFDCNT_DOUTRCMD        (0x00000020u)
#define JCU_JIFDCNT_DOUTRCMD_SHIFT  (5u)
#define JCU_JIFDCNT_DOUTLC          (0x00000010u)
#define JCU_JIFDCNT_DOUTLC_SHIFT    (4u)
#define JCU_JIFDCNT_DOUTSWAP        (0x00000007u)
#define JCU_JIFDCNT_DOUTSWAP_SHIFT  (0u)
#define JCU_JIFDDOFST_DDMW          (0x00007FFFu)
#define JCU_JIFDDOFST_DDMW_SHIFT    (0u)
#define JCU_JIFDADT_ALPHA           (0x000000FFu)
#define JCU_JIFDADT_ALPHA_SHIFT     (0u)
#define JCU_JINTE1_CBTEN            (0x00000040u)
#define JCU_JINTE1_CBTEN_SHIFT      (6u)
#define JCU_JINTE1_DINLEN           (0x00000020u)
#define JCU_JINTE1_DINLEN_SHIFT     (5u)
#define JCU_JINTE1_JOUTEN           (0x00000010u)
#define JCU_JINTE1_JOUTEN_SHIFT     (4u)
#define JCU_JINTE1_DBTEN            (0x00000004u)
#define JCU_JINTE1_DBTEN_SHIFT      (2u)
#define JCU_JINTE1_JINEN            (0x00000002u)
#define JCU_JINTE1_JINEN_SHIFT      (1u)
#define JCU_JINTE1_DOUTLEN          (0x00000001u)
#define JCU_JINTE1_DOUTLEN_SHIFT    (0u)
#define JCU_JINTS1_CBTF             (0x00000040u)
#define JCU_JINTS1_CBTF_SHIFT       (6u)
#define JCU_JINTS1_DINLF            (0x00000020u)
#define JCU_JINTS1_DINLF_SHIFT      (5u)
#define JCU_JINTS1_JOUTF            (0x00000010u)
#define JCU_JINTS1_JOUTF_SHIFT      (4u)
#define JCU_JINTS1_DBTF             (0x00000004u)
#define JCU_JINTS1_DBTF_SHIFT       (2u)
#define JCU_JINTS1_JINF             (0x00000002u)
#define JCU_JINTS1_JINF_SHIFT       (1u)
#define JCU_JINTS1_DOUTLF           (0x00000001u)
#define JCU_JINTS1_DOUTLF_SHIFT     (0u)
#define JCU_JIFESVSZ_DINYCHG        (0x00008000u)
#define JCU_JIFESVSZ_DINYCHG_SHIFT  (15u)
#define JCU_JIFESHSZ_DOUTYCHG       (0x00008000u)
#define JCU_JIFESHSZ_DOUTYCHG_SHIFT (15u)

/* Others */
#define JCU_JINTS1_ALL  ( \
    JCU_JINTS1_CBTF   | \
    JCU_JINTS1_DINLF  | \
    JCU_JINTS1_JOUTF  | \
    JCU_JINTS1_DBTF   | \
    JCU_JINTS1_JINF   | \
    JCU_JINTS1_DOUTLF )

#endif /* JCU_IOBITMASK_H */
