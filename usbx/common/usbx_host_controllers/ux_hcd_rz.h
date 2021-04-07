/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   Renesas RZ Host Controller Driver                                   */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  COMPONENT DEFINITION                                   RELEASE        */ 
/*                                                                        */ 
/*    ux_hcd_rz.h                                         PORTABLE C      */ 
/*                                                           5.8          */ 
/*  AUTHOR                                                                */ 
/*                                                                        */ 
/*    Thierry Giron                                                       */ 
/*                                                                        */ 
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This file contains all the header and extern functions used by the  */
/*    USBX host RZ Controller.                                            */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  06-01-2014     TCRG                     Initial Version 5.7           */ 
/*  06-01-2017     TCRG                     Modified comment(s),          */ 
/*                                            resulting in version 5.8    */ 
/*                                                                        */ 
/**************************************************************************/ 

#ifndef UX_HCD_RZ_H
#define UX_HCD_RZ_H

#define UX_RZ_HCD_USE_DMA

/* Define RZ generic definitions.  */

#define UX_RZ_CONTROLLER                            3
#define UX_RZ_CONTROLLER_RZ                         1
#define UX_RZ_MAX_BULK_PAYLOAD                      512 
#define UX_RZ_MAX_CONTROL_PAYLOAD                   512 
#define UX_RZ_FRAME_DELAY                           4 
#define UX_RZ_PERIODIC_ENTRY_NB                     32    
#define UX_RZ_PERIODIC_ENTRY_MASK                   0x1f
#define UX_RZ_ENABLE                                1
#define UX_RZ_DISABLE                               0
#define UX_RZ_HC_ED_CNT                             15
                                                        
/* Protection against no definition of RZ controller. */
#define UX_RZ

/* Define RZ HCOR register mapping.   */

#define UX_RZ_USB0_BASE                             0xE8010000
#define UX_RZ_USB1_BASE                             0xE8207000

#define UX_RZ_USB_BASE                              UX_RZ_USB1_BASE
                                                    

#ifdef UX_RZ
#define UX_RZ_HC_SYSCFG_0                           (UX_RZ_USB0_BASE + 0x00)

