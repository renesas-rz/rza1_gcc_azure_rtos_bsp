/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_os_abstraction.c
 * Version      : see OS_LESS_RZ_HLD_VERSION_MAJOR.OS_LESS_RZ_HLD_VERSION_MINOR
 * Description  : To make efficient code re-use the identical high layer driver shall be used in both OS and
 *                OS Less applications.
 *                This file aims to abstract the Operating system (OS) awareness when creating an OS Less driver.
 ***********************************************************************************************************************/
#if (R_RTOS_ABSTRACTION == R_RTOS_ABSTRACTION_FREERTOS)
#include <stdio.h>
#include <fcntl.h>

#include "r_typedefs.h"
#include "r_devlink_wrapper.h"

#include "FreeRTOS.h"
#include "FreeRTOSconfig.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"
#include "string.h"

/* compiler specific API header */
#include "r_compiler_abstraction_api.h"

/* OS abstraction specific API header */
#include "r_os_abstraction_api.h"

#include "r_task_priority.h"
#include "main.h"
#include "trace.h"

/* Comment this line out to turn ON module trace in this file */
#undef _TRACE_ON_

#ifndef _TRACE_ON_
    #undef TRACE
    #define  TRACE(x)
#endif

/* A minimum of 8 characters or "Unknown" will not fit! */
#define R_OS_PRV_TINY_STACK_SIZE          (configMINIMAL_STACK_SIZE)
#define R_OS_PRV_SMALL_STACK_SIZE         (configSMALL_STACK_SIZE)
#define R_OS_PRV_DEFAULT_STACK_SIZE       (configDEFAULT_STACK_SIZE)
#define R_OS_PRV_LARGE_STACK_SIZE         (configDEFAULT_STACK_SIZE*2)
#define R_OS_PRV_HUGE_STACK_SIZE          (configDEFAULT_STACK_SIZE*4)

#define R_OS_PRV_DEFAULT_HEAP             (R_REGION_LARGE_CAPACITY_RAM)

#define R_OS_PRV_INFINITE_DELAY               (portMAX_DELAY)

#define MIRROR_HEAP_START           ((void *) &_ld_mirrored_heap_start)
#define MIRROR_HEAP_END             ((void *) &_ld_mirrored_heap_end)
#define MIRROR_HEAP_LENGTH			(&_ld_mirrored_heap_end - &_ld_mirrored_heap_start)

#define UNCACHED_HEAP_START         ((void *) &_ld_uncached_heap_start)
#define UNCACHED_HEAP_END           ((void *) &_ld_uncached_heap_end)
#define UNCACHED_HEAP_LENGTH		(&_ld_uncached_heap_end - &_ld_uncached_heap_start)

/* Note ucHeap is used in freeRTOS for fixed location fixed sized memory pool */
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ((section(".OS_RAM_SECTION")));

extern uint8_t _ld_mirrored_heap_start;
extern uint8_t _ld_mirrored_heap_end;

extern uint8_t _ld_uncached_heap_start;
extern uint8_t _ld_uncached_heap_end;
extern void pvPortsetDesiredBlockForMalloc( size_t xWantedBlock );
extern void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions );

void vApplicationStackOverflowHook (xTaskHandle pxTask, char *pcTaskName);
void vApplicationIdleHook (void);


/* Allocate two blocks of RAM for use by the heap. */
HeapRegion_t xHeapRegions[] =
{
    { ( uint8_t * ) MIRROR_HEAP_START, 0},
    { ( uint8_t * ) UNCACHED_HEAP_START, 0},
    { NULL, 0 } /* Terminates the array. */
};

uint32_t xNumRegions = (sizeof(xHeapRegions) / sizeof(HeapRegion_t));

/*****************************************************************************
 Function Macros
 ******************************************************************************/

/*****************************************************************************
 Typedefs
 ******************************************************************************/

/*
 static const st_drv_info_t gs_os_version =
 {
 { ((tskKERNEL_VERSION_MAJOR << 16) + tskKERNEL_VERSION_MINOR) },
 tskKERNEL_VERSION_BUILD,
 ("FreeRTOS")
 };
 */

#ifdef LOG_TASK_INFO
    #define LOG_DATA_DUMP_SIZE_PRV_     (4096)
#endif /* LOG_TASK_INFO */

static const char gs_startup_task_name_str[] = "Main";

static xSemaphoreHandle s_pvStreamSemaphore = NULL;
static void *s_pvTaskHandle = NULL;

extern uint32_t ulPortInterruptNesting;
UBaseType_t uxSavedInterruptStatus;

static volatile char s_pcFile[200];
static volatile unsigned long s_ulLine;

/* local functions */

/* LOG_TASK_INFO provides logging of information into a circular buffer. This is currently
 * configured to do so in mallocs and frees.
 * To use: Add text to log using the log_data function. The log_write_data_to_console will
 * output the contents of the buffer to the console (currently set to do this in the
 * OS_assert function).
*/
#ifdef LOG_TASK_INFO
static volatile char gs_log_data[LOG_DATA_DUMP_SIZE_PRV_ + 1];
static volatile char *p_log_head = gs_log_data;
static volatile char *p_log_tail = gs_log_data;
static volatile uint8_t log_readback_running = 0u;

/*******************************************************************************
 * Function Name: log_data
 * Description  : Logs text to a circular buffer for debug purposes
 * Arguments    : char *p_data_to_log
 *                uint32_t len_data
 * Return Value : void
 ******************************************************************************/
static void log_data (char *p_data_to_log, uint32_t len_data)
{
    uint32_t len = 0;

    /* determine length of data to log */
    if (0 == len_data)
    {
        /* using strlen */
        len = strlen(p_data_to_log);
    }
    else
    {
        len = len_data;
    }

    /* will head fit in buffer without overlap */
    if ((p_log_head + len) < (gs_log_data + LOG_DATA_DUMP_SIZE_PRV_))
    {
        /* push tail up if overlapped */
        if ((p_log_tail > p_log_head) && (p_log_tail < (p_log_head + len)))
        {
            p_log_tail = (p_log_head + len);
        }
        strncpy(p_log_head, p_data_to_log, len);
        p_log_head += len;
    }
    else
    {
        /* overlap */

        /* add to top of buffer */

        /* calc number of bytes to top */
        uint32_t num_chars_to_top = ((gs_log_data + LOG_DATA_DUMP_SIZE_PRV_) - p_log_head);

        /* fill buffer to the top */
        strncpy(p_log_head, p_data_to_log, num_chars_to_top);

        /* add remaining to start of buffer */
        strncpy(gs_log_data, (char *) (p_data_to_log + num_chars_to_top), (len - num_chars_to_top));

        /* update head */
        p_log_head = (char *) (gs_log_data + (len - num_chars_to_top));

        /* update tail if overlapped */
        if (p_log_tail < p_log_head)
        {
            p_log_tail = p_log_head;
        }
    }
}
/*******************************************************************************
 End of function log_data
 ******************************************************************************/

