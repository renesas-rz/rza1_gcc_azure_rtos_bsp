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
 ******************************************************************************
 * Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************
 * File Name    : r_sound_sample_main.c
 * Device(s)    : RZ/A1L
 * Tool-Chain   : GNUARM-NONE-EABI-v16.01
 * H/W Platform : Stream it! v2 board
 * Description  : Audio Record and Playback sample
 ******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 13.06.2018 1.00 First Release
 *****************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 *****************************************************************************/
/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "r_typedefs.h"
#include "tx_api.h"
#include "r_os_abstraction_api.h"
#include "r_task_priority.h"
#include "dev_drv.h"

#include "console.h"
#include "trace.h"

/* GNU Compiler settings */
#include "compiler_settings.h"

/* Sound API */
#include "r_sound_app.h"
#include "sound_if.h"
#include "r_sound.h"
#include "r_ssif_drv_api.h"
#include "ssif.h"

/******************************************************************************
 Macro definitions
 ******************************************************************************/

/* Playback demo settings */
#include "LR_44_1K16B_S.dat"                /* sound data for playback application */
#define WAVEDATA_PRV_                       (LR_44_1K16B_S)
#define SIZEOF_WAVEDATA_PRV_                (sizeof(LR_44_1K16B_S))
#define WAVE_DMA_SIZE_PRV_                  (4096)  /* size of blocks for data operations with SSIF/DMA */
#define WAVEDATA_STORAGE_ALIGN_BYTES_PRV_   (32)    /* buffer alignment required for dma access */

/* Record/play demo settings */
#define NUM_AUDIO_BUFFER_BLOCKS_PRV_        (3)
#define REC_DMA_SIZE_PRV_                   (512)
#define SIZEOF_WAVEDATA_RECPLAY_PRV_        (REC_DMA_SIZE_PRV_ * NUM_AUDIO_BUFFER_BLOCKS_PRV_)

/* Comment this line out to turn ON module trace in this file */
/* #undef _TRACE_ON_ */

#ifndef _TRACE_ON_
    #undef TRACE
    #define TRACE(x)
#endif

/* SSIF Channel */
#define STREAM_IT_SOUND_CHANNEL_PRV_        (0)

/*******************************************************************************
 Typedef definitions
 ******************************************************************************/
typedef struct st_sound_config_t
{
    bool_t   initialised; /* Control structure for audio system shared between all audio applications */
    bool_t   inuse; /* Audio system in use, audio commands must hold inuse until lock on audio is released */
    event_t  task_running; /* ensures only 1 audio task can run in the system */

    uint8_t  *p_playback_data; /* ptr to play back buffer */
    uint8_t  *p_record_data; /* ptr to record buffer */

    psemaphore_t playback_semaphore; /* semaphore to control playback */
    psemaphore_t record_semaphore; /* semaphore to control record   */

    uint32_t ul_delaytime_ms;
} st_sound_config_t;

typedef st_sound_config_t *p_sound_config_t;

/*******************************************************************************
 Exported global variables (to be accessed by other files)
 ******************************************************************************/

/*******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static void userdef_aio_callback (union sigval event);

/* sound_config structure instance */
static st_sound_config_t gs_sound_t;

/* pointer to st_sound_config_t variable type */
static p_sound_config_t gsp_sound_control_t;

static void task_play_sound_demo ( ULONG parameters);
static void task_record_sound_demo ( ULONG parameters);
static int32_t configure_audio (void);
static void close_audio (void);

/* dma read/write to SSIF callback functions */
static void userdef_tx_callback (union sigval signo);
static void userdef_rx_callback (union sigval signo);

/* flags used to control read/write operations with SSIF */
static volatile bool_t gs_rx_set_flag = 0u;
static volatile bool_t gs_tx_set_flag = 0u;
static volatile bool_t gs_rx_first_return_flag = 0u;
static volatile bool_t gs_tx_first_set_flag = 0u;

/* handle for SSIF driver */
static int_t gs_ssif_handle = -1;

