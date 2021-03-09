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
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : control.h
* Version      : 1.0
* Description  : The functions and structures for device specific control and
*                the ANSI low level functions. If you only need file read and
*                write you should use the ANSI functions as your code will
*                remain portable. If you want to access things like, serial
*                ports and disk drives low level I/O is for you.
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 05.08.2010 1.00    First Release
*         : 10.11.2010 1.01    Updated Serial Control Codes
*         : 14.12.2010 1.02    Added FTDI Driver Codes
******************************************************************************/

/******************************************************************************
  WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
  OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
  SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
******************************************************************************/

#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "r_typedefs.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/* This is so the low level open function can tell the difference between a
   file and a device:
   Device name \\.\myDevice
   file name a:\myFile.txt
*/
#define DEVICE_INDENTIFIER          "\\\\.\\"

#ifndef BIT_0                       /* set bits */
#define BIT_0                       0x1
#define BIT_1                       0x2
#define BIT_2                       0x4
#define BIT_3                       0x8
#define BIT_4                       0x10
#define BIT_5                       0x20
#define BIT_6                       0x40
#define BIT_7                       0x80

#define BIT_8                       0x100
#define BIT_9                       0x200
#define BIT_10                      0x400
#define BIT_11                      0x800
#define BIT_12                      0x1000
#define BIT_13                      0x2000
#define BIT_14                      0x4000
#define BIT_15                      0x8000

#define BIT_16                      0x10000L
#define BIT_17                      0x20000L
#define BIT_18                      0x40000L
#define BIT_19                      0x80000L
#define BIT_20                      0x100000L
#define BIT_21                      0x200000L
#define BIT_22                      0x400000L
#define BIT_23                      0x800000L

#define BIT_24                      0x1000000L
#define BIT_25                      0x2000000L
#define BIT_26                      0x4000000L
#define BIT_27                      0x8000000L
#define BIT_28                      0x10000000L
#define BIT_29                      0x20000000L
#define BIT_30                      0x40000000L
#define BIT_31                      0x80000000L
#endif


/*==============================================*/
/*Generic Full-Word Access                      */
/*==============================================*/
#define GENERIC_16B_MASK        (0xFFFFu)
#define    USB_CFIFOSEL_MBW_0        (0x0400u) //added to allow manipulation of bit 10
#define NO_SHIFT                (0)


/** Common id descriptors for specifying channel support in low layer drivers */
typedef enum
{
    R_CH0  = (0x01 <<  0),  /*!< Reference channel 0 of associated driver */
    R_CH1  = (0x01 <<  1),  /*!< Reference channel 1 of associated driver */
    R_CH2  = (0x01 <<  2),  /*!< Reference channel 2 of associated driver */
    R_CH3  = (0x01 <<  3),  /*!< Reference channel 3 of associated driver */
    R_CH4  = (0x01 <<  4),  /*!< Reference channel 4 of associated driver */
    R_CH5  = (0x01 <<  5),  /*!< Reference channel 5 of associated driver */
    R_CH6  = (0x01 <<  6),  /*!< Reference channel 6 of associated driver */
    R_CH7  = (0x01 <<  7),  /*!< Reference channel 7 of associated driver */
    R_CH8  = (0x01 <<  8),  /*!< Reference channel 8 of associated driver */
    R_CH9  = (0x01 <<  9),  /*!< Reference channel 9 of associated driver */
    R_CH10 = (0x01 << 10),  /*!< Reference channel 10 of associated driver */
    R_CH11 = (0x01 << 11),  /*!< Reference channel 11 of associated driver */
    R_CH12 = (0x01 << 12),  /*!< Reference channel 12 of associated driver */
    R_CH13 = (0x01 << 13),  /*!< Reference channel 13 of associated driver */
    R_CH14 = (0x01 << 14),  /*!< Reference channel 14 of associated driver */
    R_CH15 = (0x01 << 15),  /*!< Reference channel 15 of associated driver */
    R_CH_ANY = (0xFFFF)     /*!< Reference ANY available channel of associated driver */
} e_channel_id_t;