/*******************************************************************************
 * Function Name: log_write_data_to_console
 * Description  : Outputs contents of circular debug buffer to console
 * Arguments    : void
 * Return Value : void
 ******************************************************************************/
static void log_write_data_to_console (void)
{
    /* check if console reporting is already underway */
    /* sometimes assert runs in more than 1 task... */
    if (log_readback_running)
    {
        return;
    }

    /* prevent multiple access in assert */
    log_readback_running = 1u;

    if (p_log_tail < p_log_head)
    {
        /* not overlapped. can just printf */
        *(p_log_head + 1) = 0;

        /* cast to remove volatile qualifier */
        printf((char *) p_log_tail);
    }
    else
    {
        /* overlapped. print up to top of buffer */
        /* not overlapped. can just printf */
        gs_log_data[LOG_DATA_DUMP_SIZE_PRV_] = 0;

        /* cast to remove volatile qualifier */
        printf((char *) p_log_tail);

        /* not overlapped. can just printf */
        *(p_log_head + 1) = 0;

        /* cast to remove volatile qualifier */
        printf((char *) gs_log_data);
    }

    /* empty buffer */
    p_log_head = gs_log_data;
    p_log_tail = gs_log_data;

    /* reset multiple access */
    log_readback_running = 0u;
}
/*******************************************************************************
 End of function log_write_data_to_console
 ******************************************************************************/

#endif /* LOG_TASK_INFO */

/******************************************************************************
 * Function Name: main_task
 * Description  : The main task, performs any remaining initialisation and call
 *                the user level function main().
 * Arguments    : none
 * Return Value : none
 *****************************************************************************/
static void main_task (void)
{
    /* Initialise the IO library - This must be done after the scheduler has
     * been started as it uses objects provided by FreeRTOS */
#if R_USE_ANSI_STDIO_MODE_CFG
    extern void initialise_monitor_handles _PARAMS ((void));

    initialise_monitor_handles();
#endif

    /* Call application main function which shouldn't return */
    main();

    /* Guard here to protect the system if main returns */
    while (1)
    {
        /* Stops program from running off */
        ;
    }
}
/******************************************************************************
 End of function  main_task
 *****************************************************************************/

static char *get_task_status (const char *task)
{
    xTaskStatusType *p_task_status_array = NULL;
    uint32_t ux_array_size = 0;
    uint16_t i;
    uint16_t j;
    uint16_t id;

    static const char *p_status_name[] =
    { [eRunning] = "Running", [eReady] = "Ready", [eBlocked] = "Blocked", [eSuspended] = "Suspended", [eDeleted
            ] = "Deleted", [eInvalid] = "Invalid" };

    do
    {
        if (ux_array_size > 0)
        {
            R_OS_FreeMem(p_task_status_array);
        }

        ux_array_size = uxTaskGetNumberOfTasks();

        if (ux_array_size > 0)
        {
            p_task_status_array = R_OS_AllocMem((ux_array_size * sizeof(xTaskStatusType)),R_REGION_LARGE_CAPACITY_RAM);
        }

        if (NULL == p_task_status_array)
        {
            printf("Cannot get memory for threads list\r\n");
            return (0);
        }

        ux_array_size = uxTaskGetSystemState(p_task_status_array, ux_array_size, NULL);
    }

    while (0 == ux_array_size);

    for (i = 0, id = 0; i < ux_array_size; id++)
    {
        for (j = 0; j < ux_array_size; j++)
        {
            xTaskStatusType *p_xt = p_task_status_array + j;

            if ( !(strcmp(p_xt->pcTaskName, task)))
            {
                return (char *)p_status_name[p_xt->eCurrentState];
            }
        }
    }

    return "Error";
}
/*******************************************************************************
 End of function get_task_status
 ******************************************************************************/

/**
 * @brief REQUIRED by FreeRTOS
 * The following functions are defined and required by FreeRTOS
 **/

/***********************************************************************************************************************
 * Function Name: vApplicationStackOverflowHook
 * Description  : Stops any destruction when a task escapes its alloted stack.
 *                Possible solution increase stack size in the tasks creation function.
 * Arguments    : pxTask - Task Object
 *                pcTaskName - Name of the task in ASCII
 * Return Value : none
 **********************************************************************************************************************/
void vApplicationStackOverflowHook (xTaskHandle pxTask, char *pcTaskName)
{
    /* unused variable */
    (void) pcTaskName;

    /* unused variable */
    (void) pxTask;

    taskDISABLE_INTERRUPTS();

    while (1)
    {
        /* Do Nothing */
        ;
    }
}
/***********************************************************************************************************************
 End of function vApplicationStackOverflowHook
 **********************************************************************************************************************/

/******************************************************************************
 * Function Name: vApplicationIdleHook
 * Description  : The FreeRTOS idle application hook
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void vApplicationIdleHook (void)
{
    volatile size_t x_free_heap_space;

    /* This is just a trivial example of an idle hook.  It is called on each
     cycle of the idle task.  It must *NOT* attempt to block.  In this case the
     idle task just queries the amount of FreeRTOS heap that remains.  See the
     memory management section on the http://www.FreeRTOS.org web site for memory
     management options.  If there is a lot of heap memory free then the
     configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
     RAM. */
    x_free_heap_space = xPortGetFreeHeapSize();

    /* Remove compiler warning about xFreeHeapSpace being set but never used. */
    (void) x_free_heap_space;
}
/******************************************************************************
 End of function  vApplicationIdleHook
 ******************************************************************************/

/**
 * @brief End of REQUIRED by FreeRTOS section
 **/


/***********************************************************************************************************************
 * Function Name: R_COMPILER_Nop
 * Description  : nop support
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
R_COMPILER_INLINE void R_COMPILER_Nop(void)
{
	__asm volatile ("nop");
}
/***********************************************************************************************************************
 End of function R_COMPILER_Nop
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: R_OS_AssertCalled
 * Description  : Generic error Handler
 * Arguments    : file - Source code file where the error occurred.
 *                line - Source code file where the error occurred.
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_AssertCalled (volatile const char *pcFile, volatile unsigned long ulLine)

{
    volatile uint32_t ul = 0;
    char local_string_buffer[32];

    memset(local_string_buffer, 0, 32);

    /* Variable assigned to a volatile static variable to avoid being optimised away */
    strncpy((char*)s_pcFile, (char const *)pcFile, 199);

    /* Variable assigned to a volatile static variable to avoid being optimised away */
    s_ulLine = ulLine;

    /* print to console */
    printf("\r\nR_OS_Assert. File :");
    printf((char*const)s_pcFile);
    printf("\r\n Line %ld\r\n:(\r\n", s_ulLine);
    printf("Current task:");
    strncpy(local_string_buffer, R_OS_GetCurrentTaskName(), 31);
    strcat(local_string_buffer, "\0");
    printf(local_string_buffer);
    printf("\r\nStatus : ");
    printf(get_task_status(R_OS_GetCurrentTaskName()));