static FILE *p_gout;
static FILE *p_gin;

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: initalise_control_if
 * Description  : Checks to see that the control structure has been initailised.
 *                Multiple calls to this function will only initialise the structure once.
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
static void initalise_control_if (void)
{
    if (0 == gsp_sound_control_t->initialised)
    {
        gsp_sound_control_t->initialised = true;
        gsp_sound_control_t->inuse = false;
        gsp_sound_control_t->ul_delaytime_ms = 10;

        gsp_sound_control_t->p_playback_data = 0;
        gsp_sound_control_t->p_record_data = 0;

        gsp_sound_control_t->playback_semaphore = 0;
        gsp_sound_control_t->record_semaphore = 0;

        R_OS_CreateEvent( &gsp_sound_control_t->task_running);

        printf("initalise_control_if\r\n");
    }
}
/***********************************************************************************************************************
 End of function initalise_control_if
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: task_play_sound_demo
 * Description  :
 * Arguments    :
 * Return Value :
 **********************************************************************************************************************/
static void task_play_sound_demo (ULONG parameters)
{
    int32_t res = DEVDRV_SUCCESS;

    R_OS_SetEvent( gsp_sound_control_t->task_running);

    fprintf(p_gout, "task_play_sound_demo playing\r\n");

    /* open SSIF driver for read and write */
    gs_ssif_handle = open(DEVICE_INDENTIFIER "ssif", O_WRONLY);

    /* initialise SSIF and Sound driver for audio streaming */
    res = configure_audio();

    /* Sound - set sampling rate */
    if (DEVDRV_SUCCESS == res)
    {
        res = R_SOUND_SetSamplingRate(STREAM_IT_SOUND_CHANNEL_PRV_, SOUND_FREQ_44100);
    }

    /* Sound - set Volume in percent */
    if (DEVDRV_SUCCESS == res)
    {
        res = R_SOUND_SetVolume(STREAM_IT_SOUND_CHANNEL_PRV_, SOUND_MIC_VOL_40_PERCENT);
    }

    /* Sound - Set Microphone Volume in percent */
    if (DEVDRV_SUCCESS == res)
    {
        res = R_SOUND_SetMicVolume(STREAM_IT_SOUND_CHANNEL_PRV_, SOUND_MIC_VOL_40_PERCENT);
    }

    /*******************************************************************/
    /* Playback start                                                  */
    /*******************************************************************/
    if (DEVDRV_SUCCESS == res)
    {
        /* Array of message blocks for DMA control of buffer writes */
        AIOCB aiocb[NUM_AUDIO_BUFFER_BLOCKS_PRV_];
        uint32_t loop = 0u;

        /* Create semaphore to control buffer access */
        R_OS_CreateSemaphore( &gsp_sound_control_t->playback_semaphore, 0);
        while ( 1 ) {

			/* Pass through the data, writing out to the SSIF */
			for (loop = 0; (loop * WAVE_DMA_SIZE_PRV_) < ((uint32_t) SIZEOF_WAVEDATA_PRV_ - WAVE_DMA_SIZE_PRV_); loop++)
			{
				/* Get the current element in the aiocb message array */
				int_t div = (int_t) (loop % NUM_AUDIO_BUFFER_BLOCKS_PRV_);

				/* register access semaphore */
				aiocb[div].aio_sigevent.sigev_value.sival_ptr = (void *) gsp_sound_control_t->playback_semaphore;

				/* register user callback function after dma transfer to SSIF */
				aiocb[div].aio_sigevent.sigev_notify_function = &userdef_aio_callback;

				/* Queueing request #(NUM_AUDIO_BUFFER_BLOCKS_PRV_ -1) to #N */
				control(gs_ssif_handle, R_SSIF_AIO_WRITE_CONTROL, &aiocb[div]);
				write(gs_ssif_handle, &WAVEDATA_PRV_[loop * WAVE_DMA_SIZE_PRV_], WAVE_DMA_SIZE_PRV_);

				/* Waiting complete request #0to#(N-2) */
				R_OS_WaitForSemaphore( gsp_sound_control_t->playback_semaphore, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);
			}
			fprintf(p_gout, "task_play_sound_demo complete\r\n");

        }
    }
    else
    {
        fprintf(p_gout, "unable to run task_play_sound_demo\r\n");
    }
    R_OS_DeleteSemaphore( gsp_sound_control_t->playback_semaphore);

    /* close down audio operations */
    close_audio();
    close(gs_ssif_handle);

}
/***********************************************************************************************************************
 End of function task_play_sound_demo
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: task_record_sound_demo
 * Description  : This task records from the MIC connector on the board and plays the received audio back to the
 *                LINE OUT
 * Arguments    : void *parameters - task parameter - not used
 * Return Value : void
 **********************************************************************************************************************/