/** Common id descriptors for specifying smart configuration index 32 possible configurations supported  */
typedef enum
{
    R_SC0  = (0x00),  /*!< Reference smart configuration index  0 of associated driver */
    R_SC1,            /*!< Reference smart configuration index  1 of associated driver */
    R_SC2,            /*!< Reference smart configuration index  2 of associated driver */
    R_SC3,            /*!< Reference smart configuration index  3 of associated driver */
    R_SC4,            /*!< Reference smart configuration index  4 of associated driver */
    R_SC5,            /*!< Reference smart configuration index  5 of associated driver */
    R_SC6,            /*!< Reference smart configuration index  6 of associated driver */
    R_SC7,            /*!< Reference smart configuration index  7 of associated driver */
    R_SC8,            /*!< Reference smart configuration index  8 of associated driver */
    R_SC9,            /*!< Reference smart configuration index  9 of associated driver */
    R_SC10,           /*!< Reference smart configuration index 10 of associated driver */
    R_SC11,           /*!< Reference smart configuration index 11 of associated driver */
    R_SC12,           /*!< Reference smart configuration index 12 of associated driver */
    R_SC13,           /*!< Reference smart configuration index 13 of associated driver */
    R_SC14,           /*!< Reference smart configuration index 14 of associated driver */
    R_SC15,           /*!< Reference smart configuration index 15 of associated driver */
    R_SC16,           /*!< Reference smart configuration index 16 of associated driver */
    R_SC17,           /*!< Reference smart configuration index 17 of associated driver */
    R_SC18,           /*!< Reference smart configuration index 18 of associated driver */
    R_SC19,           /*!< Reference smart configuration index 19 of associated driver */
    R_SC20,           /*!< Reference smart configuration index 20 of associated driver */
    R_SC21,           /*!< Reference smart configuration index 21 of associated driver */
    R_SC22,           /*!< Reference smart configuration index 22 of associated driver */
    R_SC23,           /*!< Reference smart configuration index 23 of associated driver */
    R_SC24,           /*!< Reference smart configuration index 24 of associated driver */
    R_SC25,           /*!< Reference smart configuration index 25 of associated driver */
    R_SC26,           /*!< Reference smart configuration index 26 of associated driver */
    R_SC27,           /*!< Reference smart configuration index 27 of associated driver */
    R_SC28,           /*!< Reference smart configuration index 28 of associated driver */
    R_SC29,           /*!< Reference smart configuration index 29 of associated driver */
    R_SC30,           /*!< Reference smart configuration index 30 of associated driver */
    R_SC31,           /*!< Reference smart configuration index 31 of associated driver */
} e_sc_id_t;

/******************************************************************************
Typedef definitions
******************************************************************************/