#ifdef LOG_TASK_INFO
    printf("\r\nLog of Memory transactions:\r\n");
    log_write_data_to_console();
#endif /* LOG_TASK_INFO */

    while (0xFFFFFF >= ul)
    {
        uint32_t ll;
        for (ll = 0; ll < 0xFFFF; ll++)
        {
            ;
        }
        ul++;
    }

    ul = 0;

    taskENTER_CRITICAL()
    ;

    /* Set ul to a non-zero value using the debugger to step out of this function */
    while (0 == ul)
    {
        ;
    }

    taskEXIT_CRITICAL()
    ;
}
/***********************************************************************************************************************
 End of function R_OS_AssertCalled
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: R_OS_InitMemManager
 * Description  : Initialise the Memory manager needs to be called before RAM usage
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_InitMemManager(void)
{
    /* Heap_5 method of memory management */
    /* Initialise the xHeapRegions array from the linker settings */

    xHeapRegions[0].pucStartAddress = (MIRROR_HEAP_START);
    xHeapRegions[0].xSizeInBytes = (size_t) MIRROR_HEAP_LENGTH;
    xHeapRegions[1].pucStartAddress = (UNCACHED_HEAP_START);
    xHeapRegions[1].xSizeInBytes = (size_t) UNCACHED_HEAP_LENGTH;
    xHeapRegions[2].pucStartAddress = (0);
    xHeapRegions[2].xSizeInBytes = (0);

    /* Pass the array into vPortDefineHeapRegions(). */
    vPortDefineHeapRegions( xHeapRegions );
}
/**********************************************************************************************************************
 End of function R_OS_InitMemManager
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OS_InitKernel
 * Description  : Initialise the kernel
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void R_OS_InitKernel (void)
{
    uint32_t default_heap = R_OS_PRV_DEFAULT_HEAP;

    /* Create the main task */
    BaseType_t t = xTaskCreate((pdTASK_CODE) main_task,

    /* ASCII human readable name for task */
    gs_startup_task_name_str,

    /* stack size, should be just big enough as to not waste resources */
#ifdef LOG_TASK_INFO
    R_OS_PRV_SMALL_STACK_SIZE,
#else
    R_OS_PRV_TINY_STACK_SIZE,
#endif /* LOG_TASK_INFO */

    /* Memory type for heap, useful if your system support fast RAM to boost performance */
    &default_heap,

    /* Initial task priority */
    R_OS_TASK_MAIN_TASK_PRI,

    /* Handle to created task not required */
    NULL);

    /* Confirm that the task was successfully created */
    configASSERT((pdPASS) == t);

    R_DEVLINK_Init();

    R_OS_StartKernel();

    while (1)
    {
        /* This point should never be reached */
        ;
    }
}
/***********************************************************************************************************************
 End of function R_OS_InitKernel
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_StartKernel
 * Description  : Start the kernel
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_StartKernel (void)
{
    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();
}
/***********************************************************************************************************************
 End of function R_OS_StartKernel
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_StopKernel
 * Description  : Stop the kernel
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_StopKernel (void)
{
    /* Do Nothing */
    vTaskEndScheduler();
}
/***********************************************************************************************************************
 End of function R_OS_StopKernel
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_CreateTask
 * Description  : Create a task
 * Arguments    : *name - task name string
 *                 task_code - task main function
 *                 params - task associated parameters (task defined)
 *                 stack_size - initial stack size (maximum)
 *                 priority - initial priority
 * Return Value : os_task_t - pointer to newly created task, NULL if failed
 **********************************************************************************************************************/
os_task_t *R_OS_CreateTask (const char_t *name, os_task_code_t task_code, void *params, size_t stack_size,
        int_t priority)
{
    portBASE_TYPE status;
    uint32_t actual_stack = 0;

    switch (stack_size)
    {
        case R_OS_ABSTRACTION_PRV_TINY_STACK_SIZE:
        {
            /* Stack size is defined in FreeRTOS as unsigned short, convert to 32 bit */
            actual_stack = (uint32_t) R_OS_PRV_TINY_STACK_SIZE;
            break;
        }
        case R_OS_ABSTRACTION_PRV_SMALL_STACK_SIZE:
        {
            /* Stack size is defined in FreeRTOS as unsigned short, convert to 32 bit */
            actual_stack = (uint32_t) R_OS_PRV_SMALL_STACK_SIZE;
            break;
        }
        case R_OS_ABSTRACTION_PRV_DEFAULT_STACK_SIZE:
        {
            /* Stack size is defined in FreeRTOS as unsigned short, convert to 32 bit */
            actual_stack = (uint32_t) R_OS_PRV_DEFAULT_STACK_SIZE;
            break;
        }
        case R_OS_ABSTRACTION_PRV_LARGE_STACK_SIZE:
        {
            /* Stack size is defined in FreeRTOS as unsigned short, convert to 32 bit */
            actual_stack = (uint32_t) R_OS_PRV_LARGE_STACK_SIZE;
            break;
        }
        case R_OS_ABSTRACTION_PRV_HUGE_STACK_SIZE:
        {
            /* Stack size is defined in FreeRTOS as unsigned short, convert to 32 bit */
            actual_stack = (uint32_t) R_OS_PRV_HUGE_STACK_SIZE;
            break;
        }
        default:
        {
            actual_stack = stack_size;
            break;
        }
    }

    /* Initialised variable (task) is checked against NULL to determine if the new task can be initialised
     * see prvInitialiseNewTask in file freertos\tasks.c */
    xTaskHandle task = (TaskHandle_t *) (NULL);
    os_task_t *p_new_task;

    /* Create a new task */
    status = xTaskCreate((pdTASK_CODE) task_code, name, (uint16_t) (actual_stack / sizeof(StackType_t)), params, (UBaseType_t) priority, &task);

    /* Check whether the task was successfully created */
    if (pdPASS == status)
    {
        p_new_task = task;
        return task;
    }
    else
    {
        /* cast to NULL required to fulfil FreeRTOS requirements */
        p_new_task = (os_task_t *) (NULL);
    }

    return p_new_task;
}
/***********************************************************************************************************************
 End of function R_OS_CreateTask
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_DeleteTask
 * Description  : Delete task
 * Arguments    : task - pointer to task, NULL deletes the current task
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_DeleteTask (os_task_t *task)
{
    /* Delete the specified task */
    vTaskDelete((xTaskHandle) task);
}
/***********************************************************************************************************************
 End of function R_OS_DeleteTask
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_SuspendTask
 * Description  : Suspend the specified task
 * Arguments    : task - pointer to task, NULL deletes the current task
 * Return Value : none
 **********************************************************************************************************************/