#define UX_RZ_HC_SYSCFG                             (UX_RZ_USB_BASE + 0x00)
#define UX_RZ_HC_BUSWAIT                            (UX_RZ_USB_BASE + 0x02)
#define UX_RZ_HC_SYSSTS0                            (UX_RZ_USB_BASE + 0x04)
#define UX_RZ_HC_DVSTCTR0                           (UX_RZ_USB_BASE + 0x08)
#define UX_RZ_HC_TESTMODE                           (UX_RZ_USB_BASE + 0x0C)
#define UX_RZ_HC_D0FBCFG                            (UX_RZ_USB_BASE + 0x10)
#define UX_RZ_HC_D1FBCFG                            (UX_RZ_USB_BASE + 0x12)
#define UX_RZ_HC_CFIFO                              (UX_RZ_USB_BASE + 0x14)
#define UX_RZ_HC_D0FIFO                             (UX_RZ_USB_BASE + 0x18)
#define UX_RZ_HC_D1FIFO                             (UX_RZ_USB_BASE + 0x1C)
#define UX_RZ_HC_CFIFOSEL                           (UX_RZ_USB_BASE + 0x20)
#define UX_RZ_HC_CFIFOCTR                           (UX_RZ_USB_BASE + 0x22)
#define UX_RZ_HC_D0FIFOSEL                          (UX_RZ_USB_BASE + 0x28)
#define UX_RZ_HC_D0FIFOCTR                          (UX_RZ_USB_BASE + 0x2A)
#define UX_RZ_HC_D1FIFOSEL                          (UX_RZ_USB_BASE + 0x2C)
#define UX_RZ_HC_D1FIFOCTR                          (UX_RZ_USB_BASE + 0x2E)
#define UX_RZ_HC_INTENB0                            (UX_RZ_USB_BASE + 0x30)
#define UX_RZ_HC_INTENB1                            (UX_RZ_USB_BASE + 0x32)
#define UX_RZ_HC_BRDYENB                            (UX_RZ_USB_BASE + 0x36)
#define UX_RZ_HC_NRDYENB                            (UX_RZ_USB_BASE + 0x38)
#define UX_RZ_HC_BEMPENB                            (UX_RZ_USB_BASE + 0x3A)
#define UX_RZ_HC_SOFCFG                             (UX_RZ_USB_BASE + 0x3C)
#define UX_RZ_HC_INTSTS0                            (UX_RZ_USB_BASE + 0x40)
#define UX_RZ_HC_INTSTS1                            (UX_RZ_USB_BASE + 0x42)
#define UX_RZ_HC_BRDYSTS                            (UX_RZ_USB_BASE + 0x46)
#define UX_RZ_HC_NRDYSTS                            (UX_RZ_USB_BASE + 0x48)
#define UX_RZ_HC_BEMPSTS                            (UX_RZ_USB_BASE + 0x4A)
#define UX_RZ_HC_FRMNUM                             (UX_RZ_USB_BASE + 0x4C)
#define UX_RZ_HC_UFRMNUM                            (UX_RZ_USB_BASE + 0x4E)
#define UX_RZ_HC_USBADDR                            (UX_RZ_USB_BASE + 0x50)
#define UX_RZ_HC_USBREQ                             (UX_RZ_USB_BASE + 0x54)
#define UX_RZ_HC_USBVAL                             (UX_RZ_USB_BASE + 0x56)
#define UX_RZ_HC_USBINDX                            (UX_RZ_USB_BASE + 0x58)
#define UX_RZ_HC_USBLENG                            (UX_RZ_USB_BASE + 0x5A)
#define UX_RZ_HC_DCPCFG                             (UX_RZ_USB_BASE + 0x5C)
#define UX_RZ_HC_DCPMAXP                            (UX_RZ_USB_BASE + 0x5E)
#define UX_RZ_HC_DCPCTR                             (UX_RZ_USB_BASE + 0x60)
#define UX_RZ_HC_PIPESEL                            (UX_RZ_USB_BASE + 0x64)
#define UX_RZ_HC_PIPECFG                            (UX_RZ_USB_BASE + 0x68)
#define UX_RZ_HC_PIPEBUF                            (UX_RZ_USB_BASE + 0x6A)
#define UX_RZ_HC_PIPEMAXP                           (UX_RZ_USB_BASE + 0x6C)
#define UX_RZ_HC_PIPEPERI                           (UX_RZ_USB_BASE + 0x6E)
#define UX_RZ_HC_PIPE1CTR                           (UX_RZ_USB_BASE + 0x70)
#define UX_RZ_HC_PIPE2CTR                           (UX_RZ_USB_BASE + 0x72)
#define UX_RZ_HC_PIPE3CTR                           (UX_RZ_USB_BASE + 0x74)
#define UX_RZ_HC_PIPE4CTR                           (UX_RZ_USB_BASE + 0x76)
#define UX_RZ_HC_PIPE5CTR                           (UX_RZ_USB_BASE + 0x78)
#define UX_RZ_HC_PIPE6CTR                           (UX_RZ_USB_BASE + 0x7A)
#define UX_RZ_HC_PIPE7CTR                           (UX_RZ_USB_BASE + 0x7C)
#define UX_RZ_HC_PIPE8CTR                           (UX_RZ_USB_BASE + 0x7E)
#define UX_RZ_HC_PIPE9CTR                           (UX_RZ_USB_BASE + 0x80)
#define UX_RZ_HC_PIPEACTR                           (UX_RZ_USB_BASE + 0x82)
#define UX_RZ_HC_PIPEBCTR                           (UX_RZ_USB_BASE + 0x84)
#define UX_RZ_HC_PIPECCTR                           (UX_RZ_USB_BASE + 0x86)
#define UX_RZ_HC_PIPEDCTR                           (UX_RZ_USB_BASE + 0x88)
#define UX_RZ_HC_PIPEECTR                           (UX_RZ_USB_BASE + 0x8A)
#define UX_RZ_HC_PIPEFCTR                           (UX_RZ_USB_BASE + 0x8C)
#define UX_RZ_HC_PIPE1TRE                           (UX_RZ_USB_BASE + 0x90)
#define UX_RZ_HC_PIPE1TRN                           (UX_RZ_USB_BASE + 0x92)
#define UX_RZ_HC_PIPE2TRE                           (UX_RZ_USB_BASE + 0x94)
#define UX_RZ_HC_PIPE2TRN                           (UX_RZ_USB_BASE + 0x96)
#define UX_RZ_HC_PIPE3TRE                           (UX_RZ_USB_BASE + 0x98)
#define UX_RZ_HC_PIPE3TRN                           (UX_RZ_USB_BASE + 0x9A)
#define UX_RZ_HC_PIPE4TRE                           (UX_RZ_USB_BASE + 0x9C)
#define UX_RZ_HC_PIPE4TRN                           (UX_RZ_USB_BASE + 0x9E)
#define UX_RZ_HC_PIPE5TRE                           (UX_RZ_USB_BASE + 0xA0)
#define UX_RZ_HC_PIPE5TRN                           (UX_RZ_USB_BASE + 0xA2)
#define UX_RZ_HC_PIPEBTRE                           (UX_RZ_USB_BASE + 0xA4)
#define UX_RZ_HC_PIPEBTRN                           (UX_RZ_USB_BASE + 0xA6)
#define UX_RZ_HC_PIPECTRE                           (UX_RZ_USB_BASE + 0xA8)
#define UX_RZ_HC_PIPECTRN                           (UX_RZ_USB_BASE + 0xAA)
#define UX_RZ_HC_PIPEDTRE                           (UX_RZ_USB_BASE + 0xAC)
#define UX_RZ_HC_PIPEDTRN                           (UX_RZ_USB_BASE + 0xAE)
#define UX_RZ_HC_PIPEETRE                           (UX_RZ_USB_BASE + 0xB0)
#define UX_RZ_HC_PIPEETRN                           (UX_RZ_USB_BASE + 0xB2)
#define UX_RZ_HC_PIPEFTRE                           (UX_RZ_USB_BASE + 0xB4)
#define UX_RZ_HC_PIPEFTRN                           (UX_RZ_USB_BASE + 0xB6)
#define UX_RZ_HC_PIPE9TRE                           (UX_RZ_USB_BASE + 0xB8)
#define UX_RZ_HC_PIPE9TRN                           (UX_RZ_USB_BASE + 0xBA)
#define UX_RZ_HC_PIPEATRE                           (UX_RZ_USB_BASE + 0xBC)
#define UX_RZ_HC_PIPEATRN                           (UX_RZ_USB_BASE + 0xBE)

#define UX_RZ_HC_DEVADD0                            (UX_RZ_USB_BASE + 0xD0)
#define UX_RZ_HC_DEVADD1                            (UX_RZ_USB_BASE + 0xD2)
#define UX_RZ_HC_DEVADD2                            (UX_RZ_USB_BASE + 0xD4)
#define UX_RZ_HC_DEVADD3                            (UX_RZ_USB_BASE + 0xD6)
#define UX_RZ_HC_DEVADD4                            (UX_RZ_USB_BASE + 0xD8)
#define UX_RZ_HC_DEVADD5                            (UX_RZ_USB_BASE + 0xDA)
#define UX_RZ_HC_DEVADD6                            (UX_RZ_USB_BASE + 0xDC)
#define UX_RZ_HC_DEVADD7                            (UX_RZ_USB_BASE + 0xDE)
#define UX_RZ_HC_DEVADD8                            (UX_RZ_USB_BASE + 0xE0)
#define UX_RZ_HC_DEVADD9                            (UX_RZ_USB_BASE + 0xE2)
#define UX_RZ_HC_DEVADDA                            (UX_RZ_USB_BASE + 0xE4)

#define UX_RZ_HC_SUSPMODE                           (UX_RZ_USB_BASE + 0x102)

#endif


/* Define RZ control register values.  */

#define UX_RZ_HC_SYSCFG_HSE                         (1<<7)
#define UX_RZ_HC_SYSCFG_DCFM                        (1<<6)
#define UX_RZ_HC_SYSCFG_DRPD                        (1<<5)
#define UX_RZ_HC_SYSCFG_DPRPU                       (1<<4)
#define UX_RZ_HC_SYSCFG_UCKSEL                      (1<<2)
#define UX_RZ_HC_SYSCFG_UPLLE                       (1<<1)
#define UX_RZ_HC_SYSCFG_USBE                        (1<<0)