typedef enum _CTLCODE
{
    CTL_FILE_SEEK = 0,
    CTL_FILE_SIZE,
    CTL_GET_VERSION,
    CTL_GET_LAST_ERROR,
    CTL_GET_ERROR_STRING,
    CTL_WDT_RESET,
    CTL_GET_TIME_STAMP,
    CTL_SET_OVERLAPPED_READ_EVENT,
    CTL_GET_OVERLAPPED_READ_RESULT,
    CTL_CANCEL_OVERLAPPED_READ,
    CTL_SET_OVERLAPPED_WRITE_EVENT,
    CTL_GET_OVERLAPPED_WRITE_RESULT,
    CTL_CANCEL_OVERLAPPED_WRITE,
    CTL_SET_TIME_OUT,
    CTL_GET_TIME_OUT,
    CTL_USB_ATTACHED,
    CTL_USB_MS_RESET,
    CTL_USB_MS_GET_MAX_LUN,
    CTL_USB_MS_CLEAR_BULK_IN_STALL,
    CTL_USB_MS_WAIT_MUTEX,
    CTL_USB_MS_RELEASE_MUTEX,
    CTL_SCI_SET_CONFIGURATION,
    CTL_SCI_GET_CONFIGURATION,
    CTL_SCI_GET_LINE_STATUS,
    CTL_SCI_PURGE_BUFFERS,
    CTL_SCI_SET_BREAK,
    CTL_SCI_CLEAR_BREAK,
    CTL_GET_RX_BUFFER_COUNT,
    CTL_GET_STDIN_EVENT,
    CTL_GET_DATE,
    CTL_SET_DATE,
    CTL_GET_MOUSE_DATA,
    CTL_GET_SPEAKER_VOLUME,
    CTL_SET_SPEAKER_VOLUME,
    CTL_INC_SPEAKER_VOLUME,
    CTL_DEC_SPEAKER_VOLUME,
    CTL_SET_SPEAKER_MUTE,
    CTL_GET_SPEAKER_SAMPLE_RATE,
    CTL_SET_MEDIA_ACCESS_CONTROL_ADDRESS,
    CTL_SET_OVERLAPPED_WRITE_EVENT_UDP_TX_COE,
    CTL_SET_OVERLAPPED_WRITE_EVENT_TCP_TX_COE,
    CTL_OVERLAPPED_PACKET_FORWARD,
    CTL_SET_START_OFFSETS,
    CTL_GET_LINK_STATE,
    CTL_GET_LINK_CHANGE_EVENT,
    CTL_MAC_AUTO_LOAD_CHECK,
    CTL_SET_PORT_NUMBER,
    CTL_SET_LWIP_SOCKET_INDEX,
    CTL_GET_LWIP_SOCKET_INDEX,
    CTL_SET_PROMISCUOUS_MODE,
    CTL_GET_PROMISCUOUS_MODE,
    CTL_SET_LINK_INDICATOR,
    CTL_SET_SYN_PORT_CFG,
    CTL_GET_FIRST_OVERLAPPED_READ_EVENT,
    CTL_GET_OVERLAPPED_READ_EVENT,
    CTL_GET_OVERLAPPED_WRITE_EVENT,
    CTL_STREAM_REQUIRES_AUTHENTICATION,
    CTL_STREAM_SET_CONNECTION_IDLE_CALL_BACK,
    CTL_STREAM_SET_INPUT_PREPARSER,
    CTL_STREAM_UDP_TCP,
    CTL_STREAM_TCP,
    CTL_STREAM_TCP_CONNECTED,
    CTL_GET_STREAM_PAIR_NAME,
    CTL_HID_KEYBOARD,
    CTL_BLANK_CHECK,
    CTL_CLEAR_LCD_SCREEN,
    CTL_SET_LCD_OUTPUT_CURSOR,
    CTL_SET_LCD_USER_CURSOR,
    CTL_SET_LCD_CURSOR_STYLE,
    CTL_SET_MM_DRV,
    CTL_SET_OS_DRV,
    CTL_USBF_IS_CONNECTED,
    CTL_USBF_SET_RW_MODE,
    CTL_USBF_SET_CONFIGURATION,
    CTL_USBF_GET_CONFIGURATION,
    CTL_USBF_SEND_HID_REPORTIN,
    CTL_USBF_START,
    CTL_USBF_STOP,
    /* TODO: add device specific control functions here */
    /* must be last control code, dynamic driver will reuse
       control code from this point forward */
    CTL_NEXT_FREE

} CTLCODE;

typedef enum _FTDERR
{
    SCI_NO_ERROR = 0,
    SCI_RX_ERROR,
    SCI_REQUEST_ERROR,
    SCI_NOT_CLEAR_TO_SEND,
    SCI_OVERRUN_ERROR,
    SCI_CONFIGURATION_ERROR,
    SCI_INVALID_CONFIGURATION,
    SCI_INVALID_CONTROL_CODE,
    SCI_INVALID_PARAMETER,
    SCI_TX_TIME_OUT,
    SCI_NUM_ERRORS
} FTDERR,
*PFTDERR;