bool_t R_OS_SuspendTask (os_task_t *task)
{
    bool_t ret;

    switch (eTaskGetState(task))
    {
        case eReady:
        case eRunning:
        case eBlocked:
        {
            vTaskSuspend(task);
            ret = true;
        }
        break;

        default:
        case eDeleted:
        case eSuspended:
        {
            ret = false;
        }
    }
    return (ret);
}
/***********************************************************************************************************************
 End of function R_OS_SuspendTask
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_ResumeTask
 * Description  : Resume the task
 * Arguments    : task - pointer to task, NULL deletes the current task
 * Return Value : none
 **********************************************************************************************************************/
bool_t R_OS_ResumeTask (os_task_t *task)
{
    bool_t ret;

    switch (eTaskGetState(task))
    {
        default:
        case eReady:
        case eRunning:
        case eBlocked:
        case eDeleted:
        {
            ret = false;
        }
        break;

        case eSuspended:
        {
            vTaskResume(task);
            ret = true;
        }
    }
    return (ret);
}
/***********************************************************************************************************************
 End of function R_OS_ResumeTask
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_TaskSleep
 * Description  : Suspend the task for the specified period of time
 * Arguments    : sleep_ms - sleep period in milliseconds
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_TaskSleep (uint32_t sleep_ms)
{
    /* Delay the task for the specified duration */
    vTaskDelay(OS_MS_TO_SYSTICKS(sleep_ms));
}
/***********************************************************************************************************************
 End of function R_OS_TaskSleep
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_Yield
 * Description  : Force a context switch
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_Yield (void)
{
    /* Force a context switch */
    taskYIELD()
    ;
}
/***********************************************************************************************************************
 End of function R_OS_Yield
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_SuspendAllTasks
 * Description  : Suspend all tasks, only attempted if the operating system is running
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_SuspendAllTasks (void)
{
    /* Make sure the operating system is running */
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        /* Suspend all tasks */
        vTaskSuspendAll();
    }
}
/***********************************************************************************************************************
 End of function R_OS_SuspendAllTasks
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_ResumeAllTasks
 * Description  : Resume all tasks, only attempted if the operating system is running
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_ResumeAllTasks (void)
{
    /* Make sure the operating system is running */
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        /* Resume all tasks */
        xTaskResumeAll();
    }
}
/***********************************************************************************************************************
 End of function R_OS_ResumeAllTasks
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_TaskUsesFloatingPoint
 * Description  : TODO .
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_TaskUsesFloatingPoint (void)
{
    portTASK_USES_FLOATING_POINT();
}
/***********************************************************************************************************************
 End of function R_OS_TaskUsesFloatingPoint
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_GetNumberOfTasks
 * Description  : Obtain total number of active tasks defined in the system, only attempted if the operating system is
 *                running
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
uint32_t R_OS_GetNumberOfTasks (void)
{
    uint32_t num_tasks = 0;

    /* Make sure the operating system is running */
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        /* use FreeRTOS call to obtain information */
        num_tasks = uxTaskGetNumberOfTasks();
    }
    return (num_tasks);
}
/***********************************************************************************************************************
 End of function R_OS_GetNumberOfTasks
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_SysLock
 * Description  : Function to lock a critical section.
 * Arguments    : An pointer to an existing OS ABSTRACTION object to populate if available, or NULL.
 * Return Value : Current interrupt level or -1 if not used.
 **********************************************************************************************************************/
int_t R_OS_SysLock (void *pp_vLockObj)
{
    int_t return_value = ( -1);
    uint32_t *object = (uint32_t *) pp_vLockObj;

    /* If a locking object pointer has not been provided then attempt a OS specific system wide lock
     * In an os-less implementation this may be implemented by disabling all interrupt sources.
     * In an os-active implementation system locking objects should be used which are implementation specific */
    if (NULL == pp_vLockObj)
    {
        /* Disable the IRQ and return -1 to show that the interrupts existing interrupt value is invalid. */
        if (0 == ulPortInterruptNesting)
        {
            taskENTER_CRITICAL()
            ;
        }
        else
        {
            uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
        }

        return_value = ( -1);
    }
    else
    {
        /* Create a Mutex */
        void *p_mutex = R_OS_CreateMutex();

        /* Check that the mutex is valid.*/
        if (NULL != (p_mutex))
        {
            R_OS_AcquireMutex(p_mutex);

            /* Return the mutex to the user */
            pp_vLockObj = *object;
            return_value = ( -1);
        }
        else
        {
            ;
        }

        /* Failed to create the mutex or to take it */
        return_value = ( -1);
    }
    return (return_value);
}
/***********************************************************************************************************************
 End of function R_OS_SysLock
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_SysUnlock
 * Description  : This primitive function is specific for os_less applications
 * Arguments    : ppvLockObj - pointer to the object to lock
 *                imask - Stored value of the interrupt mask to restore if used.
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_SysUnlock (void *pp_vLockObj, int_t imask)
{
	(void) imask;

    uint32_t *object = (uint32_t *) pp_vLockObj;

    /* To unlock using OS active we need a valid lock object and expect that the interrupt level is < 0 */
    if (NULL == pp_vLockObj)
    {
        if (0 == ulPortInterruptNesting)
        {
            taskEXIT_CRITICAL()
            ;
        }
        else
        {
            taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
        }
    }
    else
    {
        R_OS_ReleaseMutex( *object);
        R_OS_DeleteMutex( *object);
    }
}
/***********************************************************************************************************************
 End of function R_OS_SysUnlock
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_SysWaitAccess
 * Description  : This primitive function is specific for os_less applications
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_SysWaitAccess (void)
{
    void *pvTaskHandle = xTaskGetCurrentTaskHandle();
    if (s_pvTaskHandle == pvTaskHandle)
    {
        /* allow re-entrant access */
    }
    else
    {
        /* Wait for access to the list */
        if (NULL == s_pvStreamSemaphore)
        {
            s_pvStreamSemaphore = xSemaphoreCreateMutex();
        }

        /* Wait mutex */
        xSemaphoreTake(s_pvStreamSemaphore, portMAX_DELAY);

        /* Assign the task ID for future reentrant access */
        s_pvTaskHandle = pvTaskHandle;
    }
}
/***********************************************************************************************************************
 End of function R_OS_SysWaitAccess
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_SysReleaseAccess
 * Description  : This primitive function is specific for os_less applications
 * Arguments    : none
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_SysReleaseAccess (void)
{
    s_pvTaskHandle = NULL;

    /* Release mutex */
    xSemaphoreGive(s_pvStreamSemaphore);
}
/***********************************************************************************************************************
 End of function R_OS_SysReleaseAccess
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_AllocMem
 * Description  : Allocate a block of memory
 * Arguments    : size - request size
 * Return Value : ptr to memory block
 **********************************************************************************************************************/