#define UX_RZ_HC_SYSSTS0_LNST                       3

#define UX_RZ_HC_DVSTCTR0_WKUP                      (1<<8)
#define UX_RZ_HC_DVSTCTR0_RWUPE                     (1<<7)
#define UX_RZ_HC_DVSTCTR0_USBRST                    (1<<6)
#define UX_RZ_HC_DVSTCTR0_RESUME                    (1<<5)
#define UX_RZ_HC_DVSTCTR0_UACT                      (1<<4)
#define UX_RZ_HC_DVSTCTR0_RHST                      (0x7)

#define UX_RZ_HC_DVSTCTR0_SPEED_LOW                 (1)
#define UX_RZ_HC_DVSTCTR0_SPEED_FULL                (2)
#define UX_RZ_HC_DVSTCTR0_SPEED_HIGH                (3)
#define UX_RZ_HC_DVSTCTR0_RESET_IN_PROGRESS         (4)

#define UX_RZ_HC_CFIFOSEL_RCNT                      (1<<15)
#define UX_RZ_HC_CFIFOSEL_REW                       (1<<14)
#define UX_RZ_HC_CFIFOSEL_MBW_8                     (0<<10)
#define UX_RZ_HC_CFIFOSEL_MBW_16                    (1<<10)
#define UX_RZ_HC_CFIFOSEL_MBW_32                    (2<<10)
#define UX_RZ_HC_CFIFOSEL_MBW_MASK                  (3<<10)
#define UX_RZ_HC_CFIFOSEL_BIGEND                    (1<<8)
#define UX_RZ_HC_CFIFOSEL_ISEL                      (1<<5)
#define UX_RZ_HC_CFIFOSEL_CURPIPE_MASK              (0xF)

#define UX_RZ_HC_DFIFOSEL_RCNT                      (1<<15)
#define UX_RZ_HC_DFIFOSEL_REW                       (1<<14)
#define UX_RZ_HC_DFIFOSEL_DCLRM                     (1<<13)
#define UX_RZ_HC_DFIFOSEL_DREQE                     (1<<12)
#define UX_RZ_HC_DFIFOSEL_MBW_8                     (0<<10)
#define UX_RZ_HC_DFIFOSEL_MBW_16                    (1<<10)
#define UX_RZ_HC_DFIFOSEL_MBW_32                    (2<<10)
#define UX_RZ_HC_DFIFOSEL_MBW_MASK                  (3<<10)
#define UX_RZ_HC_DFIFOSEL_BIGEND                    (1<<8)
#define UX_RZ_HC_DFIFOSEL_CURPIPE_MASK              (0xF)

#define UX_RZ_HC_FIFOCTR_BVAL                       (1<<15)
#define UX_RZ_HC_FIFOCTR_BCLR                       (1<<14)
#define UX_RZ_HC_FIFOCTR_FRDY                       (1<<13)
#define UX_RZ_HC_FIFOCTR_DTLN                       (0xFFF)
                                                    
#define UX_RZ_HC_INTENB0_VBSE                       (1<<15)
#define UX_RZ_HC_INTENB0_RSME                       (1<<14)
#define UX_RZ_HC_INTENB0_SOFE                       (1<<13)
#define UX_RZ_HC_INTENB0_DVSE                       (1<<12)
#define UX_RZ_HC_INTENB0_CTRE                       (1<<11)
#define UX_RZ_HC_INTENB0_BEMPE                      (1<<10)
#define UX_RZ_HC_INTENB0_NRDYE                      (1<<9)
#define UX_RZ_HC_INTENB0_BRDYE                      (1<<8)

#define UX_RZ_HC_INTENB1_BCHGE                      (1<<14)
#define UX_RZ_HC_INTENB1_DTCHE                      (1<<12)
#define UX_RZ_HC_INTENB1_ATTCHE                     (1<<11)
#define UX_RZ_HC_INTENB1_EOFERRE                    (1<<6)
#define UX_RZ_HC_INTENB1_SIGNE                      (1<<5)
#define UX_RZ_HC_INTENB1_SACKE                      (1<<4)

#define UX_RZ_HC_PIPE0                              (1<<0)
#define UX_RZ_HC_PIPE1                              (1<<1)
#define UX_RZ_HC_PIPE2                              (1<<2)
#define UX_RZ_HC_PIPE3                              (1<<3)
#define UX_RZ_HC_PIPE4                              (1<<4)
#define UX_RZ_HC_PIPE5                              (1<<5)
#define UX_RZ_HC_PIPE6                              (1<<6)
#define UX_RZ_HC_PIPE7                              (1<<7)
#define UX_RZ_HC_PIPE8                              (1<<8)
#define UX_RZ_HC_PIPE9                              (1<<9)
#define UX_RZ_HC_PIPE_ALL                           (0x3FF)

#define UX_RZ_HC_SOFCFG_TRNENSEL                    (1<<8)
#define UX_RZ_HC_SOFCFG_BRDYM                       (1<<6)

#define UX_RZ_HC_INTSTS0_VBINT                      (1<<15)
#define UX_RZ_HC_INTSTS0_RESM                       (1<<14)
#define UX_RZ_HC_INTSTS0_SOFR                       (1<<13)
#define UX_RZ_HC_INTSTS0_DVST                       (1<<12)
#define UX_RZ_HC_INTSTS0_CTRT                       (1<<11)
#define UX_RZ_HC_INTSTS0_BEMP                       (1<<10)
#define UX_RZ_HC_INTSTS0_NRDY                       (1<<9)
#define UX_RZ_HC_INTSTS0_BRDY                       (1<<8)
#define UX_RZ_HC_INTSTS0_VBSTS                      (1<<7)
#define UX_RZ_HC_INTSTS0_DVSQ_MASK                  (7<<4)
#define UX_RZ_HC_INTSTS0_VALID                      (1<<3)
#define UX_RZ_HC_INTSTS0_CTSQ_MASK                  (7)