static void task_record_sound_demo (ULONG parameters)
{
    uint32_t div;
    uint32_t txi_data = 0;
    uint32_t txi_aio = 0;
    uint32_t rxi_data = 0;
    uint32_t rxi_aio = 0;
    uint32_t loop;

    /* message blocks for transmit and receive */
    AIOCB rx_aiocb[NUM_AUDIO_BUFFER_BLOCKS_PRV_];
    AIOCB tx_aiocb[NUM_AUDIO_BUFFER_BLOCKS_PRV_];

    /* unused argument */
    UNUSED_PARAM(parameters);

    /* populate transmit aiocb message blocks as part of this initialisation process (but not sending yet) */
    for (loop = 0u; loop < NUM_AUDIO_BUFFER_BLOCKS_PRV_; loop++)
    {
        /* register access semaphore */
        tx_aiocb[loop].aio_sigevent.sigev_value.sival_ptr = (void *) &gsp_sound_control_t->playback_semaphore;

        /* register user callback function after dma transfer to SSIF */
        tx_aiocb[loop].aio_sigevent.sigev_notify_function = &userdef_tx_callback;
    }

    /* initialise each read message block and read from SSIF to fill buffer with audio data */
    for (loop = 0u; loop < NUM_AUDIO_BUFFER_BLOCKS_PRV_; loop++)
    {
        /* register access semaphore */
        rx_aiocb[loop].aio_sigevent.sigev_value.sival_ptr = (void *) &gsp_sound_control_t->record_semaphore;

        /* register user callback function after dma transfer from SSIF */
        rx_aiocb[loop].aio_sigevent.sigev_notify_function = &userdef_rx_callback;

        /* Update SSIF configuration */
        control(gs_ssif_handle, R_SSIF_AIO_READ_CONTROL, (void *) &rx_aiocb[loop]);

        /* read data from SSIF to buffer block */
        read(gs_ssif_handle, &gsp_sound_control_t->p_record_data[loop * REC_DMA_SIZE_PRV_], REC_DMA_SIZE_PRV_);

        /* update buffer/message tracking variables */
        rxi_data++;
        rxi_aio++;
    }

    /* reset flags prior to loop */
    gs_tx_set_flag = false;
    gs_rx_set_flag = false;
    gs_rx_first_return_flag = false;
    gs_tx_first_set_flag = false;

    /* audio loop, exited by keypress on the console (implemented in calling task) */
    while (1)
    {
        /* if audio receive dma has finished, update pointers and re-start dma transfer */
        if (false != gs_rx_set_flag)
        {
            /* use correct block */
            div = rxi_aio % NUM_AUDIO_BUFFER_BLOCKS_PRV_;

            /* Update SSIF configuration */
            control(gs_ssif_handle, R_SSIF_AIO_READ_CONTROL, (void *) &rx_aiocb[div]);

            /* read from SSIF into block */
            read(gs_ssif_handle, &gsp_sound_control_t->p_record_data[rxi_data * REC_DMA_SIZE_PRV_], REC_DMA_SIZE_PRV_);

            /* update rx buffer/message tracking variables */
            rxi_data++;
            rxi_aio++;

            /* check for buffer boundary overflow */
            if ((SIZEOF_WAVEDATA_RECPLAY_PRV_ / REC_DMA_SIZE_PRV_) < rxi_data)
            {
                rxi_data = 0;
            }

            gs_rx_set_flag = false;
        }

        /* Start transmission of audio from via SSIF if there is at least 1  */
        if ((false != gs_rx_first_return_flag) && (false == gs_tx_first_set_flag))
        {
            /* initialise each message block and write from buffer to SSIF to fill buffer with data */
            for (loop = 0u; loop < NUM_AUDIO_BUFFER_BLOCKS_PRV_; loop++)
            {
                /* Update SSIF configuration */
                control(gs_ssif_handle, R_SSIF_AIO_WRITE_CONTROL, (void *) &tx_aiocb[loop]);

                /* write from block to SSIF */
                write(gs_ssif_handle, &gsp_sound_control_t->p_playback_data[loop * REC_DMA_SIZE_PRV_],
                REC_DMA_SIZE_PRV_);

                /* update tx buffer/message tracking variables */
                txi_data++;
                txi_aio++;
            }

            /* prevent future access of this block */
            gs_tx_first_set_flag = true;
        }

        /* if audio transmit dma has finished, update pointers and re-start dma transfer */
        if (false != gs_tx_set_flag)
        {
            /* point to next buffer area */
            div = txi_aio % NUM_AUDIO_BUFFER_BLOCKS_PRV_;

            /* Update SSIF configuration */
            control(gs_ssif_handle, R_SSIF_AIO_WRITE_CONTROL, (void *) &tx_aiocb[div]);

            /* write from block to SSIF */
            write(gs_ssif_handle, &gsp_sound_control_t->p_playback_data[txi_data * REC_DMA_SIZE_PRV_],
            REC_DMA_SIZE_PRV_);

            /* update tx buffer/message tracking variables */
            txi_data++;
            txi_aio++;

            /* check for buffer boundary overflow */
            if ((SIZEOF_WAVEDATA_RECPLAY_PRV_ / REC_DMA_SIZE_PRV_) < txi_data)
            {
                txi_data = 0;
            }

            gs_tx_set_flag = false;
        }
    }
}
/***********************************************************************************************************************
 End of function task_record_sound_demo
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: play_file_data
 * Description  :
 * Arguments    :
 * Return Value :
 **********************************************************************************************************************/