void *R_OS_AllocMem (size_t size, uint32_t region)
{
    volatile void *p;

    /* The call sequence pvPortsetDesiredBlockForMalloc() to pvPortMalloc() must not be interrupted. */
	/* If interrupted (via task switching) the chosen memory region might be changed by the swapped-in task */
    vTaskSuspendAll();

    switch (region)
    {
		case R_REGION_LARGE_CAPACITY_RAM:
		{
	        /* Initial Region R_REGION_LARGE_CAPACITY_RAM */
	        pvPortsetDesiredBlockForMalloc((size_t)xHeapRegions[0].pucStartAddress);
	        break;
		}
		case R_REGION_UNCACHED_RAM:
		{
	        /* Initial Region R_REGION_UNCACHED_RAM 0x6020000 */
	        pvPortsetDesiredBlockForMalloc((size_t)xHeapRegions[1].pucStartAddress);
	        break;
		}
		default:
		{
	        /* if region is incorrectly specified assign the requested memory to the first block */
	        pvPortsetDesiredBlockForMalloc((size_t)xHeapRegions[0].pucStartAddress);
		}
    }

    /* Allocate a memory block */
    p = pvPortMalloc(size);

    xTaskResumeAll();


#ifdef LOG_TASK_INFO
    {
        char temp_buffer[33];
        memset(temp_buffer, 0, 33);

        log_data("Task:", 0);
        log_data(R_OS_GetCurrentTaskName(), 0);

        sprintf(temp_buffer, ":Malloc:%d:0x%08x\r\n", size, (uintptr_t) p);
        log_data(temp_buffer, 0);
    }
#endif /* LOG_TASK_INFO */

/* Debug message */
    TRACE(("Allocating %d bytes at 0x%08x \r\n", size, (uintptr_t) p));

    /* Return a pointer to the newly allocated memory block */
    return ((void *)p);
}
/***********************************************************************************************************************
 End of function R_OS_AllocMem
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_FreeMem
 * Description  : Free memory block
 * Arguments    : p - ptr to memory block
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_FreeMem (void *p)
{
    //Make sure the pointer is valid
    if (p != NULL)
    {
        //Debug message
        TRACE(("Freeing memory at 0x%08x \r\n", p));

        /* Free memory block */
        vPortFree(p);

#ifdef LOG_TASK_INFO
        {
            char temp_buffer[33];
            memset(temp_buffer, 0, 33);

            log_data("Task:", 0);
            log_data(R_OS_GetCurrentTaskName(), 0);

            sprintf(temp_buffer, ":Free:0x%08x\r\n", (uintptr_t) p);
            log_data(temp_buffer, 0);
        }
#endif /* LOG_TASK_INFO */
    }
}
/***********************************************************************************************************************
 End of function R_OS_FreeMem
 **********************************************************************************************************************/

/* Semaphore management */
/***********************************************************************************************************************
 * Function Name: R_OS_CreateSemaphore
 * Description  : Create a semaphore
 * Arguments    : semaphore_ptr - Pointer to a associated semaphore
 *              : count         - The maximum count for the semaphore object. This value must be greater than zero
 * Return Value : The function returns TRUE if the semaphore object was successfully created
 *                Otherwise, FALSE is returned
 **********************************************************************************************************************/
bool_t R_OS_CreateSemaphore (semaphore_t semaphore_ptr, uint32_t count)
{
    bool_t ret = false;

    /* semaphore is uint32_t */
    *semaphore_ptr = (uint32_t) xSemaphoreCreateCounting(configQUEUE_REGISTRY_SIZE, count);

    if (0 != ( *semaphore_ptr))
    {
        ret = true;
    }

    return (ret);
}
/***********************************************************************************************************************
 End of function R_OS_CreateSemaphore
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_DeleteSemaphore
 * Description  : Delete a semaphore, freeing any associated resources
 * Arguments    : semaphore_ptr - Pointer to a associated semaphore
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_DeleteSemaphore (semaphore_t semaphore_ptr)
{
    if (0 != *semaphore_ptr)
    {
        volatile uint32_t temp_semaphore_handle = *semaphore_ptr;

        *semaphore_ptr = 0;

        /* cast semaphore_t used by OS abstraction to SemaphoreHandle_t used by FreeRTOS */
        vSemaphoreDelete((SemaphoreHandle_t )temp_semaphore_handle);
    }
}
/***********************************************************************************************************************
 End of function R_OS_DeleteSemaphore
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_WaitForSemaphore
 * Description  : Blocks operation until one of the following occurs:
 *              :        A timeout occurs
 *              :     The associated semaphore has been set
 * Arguments    : semaphore_ptr - Pointer to a associated semaphore
 *              : timeout       - Maximum time to wait for associated event to occur
 * Return Value : The function returns TRUE if the semaphore object was successfully set. Otherwise, FALSE is returned
 **********************************************************************************************************************/
bool_t R_OS_WaitForSemaphore (semaphore_t semaphore_ptr, systime_t timeout)
{
    bool_t ret = false;
    TickType_t ticks_to_wait;

    /* Check that there is a non zero handle to the semaphore. */
    if (0 != *semaphore_ptr)
    {
        if (R_OS_PRV_INFINITE_DELAY == timeout)
        {
            ticks_to_wait = portMAX_DELAY;
        }
        else
        {
            ticks_to_wait = OS_MS_TO_SYSTICKS(timeout);
        }

        /* Check if we are in an ISR */
        if (ulPortInterruptNesting)
        {
            BaseType_t dummy_variable;

            /* cast semaphore_t used by OS abstraction to SemaphoreHandle_t used by FreeRTOS */
            if (xSemaphoreTakeFromISR((SemaphoreHandle_t) (*semaphore_ptr), &dummy_variable) == pdTRUE)
            {
                ret = true;
            }
        }
        else
        {
            /* cast semaphore_t used by OS abstraction to SemaphoreHandle_t used by FreeRTOS */
            if (xSemaphoreTake((SemaphoreHandle_t) (*semaphore_ptr), ticks_to_wait) == pdTRUE)
            {
                ret = true;
            }
        }
    }

    return (ret);
}
/***********************************************************************************************************************
 End of function R_OS_WaitForSemaphore
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_ReleaseSemaphore
 * Description  : Release a semaphore, freeing freeing it to be used by another task
 * Arguments    : semaphore_ptr - Pointer to a associated semaphore
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_ReleaseSemaphore (semaphore_t semaphore_ptr)
{
    /* Check that there is a non zero handle to the semaphore. */
    if (0 != *semaphore_ptr)
    {
        /* Check if we are in an ISR */
        if (ulPortInterruptNesting)
        {
            /* cast semaphore_t used by OS abstraction to SemaphoreHandle_t used by FreeRTOS */
            xSemaphoreGiveFromISR((SemaphoreHandle_t ) ( *semaphore_ptr), NULL);
        }
        else
        {
            /* cast semaphore_t used by OS abstraction to SemaphoreHandle_t used by FreeRTOS */
            xSemaphoreGive((SemaphoreHandle_t ) ( *semaphore_ptr));
        }
    }
}
/***********************************************************************************************************************
 End of function R_OS_ReleaseSemaphore
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_CreateMutex
 * Description  : Create a mutex
 * Arguments    : None
 * Return Value : mutex - ptr to object
 **********************************************************************************************************************/