#define UX_RZ_HC_INTSTS1_BCHG                       (1<<14)
#define UX_RZ_HC_INTSTS1_DTCH                       (1<<12)
#define UX_RZ_HC_INTSTS1_ATTCH                      (1<<11)
#define UX_RZ_HC_INTSTS1_EOFERR                     (1<<6)
#define UX_RZ_HC_INTSTS1_SIGN                       (1<<5)
#define UX_RZ_HC_INTSTS1_SACK                       (1<<4)

#define UX_RZ_HC_FRMNUM_OVRN                        (1<<15)
#define UX_RZ_HC_FRMNUM_CRCE                        (1<<14)
#define UX_RZ_HC_FRMNUM_FRNM_MASK                   (0x7FF)

#define UX_RZ_HC_UFRMNUM_UFRNM_MASK                 (0x7)
 
#define UX_RZ_HC_DCPCFG_DIR                         (1<<4)

#define UX_RZ_HC_DCPMAXP_DEVADDR_SHIFT              (12)
#define UX_RZ_HC_DCPMAXP_DEVADDR_MASK               (0xF000)
#define UX_RZ_HC_DCPMAXP_MXPS_MASK                  (0x7F)

#define UX_RZ_HC_DCPCTR_BSTS                        (1<<15)
#define UX_RZ_HC_DCPCTR_SUREQ                       (1<<14)
#define UX_RZ_HC_DCPCTR_CSCLR                       (1<<13)
#define UX_RZ_HC_DCPCTR_CSSTS                       (1<<12)
#define UX_RZ_HC_DCPCTR_SUREQCLR                    (1<<11)
#define UX_RZ_HC_DCPCTR_SQCLR                       (1<<8)
#define UX_RZ_HC_DCPCTR_SQSET                       (1<<7)
#define UX_RZ_HC_DCPCTR_SQMON                       (1<<6)
#define UX_RZ_HC_DCPCTR_PBUSY                       (1<<5)
#define UX_RZ_HC_DCPCTR_PINGE                       (1<<4)
#define UX_RZ_HC_DCPCTR_CCPL                        (1<<2)
#define UX_RZ_HC_DCPCTR_PID_MASK                    (3)
#define UX_RZ_HC_DCPCTR_PIDNAK                      (0)
#define UX_RZ_HC_DCPCTR_PIDBUF                      (1)
#define UX_RZ_HC_DCPCTR_PIDSTALL                    (2)
#define UX_RZ_HC_DCPCTR_PIDSTALL2                   (3)

#define UX_RZ_HC_PIPECFG_TYPE_MASK                  (0xC000)
#define UX_RZ_HC_PIPECFG_TYPE_BIT_USED              (0)
#define UX_RZ_HC_PIPECFG_TYPE_BULK                  (1<<14)
#define UX_RZ_HC_PIPECFG_TYPE_INTERRUPT             (2<<14)
#define UX_RZ_HC_PIPECFG_TYPE_ISOCHRONOUS           (3<<14)
#define UX_RZ_HC_PIPECFG_BFRE                       (1<<10)
#define UX_RZ_HC_PIPECFG_DBLB                       (1<<9)
#define UX_RZ_HC_PIPECFG_CNTMD                      (1<<8)
#define UX_RZ_HC_PIPECFG_SHTNAK                     (1<<7)
#define UX_RZ_HC_PIPECFG_DIR                        (1<<4)
#define UX_RZ_HC_PIPECFG_EPNUM_MASK                 (0xF)

#define UX_RZ_HC_PIPEBUF_BUFSIZE_MASK               (0x1F<<10)
#define UX_RZ_HC_PIPEBUF_BUFNMB_MASK                (0xFF)
#define UX_RZ_HC_PIPEBUFSIZE_SHIFT                  (10)

#define UX_RZ_HC_PIPEMAXP_DEVSEL_MASK               (0xF<<12)
#define UX_RZ_HC_PIPEMAXP_DEVSEL_SHIFT              (12)
#define UX_RZ_HC_PIPEMAXP_MXPSMASK                  (0x7FF)

#define UX_RX_HC_PIPEPERI_IFIS                      (1<<12)
#define UX_RX_HC_PIPEPERI_IIVT_MASK                 (7)

#define UX_RZ_HC_PIPECTR_BSTS                       (1<<15)
#define UX_RZ_HC_PIPECTR_INBUFM                     (1<<14)
#define UX_RZ_HC_PIPECTR_CSCLR                      (1<<13)
#define UX_RZ_HC_PIPECTR_CSSTS                      (1<<12)
#define UX_RZ_HC_PIPECTR_ATREPM                     (1<<10)
#define UX_RZ_HC_PIPECTR_ACLRM                      (1<<9)
#define UX_RZ_HC_PIPECTR_SQCLR                      (1<<8)
#define UX_RZ_HC_PIPECTR_SQSET                      (1<<7)
#define UX_RZ_HC_PIPECTR_SQMON                      (1<<6)
#define UX_RZ_HC_PIPECTR_PBUSY                      (1<<5)
#define UX_RZ_HC_PIPECTR_PID_MASK                   (3)
#define UX_RZ_HC_PIPECTR_PIDNAK                     (0)
#define UX_RZ_HC_PIPECTR_PIDBUF                     (1)
#define UX_RZ_HC_PIPECTR_PIDSTALL                   (2)
#define UX_RZ_HC_PIPECTR_PIDSTALL2                  (3)

#define UX_RZ_HC_PIPECTR_DATA1                      (1<< 7)
#define UX_RZ_HC_PIPECTR_DATA0                      (1<< 8)

#define UX_RZ_HC_PIPETRE_TRCLR                      (1<<8)
#define UX_RZ_HC_PIPETRE_TRENB                      (1<<9)

#define UX_RZ_HC_DEVADD_UPPHUB_MASK                 (0x7800)
#define UX_RZ_HC_DEVADD_UPPHUB_SHIFT                (11)
#define UX_RZ_HC_DEVADD_HUBPORT_MASK                (0x0700)
#define UX_RZ_HC_DEVADD_HUBPORT_SHIFT               (8)
#define UX_RZ_HC_DEVADD_USBSPD_MASK                 (0x00C0)
#define UX_RZ_HC_DEVADD_USBSPD_SHIFT                (6)