static void play_file_data (FILE *p_in, FILE *p_out, void *p_data)
{
    bool_t user_abort = false;
    os_task_t *p_task = 0;

    /* unused argument */
    UNUSED_PARAM(p_data);

    p_gout = p_out;
	p_gin = p_in;

    int_t i_in = R_DEVLINK_FilePtrDescriptor(p_in);

    fprintf(p_out, "Play Sound sample program start\r\n");
    fprintf(p_out, "Press any key to terminate demo\r\n");

    /* Create play task (to normalise calling task) */
    p_task = R_OS_CreateTask("play sound", task_play_sound_demo, NULL,
    		1000,
    TASK_PLAY_SOUND_APP_PRI);

    if (p_task)
    {
        while (true != user_abort)
        {

        	// Wait 5 seconds before playing sound
            R_OS_TaskSleep(5000);

        }
        R_OS_DeleteTask(p_task);
        /* close down audio operations */
        close_audio();
    }
    fprintf(p_out, "Play Sound sample complete\r\n");
}
/***********************************************************************************************************************
 End of function play_file_data
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_SOUND_PlaySample
 * Description  : Play Sound application task
 Play pre-recorded sound sample (LR_44_1K16B_S.dat)
 * Arguments    : FILE * pIn  Standard input from console.
 *                FILE * pOut Standard output to console
 * Return Value : none
 ***********************************************************************************************************************/
void R_SOUND_PlaySample (FILE *p_in, FILE *p_out)
{
    int_t i_in = R_DEVLINK_FilePtrDescriptor(p_in);

    /* set control structure pointer to holding structure */
    gsp_sound_control_t = &gs_sound_t;

    /* initialise the control structure for this group of applications */
    initalise_control_if();

    if ( !gsp_sound_control_t->inuse)
    {
        gsp_sound_control_t->inuse = true;

        fprintf(p_out, "NEEDS SOURCE BUFFER ie LR_44_1K16B_S.dat \r\n");
        play_file_data(p_in, p_out, 0 /* LR_44_1K16B_S */);

        gsp_sound_control_t->inuse = false;
    }
    else
    {
        fprintf(p_out, "SSIF in use can not complete command\r\n");
    }
}
/***********************************************************************************************************************
 End of function R_SOUND_PlaySample
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: play_recorded
 * Description  : Setup and run record/playback demo
 * Arguments    : FILE * pIn  Standard input from console.
 *                FILE * pOut Standard output to console
 * Return Value : none
 ***********************************************************************************************************************/