void *R_OS_CreateMutex (void)
{
    /* cast SemaphoreHandle_t used by FreeRTOS to void * used by OS abstraction */
    return ((void *) xSemaphoreCreateMutex());
}
/***********************************************************************************************************************
 End of function R_OS_CreateMutex
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_DeleteMutex
 * Description  : Delete mutex
 * Arguments    : mutex - ptr to object
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_DeleteMutex (void *p_mutex)
{
    /* cast void * used by OS abstraction to SemaphoreHandle_t used by FreeRTOS */
    vSemaphoreDelete((SemaphoreHandle_t ) p_mutex);
}
/***********************************************************************************************************************
 End of function R_OS_DeleteMutex
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_AcquireMutex
 * Description  : Acquire the mutex, waiting indefinitely if need be
 * Arguments    : mutex - pointer to object
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_AcquireMutex (void *p_mutex)
{
    /* Obtain ownership of the mutex object */
    /* cast void * used by OS abstraction to SemaphoreHandle_t used by FreeRTOS */
    xSemaphoreTake((SemaphoreHandle_t ) p_mutex, portMAX_DELAY);
}
/***********************************************************************************************************************
 End of function R_OS_AcquireMutex
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_ReleaseMutex
 * Description  : This primitive function is specific for os_less applications
 * Arguments    : mutex - ptr to object
 * Return Value : none
 **********************************************************************************************************************/
void R_OS_ReleaseMutex (void *p_mutex)
{
    /* Release ownership of the mutex object */
    /* cast void * used by OS abstraction to SemaphoreHandle_t used by FreeRTOS */
    xSemaphoreGive((SemaphoreHandle_t ) p_mutex);
}
/***********************************************************************************************************************
 End of function R_OS_ReleaseMutex
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_EventWaitMutex
 * Description  : Wait for a mutex. The mutex is created if it's NULL
 * Arguments    : *pEvent - object to lock
 *                dwTimeOut - wait time, maximum use R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE
 * Return Value : true if the mutex was acquired, false if not
 **********************************************************************************************************************/
bool_t R_OS_EventWaitMutex (pevent_t pEvent, uint32_t dwTimeOut)
{
    xSemaphoreHandle x_semaphore = NULL;

    /* Check to see if it is pointing to NULL */
    if (NULL == ( *pEvent))
    {
        /* Create the semaphore */
        x_semaphore = xSemaphoreCreateMutex();

        /* Return the mutex to the user */
        *((xSemaphoreHandle *) pEvent) = x_semaphore;
    }
    else
    {
        /* Get the mutex handle from the user */
        x_semaphore = *((xSemaphoreHandle *) pEvent);
    }

    /* Check that the semaphore was created */
    if (x_semaphore)
    {
        if (R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE == dwTimeOut)
        {
            dwTimeOut = portMAX_DELAY;
        }

        /* Get mutex */
        if (xSemaphoreTake(x_semaphore, dwTimeOut))
        {
            return (true);
        }
    }

    return (false);
}
/***********************************************************************************************************************
 End of function R_OS_EventWaitMutex
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_EventReleaseMutex
 * Description  : Release the mutex acquired by R_OS_EventWaitMutex
 * Arguments    : *pEvent - object to lock
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_EventReleaseMutex (pevent_t pEvent)
{
    xSemaphoreHandle x_semaphore = *((xSemaphoreHandle *) pEvent);

    /* Release mutex */
    xSemaphoreGive(x_semaphore);
}
/***********************************************************************************************************************
 End of function R_OS_EventReleaseMutex
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_EnterCritical
 * Description  : Enter critical section
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_EnterCritical (void)
{
    taskENTER_CRITICAL()
    ;
}
/***********************************************************************************************************************
 End of function R_OS_EnterCritical
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_ExitCritical
 * Description  : Exit critical section
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_ExitCritical (void)
{
    taskEXIT_CRITICAL()
    ;
}
/***********************************************************************************************************************
 End of function R_OS_ExitCritical
 **********************************************************************************************************************/

/** OS Abstraction CreateMessageQueue Function
 *  @brief     Create an Message Queue.
 *  @param[in] queue_sz Maximum number of elements in queue.
 *  @param[out] pp_queue_handle pointer to queue handle pointer.
 *  @return    The function returns TRUE if the message queue was successfully created. Otherwise, FALSE is returned
 */
bool_t R_OS_CreateMessageQueue (uint32_t queue_sz, os_msg_queue_handle_t *p_queue_handle)
{
    bool_t ret_value = false;
    os_msg_queue_handle_t *p_queue_temp;

    /* Create queue with size of os_msg_t struct which points to message */
    p_queue_temp = (os_msg_queue_handle_t *) xQueueCreate(queue_sz, sizeof(os_msg_t));

    /* test for valid response before confirming correct create */
    if (NULL != p_queue_temp)
    {
        *p_queue_handle = p_queue_temp;
        ret_value = true;
    }

    return (ret_value);
}
/*******************************************************************************
 End of function R_OS_CreateMessageQueue
 ******************************************************************************/

/** OS Abstraction PutMessageQueue Function
 *  @brief     Put a message onto a queue.
 *  @param[in] p_queue_handle pointer to queue handle.
 *  @param[in] p_message pointer to message.
 *  @return    The function returns TRUE if the event object was successfully added to the queue. Otherwise, FALSE is returned
 */