#define UX_RZ_HC_DEVADD_SPEED_LOW                   (1<<6)
#define UX_RZ_HC_DEVADD_SPEED_FULL                  (2<<6)
#define UX_RZ_HC_DEVADD_SPEED_HIGH                  (3<<6)

#define UX_RX_HC_SUSPMODE_SUSPM                     (1<<14)

#define UX_RZ_HC_DCPCTR_DATA1                       (1<< 7)
#define UX_RZ_HC_DCPCTR_DATA0                       (1<< 8)

#define UX_RZ_HC_SUSPMODE_SUSPM                     (1<< 14)


/* Define RZ PIPE selection definitions.  */
#define UX_RZ_HC_PIPESEL_NO_PIPE                    0x000f

#define UX_RZ_HC_FIFO_D0                            0
#define UX_RZ_HC_FIFO_D1                            1
#define UX_RZ_HC_FIFO_C                             2


#ifdef UX_RZ_HCD_USE_DMA
/* Define RZ DMA register mapping.  */

#define UX_RZ_DMA_TX_CH                                     10
#define UX_RZ_DMA_RX_CH                                     11

#define UX_RZ_DMA_TX_INT_ID                                 INTC_ID_DMAINT10
#define UX_RZ_DMA_RX_INT_ID                                 INTC_ID_DMAINT11

#define UX_RZ_DMA_BASE(ch)                                  (0xE8200400 + ((ch) - 8) * 0x40)

#define UX_RZ_DMA_N0SA(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x00)
#define UX_RZ_DMA_N0DA(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x04)
#define UX_RZ_DMA_N0TB(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x08)
#define UX_RZ_DMA_N1SA(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x0C)
#define UX_RZ_DMA_N1DA(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x10)
#define UX_RZ_DMA_N1TB(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x14)
#define UX_RZ_DMA_CRSA(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x18)
#define UX_RZ_DMA_CRDA(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x1C)
#define UX_RZ_DMA_CRTB(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x20)
#define UX_RZ_DMA_CHSTAT(ch)                                (UX_RZ_DMA_BASE(ch) + 0x24)
#define UX_RZ_DMA_CHCTRL(ch)                                (UX_RZ_DMA_BASE(ch) + 0x28)
#define UX_RZ_DMA_CHCFG(ch)                                 (UX_RZ_DMA_BASE(ch) + 0x2C)
#define UX_RZ_DMA_CHITVL(ch)                                (UX_RZ_DMA_BASE(ch) + 0x30)
#define UX_RZ_DMA_CHIEXT(ch)                                (UX_RZ_DMA_BASE(ch) + 0x34)
#define UX_RZ_DMA_NXLA(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x38)
#define UX_RZ_DMA_CRLA(ch)                                  (UX_RZ_DMA_BASE(ch) + 0x3C)

#define UX_RZ_DMA_RS0                                       (0xFCFE1000)
#define UX_RZ_DMA_RS1                                       (0xFCFE1004)
#define UX_RZ_DMA_RS2                                       (0xFCFE1008)
#define UX_RZ_DMA_RS3                                       (0xFCFE100C)
#define UX_RZ_DMA_RS4                                       (0xFCFE1010)
#define UX_RZ_DMA_RS5                                       (0xFCFE1014)
#define UX_RZ_DMA_RS6                                       (0xFCFE1018)
#define UX_RZ_DMA_RS7                                       (0xFCFE101C)

#define UX_RZ_DMA_TX_RS                                     UX_RZ_DMA_RS5
#define UX_RZ_DMA_RX_RS                                     UX_RZ_DMA_RS5
#define UX_RZ_DMA_TX_RS_VALUE                               0x8B
#define UX_RZ_DMA_RX_RS_VALUE                               0x8B << 16

/* Define RZ DMA register values.  */

#define UX_RZ_DMA_CHSTAT_INTMSK                             (1<<16)
#define UX_RZ_DMA_CHSTAT_MODE                               (1<<11)
#define UX_RZ_DMA_CHSTAT_DER                                (1<<10)
#define UX_RZ_DMA_CHSTAT_DW                                 (1<<9)
#define UX_RZ_DMA_CHSTAT_DL                                 (1<<8)
#define UX_RZ_DMA_CHSTAT_SR                                 (1<<7)
#define UX_RZ_DMA_CHSTAT_TC                                 (1<<6)
#define UX_RZ_DMA_CHSTAT_END                                (1<<5)
#define UX_RZ_DMA_CHSTAT_ER                                 (1<<4)
#define UX_RZ_DMA_CHSTAT_SUS                                (1<<3)
#define UX_RZ_DMA_CHSTAT_TACT                               (1<<2)
#define UX_RZ_DMA_CHSTAT_RQST                               (1<<1)
#define UX_RZ_DMA_CHSTAT_EN                                 (1<<0)

#define UX_RZ_DMA_CHCTRL_CLRINTMSK                          (1<<17)
#define UX_RZ_DMA_CHCTRL_SETINTMSK                          (1<<16)
#define UX_RZ_DMA_CHCTRL_CLRSUS                             (1<<9)
#define UX_RZ_DMA_CHCTRL_SETSUS                             (1<<7)
#define UX_RZ_DMA_CHCTRL_CLRTC                              (1<<6)
#define UX_RZ_DMA_CHCTRL_CLREND                             (1<<5)
#define UX_RZ_DMA_CHCTRL_CLRRQ                              (1<<4)
#define UX_RZ_DMA_CHCTRL_SWRST                              (1<<3)
#define UX_RZ_DMA_CHCTRL_STG                                (1<<2)
#define UX_RZ_DMA_CHCTRL_CLREN                              (1<<1)
#define UX_RZ_DMA_CHCTRL_SETEN                              (1<<0)