typedef enum _SCIERR
{
    DDSCI_OK = 0,
    DDSCI_OVERRUN_ERROR = (1 << 0),
    DDSCI_FRAME_ERROR = (1 << 1),
    DDSCI_PARITY_ERROR = (1 << 2),
    DDSCI_RX_BUFFER_FULL = (1 << 3),
    DDSCI_DEVICE_ERROR = (1 << 4),
    DDSCI_BREAK_ERROR = (1 << 5),
    DDSCI_INVALID_MODE = (1 << 6),
    DDSCI_BAD_BAUD_MATCH = (1 << 7)
} SCIERR;

typedef enum _BLKERR
{
    BULK_EP_NO_ERROR = 0,
    BULK_EP_IO_PENDING,
    BULK_EP_TIME_OUT,
    BULK_EP_DATA_ERROR,
    BULK_EP_REQUEST_ERROR,
    BULK_EP_DEVICE_REMOVED,
    BULK_EP_STALL_ERROR,
    BULK_EP_OVERRUN_ERROR,
    BULK_EP_INVALID_CONTROL_CODE,
    BULK_EP_INVALID_PARAMETER,
    BULK_EP_NUM_ERRORS
} BLKERR,
*PBLKERR;

typedef enum _KBDERR
{
    HID_KBD_OK = 0,
    HID_KBD_MEMORY_ALLOCATION_ERROR,
    HID_KBD_DEVICE_NOT_FOUND,
    HID_KBD_ENDPOINT_NOT_FOUND,
    HID_KBD_REQUEST_ERROR,
    HID_KBD_REPORT_ERROR
} KBDERR;

typedef enum _MSEERR
{
    HID_MSE_OK = 0,
    HID_MSE_MEMORY_ALLOCATION_ERROR,
    HID_MSE_DEVICE_NOT_FOUND,
    HID_MSE_ENDPOINT_NOT_FOUND,
    HID_MSE_REQUEST_ERROR,
    HID_MSE_REPORT_ERROR
} MSEERR;

typedef enum _SPKERR
{
    AC_SPK_OK = 0,
    AC_SPK_MEMORY_ALLOCATION_ERROR,
    AC_SPK_DEVICE_NOT_FOUND,
    AC_SPK_ENDPOINT_NOT_FOUND,
    AC_SPK_REQUEST_ERROR,
    AC_SPK_FAILED_TO_SET_INTERFACE_ERROR,
    AC_SPK_INVALID_DATA_SIZE,
    AC_SPK_INVALID_DATA_ALIGNMENT,
    AC_SPK_DATA_TRANSPORT_ERROR,
    AC_SPK_IO_PENDING
} SPKERR;

typedef enum _CDCERR
{
    CDC_OK = 0,
    CDC_MEMORY_ALLOCATION_ERROR,
    CDC_DEVICE_NOT_FOUND,
    CDC_ENDPOINT_NOT_FOUND,
    CDC_REQUEST_ERROR,
    CDC_FAILED_TO_SET_INTERFACE_ERROR,
    CDC_DATA_TRANSPORT_ERROR,
    CDC_IO_PENDING
} CDCERR;

typedef enum
{
    SINGLE_SHOT_TIMER = 0,
    PERIODIC_TIMER
} TMRTYPE,
*PTMRTYPE;

typedef enum
{
    CURSOR_INVSIBLE,
    CURSOR_UNDERLINE,
    CURSOR_BLOCK
} CSTYLE;

typedef enum
{
    FONT_5X7,
    FONT_6X8,
    FOMT_8X8
} LCDFONT;

/*!< Common error codes supported by all drivers */
typedef enum
{
    DRV_SUCCESS = 0,  /*!< No Error */
    DRV_ERROR = -1,   /*!< General error */
} e_err_code_t;

/* TODO: Add device specific error codes here */

/******************************************************************************
Data structures for driver specific functions
******************************************************************************/