bool_t R_OS_PutMessageQueue (os_msg_queue_handle_t p_queue_handle, os_msg_t p_message)
{
    int32_t freertos_ret_value;
    bool_t ret_value = false;

    /* return failed if queue handle pointer is NULL */
    if (NULL != p_queue_handle)
    {
        /* Check if we are in an ISR */
        if (ulPortInterruptNesting)
        {
            /* casts to xQueueHandle and void **/
            freertos_ret_value = xQueueSendFromISR((xQueueHandle)p_queue_handle, (void * ) &p_message, 0UL);
        }
        else
        {
            /* casts to xQueueHandle and void * */
            freertos_ret_value = xQueueSend((xQueueHandle)p_queue_handle, (void * ) &p_message, 0UL);
        }

        /* check for failure of operation */
        if (pdPASS != freertos_ret_value)
        {
            ret_value = false;
        }
        else
        {
            ret_value = true;
        }
    }

    /* return status */
    return (ret_value);
}
/*******************************************************************************
 End of function R_OS_PutMessageQueue
 ******************************************************************************/

/** OS Abstraction GetMessageQueue Function
 *  @brief     Get a message from a queue.
 *  @param[in] p_queue_handle pointer to queue handle.
 *  @param[out] ppmsg pointer to message pointer. Pointer will point to NULL if no message and times out.
 *  @param[in] timeout in system ticks.
 *  @param[in] blocking true = block thread/task until message received.False = not blocking
 *  @return    The function returns TRUE if the event object was successfully retrieved from the queue. Otherwise, FALSE is returned
 */
bool_t R_OS_GetMessageQueue (os_msg_queue_handle_t queue, os_msg_t *ppmsg, uint32_t timeout, bool_t blocking)
{
    /* set default timeout to be OS max */
    uint32_t ticks_to_wait = portMAX_DELAY;
    bool_t ret_value = false;

    /* ensure that pointers are not NULL before proceeding */
    if ((NULL != queue) && (NULL != ppmsg))
    {
        /* handle max timeout */
        if (R_OS_PRV_INFINITE_DELAY != timeout)
        {
            ticks_to_wait = OS_MS_TO_SYSTICKS(timeout);
        }

        if (false == blocking)
        {
            /* Receive check is not task-blocking */
            /* Receive with time-out. Cast to xQueueHandle */
            if (pdPASS == xQueueReceive((xQueueHandle) queue, (void *) (ppmsg), ticks_to_wait / portTICK_RATE_MS))
            {
                ret_value = true;
            }
            else
            {
                /* timed out - report failure */
                *ppmsg = NULL;
            }
        }
        else
        {
            /* Receive check is task-blocking */
            /* Receive with time-out. Cast to xQueueHandle */
            while (pdPASS != xQueueReceive((xQueueHandle) queue, (void *) ( *ppmsg), ticks_to_wait / portTICK_RATE_MS))
            {
                taskYIELD()
                ;
            }

            ret_value = true;
        }
    }

    /* return status */
    return (ret_value);
}
/*******************************************************************************
 End of function R_OS_GetMessageQueue
 ******************************************************************************/

/** OS Abstraction ClearMessageQueue Function
 *  @brief     Clear a message queue.
 *  @param[in] p_queue_handle pointer to queue handle.
 *  @return    The function returns TRUE if the event object was successfully cleared. Otherwise, FALSE is returned
 */
bool_t R_OS_ClearMessageQueue (os_msg_queue_handle_t *p_queue_handle)
{
    /* ensure that queue handle is valid */
    if (NULL != p_queue_handle)
    {
        /* cast to xQueueHandle */
        xQueueReset((xQueueHandle) p_queue_handle);

        /* freeRTOS always returns true */
        return (true);
    }

    /* NULL queue pointer as argument */
    return (false);
}
/*******************************************************************************
 End of function R_OS_ClearMessageQueue
 ******************************************************************************/

/** OS Abstraction DeleteMessageQueue Function
 *  @brief     Delete a message queue. The message queue pointer argument will be set to NULL.
 *  @param[in] pp_queue_handle pointer to queue handle pointer.
 *  @return    The function returns TRUE if the event object was successfully deleted. Otherwise, FALSE is returned
 */
bool_t R_OS_DeleteMessageQueue (os_msg_queue_handle_t *pp_queue_handle)
{
    /* ensure that queue handle is valid */
    if (NULL != pp_queue_handle)
    {
        /* delete queue structure */
        vQueueDelete((xQueueHandle) *pp_queue_handle);

        /* set queue pointer to NULL */
        *pp_queue_handle = NULL;

        /* freeRTOS always returns true */
        return (true);
    }

    /* queue pointer is NULL to start with so nothing deleted */
    return (false);
}
/*******************************************************************************
 End of function R_OS_DeleteMessageQueue
 ******************************************************************************/

static size_t gstEventCount = 0UL;
static size_t gstEventMax = 0UL;

/* Event Management */
/***********************************************************************************************************************
 * Function Name: R_OS_CreateEvent
 * Description  : Create an event
 * Arguments    : event_ptr - pointer to a associated event
 * Return Value : The function returns TRUE if the event object was successfully created. Otherwise, FALSE is returned
 **********************************************************************************************************************/
bool_t R_OS_CreateEvent (pevent_t event_ptr)
{
    /* Allocate a queue */
    QueueHandle_t event = xQueueCreate(1U, sizeof(e_event_state_t));

    if (NULL != event)
    {
        /* Reset the event */
        xQueueReset(event);

        /* return the event */
        *event_ptr = event;

        gstEventCount++;
        if (gstEventCount > gstEventMax)
        {
            gstEventMax = gstEventCount;
        }
    }

    return (NULL != event);
}
/***********************************************************************************************************************
 End of function R_OS_CreateEvent
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_DeleteEvent
 * Description  : Delete an event, freeing any associated resources
 * Arguments    : event_ptr - pointer to a associated event
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_DeleteEvent (pevent_t event_ptr)
{
    /* Make sure the handle is valid */
    if (NULL != event_ptr)
    {
        /* Properly dispose the event object */
        vQueueDelete( *event_ptr);

        if (gstEventCount != 0)
        {
            gstEventCount--;
        }
    }

    /* The expectation is that the handle to the event should now be NULL
     * so that any future attempt to get the event will not be valid */
    *event_ptr = NULL;
}
/***********************************************************************************************************************
 End of function R_OS_DeleteEvent
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_SetEvent
 * Description  : Sets the state on the associated event
 * Arguments    : event_ptr - pointer to a associated event
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_SetEvent (pevent_t event_ptr)
{
    e_event_state_t event_state = EV_SET;

    /* Check if we are in an ISR */
    if (ulPortInterruptNesting)
    {
        xQueueOverwriteFromISR( *event_ptr, &event_state, NULL);
    }
    else
    {
        xQueueOverwrite( *event_ptr, &event_state);
    }
}
/***********************************************************************************************************************
 End of function R_OS_SetEvent
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_ResetEvent
 * Description  : Clears the state on the associated event
 * Arguments    : event_ptr - pointer to a associated event
 * Return Value : None
 **********************************************************************************************************************/