#define UX_RZ_DMA_CHCFG_DMS                                 (1<<31)
#define UX_RZ_DMA_CHCFG_REN                                 (1<<30)
#define UX_RZ_DMA_CHCFG_RSW                                 (1<<29)
#define UX_RZ_DMA_CHCFG_RSEL                                (1<<28)
#define UX_RZ_DMA_CHCFG_SBE                                 (1<<27)
#define UX_RZ_DMA_CHCFG_DEM                                 (1<<24)
#define UX_RZ_DMA_CHCFG_TM                                  (1<<22)
#define UX_RZ_DMA_CHCFG_DAD                                 (1<<21)
#define UX_RZ_DMA_CHCFG_SAD                                 (1<<20)
#define UX_RZ_DMA_CHCFG_DDS_8                               (0<<16)
#define UX_RZ_DMA_CHCFG_DDS_16                              (1<<16)
#define UX_RZ_DMA_CHCFG_DDS_32                              (2<<16)
#define UX_RZ_DMA_CHCFG_DDS_64                              (3<<16)
#define UX_RZ_DMA_CHCFG_DDS_128                             (4<<16)
#define UX_RZ_DMA_CHCFG_DDS_256                             (5<<16)
#define UX_RZ_DMA_CHCFG_DDS_512                             (6<<16)
#define UX_RZ_DMA_CHCFG_DDS_1024                            (7<<16)
#define UX_RZ_DMA_CHCFG_DDS_MASK                            (0xF<<16)
#define UX_RZ_DMA_CHCFG_SDS_8                               (0<<12)
#define UX_RZ_DMA_CHCFG_SDS_16                              (1<<12)
#define UX_RZ_DMA_CHCFG_SDS_32                              (2<<12)
#define UX_RZ_DMA_CHCFG_SDS_64                              (3<<12)
#define UX_RZ_DMA_CHCFG_SDS_128                             (4<<12)
#define UX_RZ_DMA_CHCFG_SDS_256                             (5<<12)
#define UX_RZ_DMA_CHCFG_SDS_512                             (6<<12)
#define UX_RZ_DMA_CHCFG_SDS_1024                            (7<<12)
#define UX_RZ_DMA_CHCFG_SDS_MASK                            (0xF<<12)
#define UX_RZ_DMA_CHCFG_AM_MASK                             (0x7<<8)
#define UX_RZ_DMA_CHCFG_LVL                                 (1<<6)
#define UX_RZ_DMA_CHCFG_HIEN                                (1<<5)
#define UX_RZ_DMA_CHCFG_LOEN                                (1<<4)
#define UX_RZ_DMA_CHCFG_REQD                                (1<<3)
#define UX_RZ_DMA_CHCFG_SEL_MASK                            (0x7)

#define UX_RZ_DMA_STATUS_IDLE                               0
#define UX_RZ_DMA_STATUS_READING                            1
#define UX_RZ_DMA_STATUS_WRITING                            2
#define UX_RZ_DMA_STATUS_WRITING_SHORT                      3

#endif

/* Define RZ HCOR command/status bitmaps.  */


/* Define RZ static definition.  */

#define UX_RZ_HC_AVAILABLE_BANDWIDTH                920
#define UX_RZ_HC_INIT_DELAY                         1000
#define UX_RZ_HC_RESET_RETRY                        1000
#define UX_RZ_HC_RESET_DELAY                        10
#define UX_RZ_HC_PORT_RESET_RETRY                   50
#define UX_RZ_HC_FORCE_PORT_RESET_RETRY             50
#define UX_RZ_HC_FORCE_PORT_RESET_DELAY             1
#define UX_RZ_HC_CHECK_PORT_RESET_RETRY             500
#define UX_RZ_HC_PORT_RESET_DELAY                   300 
#define UX_RZ_HC_PORT_RESET_RECOVERY_DELAY          100 

/* Define RZ initialization values.  */

#define UX_RZ_HC_COMMAND_STATUS_RESET               0
#define UX_RZ_HC_INIT_RESET_DELAY                   10
#define UX_RZ_HC_MAX_BUF_SIZE                       64
#define UX_RZ_HC_MAX_BUF_NUM                        127


/* Define RZ FIFO write completion code.  */

#define UX_RZ_HC_FIFO_WRITING                       2
#define UX_RZ_HC_FIFO_WRITE_END                     3
#define UX_RZ_HC_FIFO_WRITE_SHORT                   4
#define UX_RZ_HC_FIFO_WRITE_DMA                     5
#define UX_RZ_HC_FIFO_WRITE_ERROR                   6

/* Define RZ FIFO read completion code.  */

#define UX_RZ_HC_FIFO_READING                       2
#define UX_RZ_HC_FIFO_READ_END                      3
#define UX_RZ_HC_FIFO_READ_SHORT                    4
#define UX_RZ_HC_FIFO_READ_DMA                      5
#define UX_RZ_HC_FIFO_READ_ERROR                    6
#define UX_RZ_HC_FIFO_READ_OVER                     7

/* Define RZ physical endpoint definitions.  */

#define UX_RZ_HC_ED_BRDY                            0x00000001
#define UX_RZ_HC_ED_NRDY                            0x00000002
#define UX_RZ_HC_ED_BEMP                            0x00000004
#define UX_RZ_HC_ED_EOFERR                          0x00000010
#define UX_RZ_HC_ED_SIGN                            0x00000020
#define UX_RZ_HC_ED_SACK                            0x00000040
#define UX_RZ_HC_ED_TIMEOUT                         0x00000080

/* Define RZ Root hub states.  */

#define UX_RZ_HC_PORT_ENABLED                       1
#define UX_RZ_HC_PORT_DISABLED                      0



/* Define RZ structure.  */