static void play_recorded (FILE *p_in, FILE *p_out)
{
    bool_t user_abort = false;
    os_task_t *p_task = 0;
    void *p_wavebuf_non_aligned;
    int32_t res = DEVDRV_SUCCESS;

    int_t i_in = R_DEVLINK_FilePtrDescriptor(p_in);

    /* May be required later */
    UNUSED_PARAM(p_in);

    fprintf(p_out, "Play/record sample program start\r\n");

    /* setup record/playback buffers */
    p_wavebuf_non_aligned = R_OS_AllocMem(
            ((WAVE_DMA_SIZE_PRV_ * NUM_AUDIO_BUFFER_BLOCKS_PRV_) + WAVEDATA_STORAGE_ALIGN_BYTES_PRV_),
            R_REGION_LARGE_CAPACITY_RAM);

    /* ensure buffer has been allocated */
    if (NULL == p_wavebuf_non_aligned)
    {
        res = DEVDRV_ERROR;
        fprintf(p_out, "Sound sample could not allocate memory for buffer\r\n");
    }

    else
    {
        /* The buffer used needs to be aligned to WAVEDATA_STORAGE_ALIGN_BYTES_PRV_ in order for DMA to work.
         * Aligned by clearing lower bits of pointer then adding the alignment size.
         * The malloc for the buffer area has taken this alignment process into account. */
        gsp_sound_control_t->p_record_data = (void *) (((uint32_t) p_wavebuf_non_aligned)
                & (uint32_t) ( ~(WAVEDATA_STORAGE_ALIGN_BYTES_PRV_ - 1)));

        /* alignment of buffer part 2 */
        gsp_sound_control_t->p_record_data = (void *) ((uint8_t *) gsp_sound_control_t->p_record_data
                + WAVEDATA_STORAGE_ALIGN_BYTES_PRV_);

        /* record and playback are from the same buffer */
        gsp_sound_control_t->p_playback_data = gsp_sound_control_t->p_record_data;

        /* show status of demo as running */
        R_OS_SetEvent( &gsp_sound_control_t->task_running);

        /* setup semaphores for read and write access control */
        /* semaphore for receive DMA from SSIF peripheral to buffer */
        if (true != R_OS_CreateSemaphore(&gsp_sound_control_t->record_semaphore, 1))
        {
            res = DEVDRV_ERROR;
        }
        else
        {
            /* playback and record are on the same buffer so should have same access */
            gsp_sound_control_t->playback_semaphore = gsp_sound_control_t->record_semaphore;
        }

        if (DEVDRV_SUCCESS == res)
        {
            /* open SSIF driver for read and write */
            gs_ssif_handle = open(DEVICE_INDENTIFIER "ssif", O_RDWR);

            if (0 >= gs_ssif_handle)
            {
                fprintf(p_out, "Could not open ssif driver\r\n");
                res = DEVDRV_ERROR;
            }
        }

        /* initialise SSIF and Sound driver for audio streaming */
        if (DEVDRV_SUCCESS == res)
        {
            res = configure_audio();
        }

        /* Sound - set sampling rate */
        if (DEVDRV_SUCCESS == res)
        {
            res = R_SOUND_SetSamplingRate(STREAM_IT_SOUND_CHANNEL_PRV_, SOUND_FREQ_44100);
        }

        /* Sound - set Volume in percent */
        if (DEVDRV_SUCCESS == res)
        {
            res = R_SOUND_SetVolume(STREAM_IT_SOUND_CHANNEL_PRV_, 1);
        }

        /* Sound - Set Microphone Volume in percent */
        if (DEVDRV_SUCCESS == res)
        {
            res = R_SOUND_SetMicVolume(STREAM_IT_SOUND_CHANNEL_PRV_, 1);
        }

        /* Wait to finish Codec initializing */
        R_OS_TaskSleep(2000);

        if (DEVDRV_SUCCESS == res)
        {
            fprintf(p_out, "Press any key to terminate demo\r\n");

            /* Create play task (to normalise calling task) */
            p_task = R_OS_CreateTask("rec sound", task_record_sound_demo, p_out,
            R_OS_ABSTRACTION_PRV_SMALL_STACK_SIZE,
            TASK_RECORD_SOUND_APP_PRI);

            if (p_task)
            {
                uint32_t count=0;
                while (true != user_abort)
                {
                    R_OS_TaskSleep(50);

                    if( count < 40 )
                    {
                        count++;
                        res = R_SOUND_SetVolume(STREAM_IT_SOUND_CHANNEL_PRV_, count);
                        res = R_SOUND_SetMicVolume(STREAM_IT_SOUND_CHANNEL_PRV_, count);
                    }

                    /* If a console key has been pressed or task has completed then quit */
                    if (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) != 0)
                    {
                        user_abort = true;

                        /* set task quit event to signal to created task to stop */
                        /* Note this "rec sound" task will delete itself once de-initialised */
                        R_OS_ResetEvent( &gsp_sound_control_t->task_running);
                        fprintf(p_out, "record sample user quit\r\n");
                        fgetc(p_in);
                    }
                }
            }
            fprintf(p_out, "Record Sound sample complete\r\n");

            /* close down audio operations */
            close_audio();

            /* Delete the record/playback task */
            R_OS_DeleteTask(p_task);
        }

        /* Ensure that the record control semaphore is deleted */
        R_OS_DeleteSemaphore((gsp_sound_control_t->record_semaphore));

        /* Free memory buffer used for recording/playback */
        R_OS_FreeMem(p_wavebuf_non_aligned);

        /* demo quitting */
        R_OS_ResetEvent( &gsp_sound_control_t->task_running);
    }
}
/*******************************************************************************
 End of function play_recorded
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_SOUND_RecordSample
 * Description  : Run the record/playback Sound application
 * Arguments    : FILE * pIn  Standard input from console.
 *                FILE * pOut Standard output to console
 * Return Value : none
 ***********************************************************************************************************************/