/* Define the configuration bits for the dwConfig field */
#define SCI_PARITY_NONE             ((unsigned long)(1 << 0))
#define SCI_PARITY_EVEN             ((unsigned long)(1 << 1))
#define SCI_PARITY_ODD              ((unsigned long)(1 << 2))
#define SCI_DATA_BITS_SEVEN         ((unsigned long)(1 << 3))
#define SCI_DATA_BITS_EIGHT         ((unsigned long)(1 << 4))
#define SCI_ONE_STOP_BIT            ((unsigned long)(1 << 5))
#define SCI_ONE_HALF_STOP_BIT       ((unsigned long)(1 << 6))
#define SCI_TWO_STOP_BIT            ((unsigned long)(1 << 7))
#define SCI_TXD_NRTS_ENABLE         ((unsigned long)(1 << 8))
#define SCI_DTR_ASSERT              ((unsigned long)(1 << 9))     /* CDC ACM Driver */
#define SCI_DTR_DSR_ENABLE          ((unsigned long)(1 << 9))     /* FTDI Driver only */
#define SCI_RXD_NCTS_ENABLE         ((unsigned long)(1 << 10))
#define SCI_RTS_ASSERT              ((unsigned long)(1 << 11))    /* CDC ACM Driver */
#define SCI_RTS_CTS_ENABLE          ((unsigned long)(1 << 11))    /* FTDI Driver only */
#define SCI_SYNCHRONOUS             ((unsigned long)(1 << 12))
#define SCI_MULTI_PROCESSOR         ((unsigned long)(1 << 13))
#define SCI_SYNCCLOCK_INPUT         ((unsigned long)(1 << 14))
#define SCI_X16CLOCK_INPUT          ((unsigned long)(1 << 15))
#define SCI_ASYNCLOCK_OUTPUT        ((unsigned long)(1 << 16))

#pragma pack(1)

/** Control structure for CTL_SCI_GET_LINE_STATUS */
typedef struct SCILST
{
    /* Input signals */
    unsigned short clearToSend:1;
    unsigned short dataSetReady:1;
    unsigned short ringIndicator:1;
    unsigned short receiveLineSignalDetect:1;

    /* Status indicators */
    unsigned short dataReady:1;
    unsigned short overrunError:1;
    unsigned short parityError:1;
    unsigned short frameError:1;
    unsigned short breakSignal:1;
    unsigned short breakOutput:1;
    unsigned short :6;
} SCILST,
*PSCILST;

/** Define a type for the timer call-back function to prevent the code from
   looking over complicated */
typedef void (*PFNTMR)(void*);
typedef void (*const PCFNTMR)(void*);

/** Control structure for CTL_START_CALLBACK_TIMER and CTL_START_EVENT_TIMER */
typedef struct _CBTMR
{
    PFNTMR      pTimerFunction;
    void        *pvParameter;
    uint32_t    ulTime;
    TMRTYPE     Type;
    void        *pvTimerHandle;
} CBTMR,
*PCBTMR;

/** Control structure for CTL_GET_TIME_STAMP */
typedef struct _TMSTMP
{
    uint32_t   ulMilisecond;
    uint16_t   usSubMilisecond;
} TMSTMP,
*PTMSTMP;


/** Control structure for CTL_SET_OVERLAPPED_READ_EVENT, CTL_SET_OVERLAPPED_WRITE_EVENT
   and CTL_GET_OVERLAPPED_READ_RESULT */
typedef struct _OLD
{
    /* Pointer to the event used to signal the completion of the IO */
    void   *pEvent;

    /* The number of bytes transferred */
    size_t  stLength;

    /* The result of the transfer */
    int     iResult;
} OLD,
*POLD;

/** Control structure for CTL_GET_ERROR_STRING */
typedef struct _ERRSTR
{
    int     iErrorCode;
    int8_t  *pszErrorString;
} ERRSTR,
*PERRSTR;

/** Control structure for CTL_GET_DATE and CTL_SET_DATE */
typedef union _DATE
{
    unsigned long long llCode;
    struct
    {
        unsigned long long WeekDay :3;
        unsigned long long Year    :35;
        unsigned long long Month   :4;
        unsigned long long Day     :5;
        unsigned long long Hour    :5;
        unsigned long long Minute  :6;
        unsigned long long Second  :6;
    } Field;
} DATE,
*PDATE;