void R_OS_ResetEvent (pevent_t event_ptr)
{
    /* Force the specified event to the non-signalled state */
    e_event_state_t event_state = EV_RESET;

    /* Check if we are in an ISR */
    if (ulPortInterruptNesting)
    {
        xQueueOverwriteFromISR( *event_ptr, &event_state, NULL);
    }
    else
    {
        xQueueOverwrite( *event_ptr, &event_state);
    }
}
/***********************************************************************************************************************
 End of function R_OS_ResetEvent
 **********************************************************************************************************************/

/*****************************************************************************
 Function Name: R_OS_EventState
 Description:   Function to return the current state of an event
 Arguments:     IN event_ptr - pointer to the event
 Return value:  The state of the event
 *****************************************************************************/
e_event_state_t R_OS_EventState (pevent_t event_ptr)
{
    e_event_state_t event_state = EV_RESET;

    if (ulPortInterruptNesting)
    {
        xQueuePeekFromISR( *event_ptr, &event_state);
    }
    else
    {
        xQueuePeek( *event_ptr, &event_state, 0UL);
    }

    return event_state;
}
/*****************************************************************************
 End of function R_OS_EventState
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_WaitForEvent
 * Description  : Blocks operation until one of the following occurs
 *                A timeout occurs
 *                The associated event has been set
 * Arguments    : event_ptr - pointer to a associated event
 *                timeout   - maximum time to wait for associated event to occur
 * Return Value : The function returns TRUE if the event object was successfully set. Otherwise, FALSE is returned
 **********************************************************************************************************************/
bool_t R_OS_WaitForEvent (pevent_t event_ptr, systime_t timeout)
{
    volatile bool_t ret_value = false;
    volatile bool_t process = true;
    e_event_state_t event_state = EV_INVALID;
    TickType_t ticks_to_wait;

    if (R_OS_PRV_INFINITE_DELAY == timeout)
    {
        ticks_to_wait = portMAX_DELAY;
    }
    else
    {
        ticks_to_wait = OS_MS_TO_SYSTICKS(timeout);
    }

    while (process == true)
    {
        if (xQueueReceive( *event_ptr, &event_state, ticks_to_wait) == pdTRUE)
        {
            if (EV_SET == event_state)
            {
                ret_value = true;
                process = false;
            }
        }

        if (R_OS_PRV_INFINITE_DELAY != timeout)
        {
            process = false;
        }
    }

    return (ret_value);
}
/***********************************************************************************************************************
 End of function R_OS_WaitForEvent
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_SetEventFromIsr
 * Description  : Sets the state on the associated event
 *                Warning : Function shall only be caller from within an ISR routine
 * Arguments    : event_ptr  - Pointer to a associated event
 * Return Value : The function returns TRUE if the event object was successfully set. Otherwise, FALSE is returned
 **********************************************************************************************************************/
bool_t R_OS_SetEventFromIsr (pevent_t event_ptr)
{
    portBASE_TYPE ret_value;
    e_event_state_t event_state = EV_SET;

    ret_value = xQueueSendFromISR( *event_ptr, &event_state, NULL);

    return (pdTRUE == ret_value);
}
/***********************************************************************************************************************
 End of function R_OS_SetEventFromIsr
 **********************************************************************************************************************/

/*****************************************************************************
 Function Name: R_OS_TaskGetPriority
 Description:   Function to get the task priority
 Arguments:     IN uiTaskID - The ID of the task to get the priority of
 Return value:  The priority of the task or -1U if not found
 *****************************************************************************/
int32_t R_OS_TaskGetPriority (uint32_t uiTaskID)
{
    if ((xTaskHandle) uiTaskID != (xTaskHandle) NULL)
    {
        return (int32_t) uxTaskPriorityGet((xTaskHandle) uiTaskID);
    }
    else
    {
        return -1;
    }
}
/*****************************************************************************
 End of function  R_OS_TaskGetPriority
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_OS_TaskSetPriority
 Description:   Function to set the priority of a task
 Arguments:     IN  uiTaskID - The ID of the task to get the priority for
 IN  uiPriority - The priority of the task
 Return value:  true if the priority was set
 *****************************************************************************/
bool_t R_OS_TaskSetPriority (uint32_t uiTaskID, uint32_t uiPriority)
{
    if ((xTaskHandle) uiTaskID != (xTaskHandle) NULL)
    {
        vTaskPrioritySet((xTaskHandle) uiTaskID, uiPriority);
        return true;
    }
    else
    {
        return false;
    }
}
/*****************************************************************************
 End of function  R_OS_TaskSetPriority
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_GetCurrentTask
 * Description  : Get the ID of a task
 * Arguments    : task - pointer to task
 * Return Value : Task id
 **********************************************************************************************************************/
os_task_t *R_OS_GetCurrentTask (void)
{
    return xTaskGetCurrentTaskHandle();
}
/***********************************************************************************************************************
 End of function R_OS_GetCurrentTask
 **********************************************************************************************************************/

/*******************************************************************************
 * Function Name: R_OS_GetCurrentTaskName
 * Description  : return a pointer to the current task name
 * Arguments    : none
 * Return Value : char * pointer to task name
 ******************************************************************************/
char *R_OS_GetCurrentTaskName (void)
{
    TaskHandle_t xHandle = xTaskGetCurrentTaskHandle();
    TaskStatus_t xTaskDetails;

// Use the handle to obtain further information about the task.
    vTaskGetInfo(xHandle, &xTaskDetails, pdTRUE, eInvalid);

    return ((char *)xTaskDetails.pcTaskName);
}
/*******************************************************************************
 End of function R_OS_GetCurrentTaskName
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_OS_GetTaskState
 * Description  : Get the state of a task
 * Arguments    : task - pointer to task
 * Return Value : String describing the status of the task
 **********************************************************************************************************************/
char *R_OS_GetTaskState (const char *task)
{
    return get_task_status(task);
}
/***********************************************************************************************************************
 End of function R_OS_GetTaskState
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_OS_GetVersion
 * Description  :
 * Arguments    : pinfo - detailed driver information structure
 * Return Value : 0 can not fail
 **********************************************************************************************************************/
/*
 int32_t R_OS_GetVersion(st_drv_info_t *pinfo)
 {
 pinfo->version.sub.major = gs_os_version.version.sub.major;
 pinfo->version.sub.minor = gs_os_version.version.sub.minor;
 pinfo->build = gs_os_version.build;
 pinfo->p_szdriver_name = gs_os_version.p_szdriver_name;
 return (0);

 }
 */
#endif (R_RTOS_ABSTRACTION == R_RTOS_ABSTRACTION_FREERTOS)
/**********************************************************************************************************************
 End of function R_OS_GetVersion
 **********************************************************************************************************************/