typedef struct UX_HCD_RZ_STRUCT
{

    struct UX_HCD_STRUCT
                    *ux_hcd_rz_hcd_owner;
    ULONG           ux_hcd_rz_hcor;
    UINT            ux_hcd_rz_nb_root_hubs;
    ULONG           ux_hcd_rz_port_status;
    ULONG           ux_hcd_rz_frame_number;
    ULONG           ux_hcd_rz_root_hub_device_address;
    struct UX_RZ_ED_STRUCT         
                    *ux_hcd_rz_ed_list;
    struct UX_RZ_TD_STRUCT         
                    *ux_hcd_rz_td_list;
    struct UX_RZ_ISO_TD_STRUCT     
                    *ux_hcd_rz_iso_td_list;
    struct UX_RZ_ED_STRUCT         
                    *ux_hcd_rz_asynch_head_ed;   
    struct UX_RZ_ED_STRUCT         
                    *ux_hcd_rz_asynch_current_ed;              
    struct UX_RZ_ED_STRUCT         
                    *ux_hcd_rz_iso_head_ed;
    struct UX_RZ_ED_STRUCT         
                    *ux_hcd_rz_interrupt_ed_list[32];
    UINT            ux_hcd_rz_queue_empty;
    UINT            ux_hcd_rz_periodic_scheduler_active;
    ULONG           ux_hcd_rz_number_tds;
    UINT            ux_hcd_rz_interruptible;
    ULONG           ux_hcd_rz_interrupt_count;
    ULONG           ux_hcd_rz_intsts0;
    ULONG           ux_hcd_rz_intsts1;
    ULONG           ux_rz_device_attached;
    ULONG           ux_rz_next_available_bufnum;    /* will need to implement some type of dynamic buffer management, for now just carve off */
    ULONG           ux_hcd_rz_ed_irq[UX_RZ_HC_ED_CNT];
    struct UX_RZ_ED_STRUCT  
                    *ux_hcd_rz_ed_ptr[UX_RZ_HC_ED_CNT];

#ifdef UX_RZ_HCD_USE_DMA
    ULONG           ux_hcd_rz_dma_status;
    ULONG           ux_hcd_rz_current_fifo_ctrl;
    UX_TRANSFER     *ux_hcd_rz_transfer_request;
#endif
    
} UX_HCD_RZ;


/* Define RZ ED structure.  */

typedef struct UX_RZ_ED_STRUCT
{

    struct UX_RZ_TD_STRUCT        
                    *ux_rz_ed_tail_td;
    struct UX_RZ_TD_STRUCT        
                    *ux_rz_ed_head_td;
    struct UX_RZ_ED_STRUCT        
                    *ux_rz_ed_next_ed;
    struct UX_RZ_ED_STRUCT        
                    *ux_rz_ed_previous_ed;

    ULONG           ux_rz_ed_status;
    struct UX_ENDPOINT_STRUCT          
                    *ux_rz_ed_endpoint;
    ULONG           ux_rz_ed_toggle;   
    ULONG           ux_rz_ed_frame;   

    ULONG           ux_rz_endpoint_index; 
    ULONG           ux_rz_pipe_index; 
    ULONG           ux_rz_fifo_index; 

    ULONG           ux_rz_buffer_size;
    ULONG           ux_rz_buffer_number;

} UX_RZ_ED;


/* Define RZ ED bitmap.  */

#define UX_RZ_ED_STATIC                                0x80000000
#define UX_RZ_ED_SKIP                                  0x40000000

/* Define RZ TD structure.  */

typedef struct UX_RZ_TD_STRUCT
{                                                

    UCHAR *         ux_rz_td_buffer;
    ULONG           ux_rz_td_length;
    struct UX_RZ_TD_STRUCT        
                    *ux_rz_td_next_td;
    struct UX_TRANSFER_STRUCT          
                    *ux_rz_td_transfer_request;
    struct UX_RZ_TD_STRUCT        
                    *ux_rz_td_next_td_transfer_request;
    struct UX_RZ_ED_STRUCT        
                    *ux_rz_td_ed;
    ULONG           ux_rz_td_actual_length;
    ULONG           ux_rz_td_status;
    ULONG           ux_rz_td_direction;
    ULONG           ux_rz_td_toggle;
    ULONG           ux_rz_td_transfer_type; 
    ULONG           ux_rz_td_error_count;

} UX_RZ_TD;


/* Define RZ TD bitmap.  */

#define UX_RZ_TD_SETUP_PHASE                           0x00010000   
#define UX_RZ_TD_DATA_PHASE                            0x00020000   
#define UX_RZ_TD_STATUS_PHASE                          0x00040000   
#define UX_RZ_TD_OUT                                   0x00000800
#define UX_RZ_TD_IN                                    0x00001000   
#define UX_RZ_TD_TOGGLE_FROM_ED                        0x80000000
#define UX_RZ_TD_ACK_PENDING                           0x00080000


#define UX_RZ_TD_SETUP_TYPE                            1
#define UX_RZ_TD_DATA_TYPE                             2
#define UX_RZ_TD_STATUS_TYPE                           3
#define UX_RZ_TD_MAX_ERROR                             3


/* Define RZ ISOCHRONOUS TD structure.  */

typedef struct UX_RZ_ISO_TD_STRUCT
{

    UCHAR *         ux_rz_iso_td_buffer;
    ULONG           ux_rz_iso_td_length;
    struct UX_RZ_ISO_TD_STRUCT    
                    *ux_rz_iso_td_next_td;
    struct UX_TRANSFER_STRUCT          
                    *ux_rz_iso_td_transfer_request;
    struct UX_RZ_ISO_TD_STRUCT    
                    *ux_rz_iso_td_next_td_transfer_request;
    struct UX_RZ_ED_STRUCT        
                    *ux_rz_iso_td_ed;
    ULONG           ux_rz_iso_td_actual_length;
    ULONG           ux_rz_iso_td_status;
    ULONG           ux_rz_iso_td_direction;
    
} UX_RZ_ISO_TD;


/* Define RZ function prototypes.  */