/** The Unix time structure - for compatibility */
typedef struct tm2
{
    int tm_sec;     /* seconds after the minute - [0,59] */
    int tm_min;     /* minutes after the hour - [0,59] */
    int tm_hour;    /* hours since midnight - [0,23] */
    int tm_mday;    /* day of the month - [1,31] */
    int tm_mon;     /* months since January - [0,11] */
    int tm_year;    /* years since 1900 */
    int tm_wday;    /* days since Sunday - [0,6] */
    int tm_yday;    /* days since January 1 - [0,365] */
    int tm_isdst;   /* daylight savings time flag */
} U_TIME,
*PU_TIME;

#pragma pack(1)

/** Control structure for CTL_GET_MOUSE_DATA */
typedef union _MDAT
{
    unsigned long ulData;
    struct
    {
        union
        {
            uint8_t   byButtonMap;
            struct
            {
        #ifdef _BITFIELDS_MSB_FIRST_
                uint8_t :5;
                uint8_t Middle:1;
                uint8_t Right:1;
                uint8_t Left:1;
        #else
                uint8_t Left:1;
                uint8_t Right:1;
                uint8_t Middle:1;
                uint8_t :5;
        #endif
            } Field;
        } Buttons;
        int8_t    deltaX;
        int8_t    deltaY;
        int8_t    deltaScroll;
    } Part;
} MDAT,
*PMDAT;

#pragma pack()

/** Control structure for CTL_GET_SPEAKER_CONTROL & CTL_SET_SPEAKER_CONTROL */
typedef struct _SPKDAT
{
    short           siMaxVol;
    short           siMinVol;
    short           siVolSteps;
    short           siStepSize;
    uint8_t         bfMute;
} SPKDAT,
*PSPKDAT;

/** Control structure for CTL_SET_START_OFFSETS */
typedef struct _ETSOFF
{
    int         iRxStartOffset;
    int         iTxStartOffset;
} ETSOFF,
*PETSOFF;

/** Control structure for CTL_STREAM_SET_CONNECTION_IDLE_CALL_BACK */
typedef void (*PFNCB)(uint32_t, void*);
typedef struct _IDLECB
{
    PFNCB       pCallBack;
    uint32_t    uiTaskID;
    void        *pvParameter;
} IDLECB,
*PIDLECB;


/** The control structure for CTL_STREAM_SET_INPUT_PREPARSER */
typedef void (*PFNPKT)(uint8_t *pbyFrame, uint16_t usLength);

/* TODO: Add device specific control structures here */

/** Version Information for drivers (high or low level little endian) */
typedef struct
{
    union
    {
        uint32_t full;      /*!< Major + Minor combined as 1 uint32_t data member  */
        struct
        {
          uint16_t minor;    /*!< Version, modified by Product Owner  */
          uint16_t major;    /*!< Version, modified by developer  */
        } sub;
    } version;
    uint32_t build;          /*!< Build Number Generated during the release   */

    /* The driver name */
    const char   *p_szdriver_name;
} st_drv_info_t;


/******************************************************************************
 * Version Info Structure
 ******************************************************************************/
typedef struct
{
    st_drv_info_t hld;
    st_drv_info_t lld;
} st_ver_info_t, *st_ver_info_ptr_t;

#pragma pack()

/******************************************************************************
Function Prototypes
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* The functions prefixed low_ should only be called when the CPU is in supervisor mode */

/**
 * @brief  ANSI library low level open function
 * @param  pszFileName - Pointer to the file name
 * @param  pdev - device descriptor
 * @return File number or -1 on error
 */
extern int low_register(char *pszFileName, void *pdev);

/**
 * @brief  ANSI library low level close function
 * @param  pszFileName - Pointer to the file name
 * @return File number or -1 on error
 */
extern int low_unregister(char *pszFileName);


#ifdef __cplusplus
}
#endif

#endif /* CONTROL_H_INCLUDED */

/******************************************************************************
End  Of File
******************************************************************************/
