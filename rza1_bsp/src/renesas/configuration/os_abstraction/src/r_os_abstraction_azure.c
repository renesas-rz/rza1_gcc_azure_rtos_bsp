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

#include <stdio.h>
#include <fcntl.h>

#include "r_typedefs.h"
#include "r_devlink_wrapper.h"

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


extern uint8_t _ld_mirrored_heap_start;
extern uint8_t _ld_mirrored_heap_end;

extern uint8_t _ld_uncached_heap_start;
extern uint8_t _ld_uncached_heap_end;
extern void pvPortsetDesiredBlockForMalloc( size_t xWantedBlock );

void vApplicationIdleHook (void);

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

static void *s_pvTaskHandle = NULL;

extern uint32_t ulPortInterruptNesting;

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

/**
 * @brief REQUIRED by FreeRTOS
 * The following functions are defined and required by FreeRTOS
 **/


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

    R_DEVLINK_Init();

    main();

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

    return NULL;
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
	return NULL;
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
    bool_t ret_value = false;

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

    bool_t ret_value = false;

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
    return (false);
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

    return (true);
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
    return -1;
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
    return false;
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
    return NULL;
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
    return NULL;
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
    return NULL;
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
/**********************************************************************************************************************
 End of function R_OS_GetVersion
 **********************************************************************************************************************/