VOID    _ux_hcd_rz_asynch_queue_process(UX_HCD_RZ *hcd_rz);
VOID    _ux_hcd_rz_asynch_schedule(UX_HCD_RZ *hcd_rz);
UINT    _ux_hcd_rz_asynchronous_endpoint_create(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_rz_asynchronous_endpoint_destroy(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_rz_control_endpoint_create(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_rz_bulk_endpoint_create(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_rz_control_td_add(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
UINT    _ux_hcd_rz_bulk_int_td_add(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
UINT    _ux_hcd_rz_controller_disable(UX_HCD_RZ *hcd_rz);

#ifdef UX_RZ_HCD_USE_DMA
VOID    _ux_hcd_rz_dma_rx_interrupt_handler(UINT int_sense);
VOID    _ux_hcd_rz_dma_tx_interrupt_handler(UINT int_sense);
VOID    _ux_hcd_rz_dma_register_clear(UX_HCD_RZ *hcd_rz, ULONG rz_register, ULONG value);
ULONG   _ux_hcd_rz_dma_register_read(UX_HCD_RZ *hcd_rz, ULONG rz_register);
VOID    _ux_hcd_rz_dma_register_set(UX_HCD_RZ *hcd_rz, ULONG rz_register, ULONG value);
VOID    _ux_hcd_rz_dma_register_write(UX_HCD_RZ *hcd_rz, ULONG rz_register, ULONG value);
#endif

UX_RZ_ED       *_ux_hcd_rz_ed_obtain(UX_HCD_RZ *hcd_rz);
VOID    _ux_hcd_rz_ed_td_clean(UX_RZ_ED *ed);
UINT    _ux_hcd_rz_endpoint_reset(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint);
UINT    _ux_hcd_rz_entry(UX_HCD *hcd, UINT function, VOID *parameter);
UINT    _ux_hcd_rz_frame_number_get(UX_HCD_RZ *hcd_rz, ULONG *frame_number);
VOID    _ux_hcd_rz_frame_number_set(UX_HCD_RZ *hcd_rz, ULONG frame_number);
ULONG   _ux_hcd_rz_register_read(UX_HCD_RZ *hcd_rz, ULONG rz_register);
VOID    _ux_hcd_rz_register_write(UX_HCD_RZ *hcd_rz, ULONG rz_register, ULONG register_value);
VOID    _ux_hcd_rz_register_clear(UX_HCD_RZ *hcd_rz, ULONG rz_register, ULONG register_value);
VOID    _ux_hcd_rz_register_set(UX_HCD_RZ *hcd_rz, ULONG rz_register, ULONG register_value);
UINT    _ux_hcd_rz_initialize(UX_HCD *hcd);
UINT    _ux_hcd_rz_interrupt_endpoint_create(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint);
VOID    _ux_hcd_rz_interrupt_handler(UINT int_sense);
VOID    _ux_hcd_rz_iso_queue_process(UX_HCD_RZ *hcd_rz);
VOID    _ux_hcd_rz_iso_schedule(UX_HCD_RZ *hcd_rz);
UINT    _ux_hcd_rz_isochronous_endpoint_create(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint);
VOID    _ux_hcd_rz_low_level_setup(VOID);
UX_RZ_ISO_TD   *_ux_hcd_rz_isochronous_td_obtain(UX_HCD_RZ *hcd_rz);
UX_RZ_ED       *_ux_hcd_rz_least_traffic_list_get(UX_HCD_RZ *hcd_rz);
UINT    _ux_hcd_rz_periodic_endpoint_destroy(UX_HCD_RZ *hcd_rz, UX_ENDPOINT *endpoint);
VOID    _ux_hcd_rz_periodic_schedule(UX_HCD_RZ *hcd_rz);
UINT    _ux_hcd_rz_periodic_tree_create(UX_HCD_RZ *hcd_rz);
UINT    _ux_hcd_rz_port_disable(UX_HCD_RZ *hcd_rz, ULONG port_index);
UINT    _ux_hcd_rz_port_enable(UX_HCD_RZ *hcd_rz, ULONG port_index);
UINT    _ux_hcd_rz_port_reset(UX_HCD_RZ *hcd_rz, ULONG port_index);
UINT    _ux_hcd_rz_port_resume(UX_HCD_RZ *hcd_rz, UINT port_index);
ULONG   _ux_hcd_rz_port_status_get(UX_HCD_RZ *hcd_rz, ULONG port_index);
UINT    _ux_hcd_rz_port_suspend(UX_HCD_RZ *hcd_rz, ULONG port_index);
UINT    _ux_hcd_rz_power_down_port(UX_HCD_RZ *hcd_rz, ULONG port_index);
UINT    _ux_hcd_rz_power_on_port(UX_HCD_RZ *hcd_rz, ULONG port_index);
VOID    _ux_hcd_rz_power_root_hubs(UX_HCD_RZ *hcd_rz);
UINT    _ux_hcd_rz_td_add(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
UX_RZ_TD       *_ux_hcd_rz_regular_td_obtain(UX_HCD_RZ *hcd_rz);
UINT    _ux_hcd_rz_request_bulk_transfer(UX_HCD_RZ *hcd_rz, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_rz_request_control_transfer(UX_HCD_RZ *hcd_rz, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_rz_request_interrupt_transfer(UX_HCD_RZ *hcd_rz, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_rz_request_isochronous_transfer(UX_HCD_RZ *hcd_rz, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_rz_request_transfer(UX_HCD_RZ *hcd_rz, UX_TRANSFER *transfer_request);
UINT    _ux_hcd_rz_transfer_abort(UX_HCD_RZ *hcd_rz, UX_TRANSFER *transfer_request);
ULONG   _ux_hcd_rz_fifo_port_change(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed, ULONG direction);
VOID    _ux_hcd_rz_current_endpoint_change(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed, ULONG direction);
ULONG   _ux_hcd_rz_data_buffer_size(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
UINT    _ux_hcd_rz_buffer_write(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
UINT    _ux_hcd_rz_fifod_write(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
UINT    _ux_hcd_rz_fifoc_write(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
UINT    _ux_hcd_rz_fifo_read(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
UINT    _ux_hcd_rz_buffer_read(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
VOID    _ux_hcd_rz_buffer_ready_interrupt(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed, ULONG flag);
VOID    _ux_hcd_rz_buffer_empty_interrupt(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed, ULONG flag);
VOID    _ux_hcd_rz_endpoint_nak_set(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
VOID    _ux_hcd_rz_asynch_queue_process_bemp(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
VOID    _ux_hcd_rz_asynch_queue_process_nrdy(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
VOID    _ux_hcd_rz_asynch_queue_process_brdy(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
VOID    _ux_hcd_rz_asynch_queue_process_sign(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed);
VOID    _ux_hcd_rz_buffer_notready_interrupt(UX_HCD_RZ *hcd_rz, UX_RZ_ED *ed, ULONG flag);

#ifdef UX_RZ_HCD_USE_DMA
void  BSP_DCache_FlushRange       (void *, unsigned long);
void  BSP_DCache_InvalidateRange  (void *, unsigned long);
#endif

#define ux_hcd_rz_initialize                      _ux_hcd_rz_initialize
#define ux_hcd_rz_interrupt_handler               _ux_hcd_rz_interrupt_handler

#endif