void R_SOUND_RecordSample (FILE *p_in, FILE *p_out)
{
    int_t i_in = R_DEVLINK_FilePtrDescriptor(p_in);

    /* May be required later */
    UNUSED_PARAM(p_in);

    /* flush any remaining input */
    while (control(i_in, CTL_GET_RX_BUFFER_COUNT, NULL) != 0)
    {
        fgetc(p_in);
    }

    /* set control structure pointer to holding structure */
    gsp_sound_control_t = &gs_sound_t;

    /* initialise the control structure for this group of applications */
    initalise_control_if();

    /* only run demo if audio is not in use */
    if ( !gsp_sound_control_t->inuse)
    {
        gsp_sound_control_t->inuse = true;

        play_recorded(p_in, p_out);

        gsp_sound_control_t->inuse = false;
    }
    else
    {
        fprintf(p_out, "SSIF in use can not complete command\r\n");
    }
}
/***********************************************************************************************************************
 End of function R_SOUND_RecordSample
 **********************************************************************************************************************/

/*******************************************************************************
 * Function Name: configure_audio
 * Description  : Configures SOUND, SSIF for record/playback operations
 * Arguments    : void
 * Return Value : int32_t DEV_DRV_SUCCESS: OK
 *                        DEVDRV_ERROR:    Error
 ******************************************************************************/
static int32_t configure_audio (void)
{
    int32_t res = DEVDRV_ERROR;

    /* Initialise SOUND module */
    res = R_SOUND_Init();

    /* Open SOUND */
    if (DEVDRV_SUCCESS == res)
    {
        res = R_SOUND_Open(STREAM_IT_SOUND_CHANNEL_PRV_);
    }

    /* return status of operation */
    return (res);
}
/*******************************************************************************
 End of function configure_audio
 ******************************************************************************/

/*******************************************************************************
 * Function Name: close_audio
 * Description  : Closes down SOUND driver, SSIF operations
 * Arguments    : void
 * Return Value : void
 ******************************************************************************/
static void close_audio (void)
{
    /* Close DAC */
    R_SOUND_Close(STREAM_IT_SOUND_CHANNEL_PRV_);

    /* De-Init Sound driver */
    R_SOUND_UnInit();

    /* stop SSIF driver */
    close(gs_ssif_handle);
}
/*******************************************************************************
 End of function close_audio
 ******************************************************************************/

/**************************************************************************//**
 * Function Name: userdef_tx_callback
 * @brief         SSIF driver : transfer request end callback function
 *
 *                Description:<br>
 *                SCIF transmit request end callback function
 * @param[in]     signo.sival_ptr : semaphore id
 * @retval        none
 ******************************************************************************/
static void userdef_tx_callback (union sigval signo)
{
    /* function argument is unused */
    UNUSED_PARAM(signo);

    gs_tx_set_flag = true;
}
/*******************************************************************************
 End of function userdef_tx_callback
 *******************************************************************************/

/**************************************************************************//**
 * Function Name: userdef_rx_callback
 * @brief         SSIF driver : receive request end callback function
 *
 *                Description:<br>
 *                SCIF receive request end callback function
 * @param[in]     signo.sival_ptr : semaphore id
 * @retval        none
 ******************************************************************************/
static void userdef_rx_callback (union sigval signo)
{
    /* function argument is unused */
    UNUSED_PARAM(signo);

    gs_rx_set_flag = true;
    gs_rx_first_return_flag = true;
}
/*******************************************************************************
 End of function userdef_rx_callback
 *******************************************************************************/

/**************************************************************************/
/**
 * Function Name: userdef_aio_callback
 * @brief         SSIF driver : request end callback function
 *
 *                Description:<br>
 *                SCIF transmit request end callback function
 * @param[in]     signo.sival_ptr : semaphore id
 * @retval        none
 ******************************************************************************/
static void userdef_aio_callback (union sigval signo)
{
    R_OS_ReleaseSemaphore(signo.sival_ptr);
}
/*******************************************************************************
 End of function userdef_aio_callback
 ******************************************************************************/
