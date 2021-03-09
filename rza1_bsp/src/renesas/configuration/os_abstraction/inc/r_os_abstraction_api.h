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
 * File Name    : r_os_abstraction_api.h
 * Description  : see Detailed description (below)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup RENESAS_OS_ABSTRACTION Operating System Abstraction module
 * @brief Provides OS abstraction, use these primitives in the code base NOT direct calls to OS primitives.
 *
 * To make efficient code re-use identical calls shall be used in both OS and OS Less applications.
 * This file aims to abstract the Operating system (OS) awareness in systems allowing for enhanced portability.
 *
 * @anchor RENESAS_OS_ABSTRACTION_LIMITATIONS
 * @par Known Limitations
 * NONE
 *
 * @anchor RENESAS_OS_ABSTRACTION_INSTANCES
 * @par Known Implementations
 * @ref NONE_YET
 *
 * @anchor RENESAS_OS_ABSTRACTION_SEE_ALSO
 * @ref DS_BOARD_SUPPORT,
 * @ref RZA1H_RSK_OSTM_DRIVER,
 * @ref RZA1H_RSK_LED
 * @{
 **********************************************************************************************************************/
#include "r_typedefs.h"

#ifndef SRC_RENESAS_APPLICATION_INC_R_OS_ABSTRACTION_API_H_
#define SRC_RENESAS_APPLICATION_INC_R_OS_ABSTRACTION_API_H_

#define R_RTOS_ABSTRACTION_FREERTOS			(0)
#define R_RTOS_ABSTRACTION_AZURE			(1)
#define R_RTOS_ABSTRACTION R_RTOS_ABSTRACTION_AZURE

/** Major Version Number of API. */
#define R_OS_ABSTRACTION_RZ_HLD_VERSION_MAJOR      (0)
/** Minor Version Number of API. */
#define R_OS_ABSTRACTION_RZ_HLD_VERSION_MINOR      (1)
/** Unique ID. */
#define R_OS_ABSTRACTION_RZ_UID                    (80)

/** Maximum timeout used in wait functions inside the OS abstraction module */
#define R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE      (0xFFFFFFFFUL)

/** Invalid handle used in functions inside the OS abstraction module */
#define R_OS_ABSTRACTION_PRV_INVALID_HANDLE        (-1)

/** Stack sizes, these indexes are mapped to actual sizes inside the OS abstraction module */
#define R_OS_ABSTRACTION_PRV_TINY_STACK_SIZE       (0)
#define R_OS_ABSTRACTION_PRV_SMALL_STACK_SIZE      (1)
#define R_OS_ABSTRACTION_PRV_DEFAULT_STACK_SIZE    (2)
#define R_OS_ABSTRACTION_PRV_LARGE_STACK_SIZE      (3)
#define R_OS_ABSTRACTION_PRV_HUGE_STACK_SIZE       (4)
#define R_OS_ABSTRACTION_PRV_DEFAULT_HEAP          (0)

#define R_OS_ABSTRACTION_PRV_MAX_TASK_NAME_SIZE    (24)

/** Milliseconds to system ticks */
#ifndef OS_MS_TO_SYSTICKS
   #define OS_MS_TO_SYSTICKS(n) (n)
#endif

/** System ticks to milliseconds */
#ifndef OS_SYSTICKS_TO_MS
   #define OS_SYSTICKS_TO_MS(n) (n)
#endif

/* Un-commenting LOG_TASK_INFO enables logging of mallocs and frees to a buffer to enable output in os_assert */
/* #define LOG_TASK_INFO   (1) */

//typedef void (*PTASKFN) (void *pParameter);

/** Event state object */
typedef enum _event_state_t
{
    EV_RESET = 0,
    EV_SET,
    EV_INVALID
} e_event_state_t;

/* message structures */
typedef void *os_msg_t;

typedef void *os_msg_queue_handle_t;

/** semaphore handle object */
typedef uint32_t* semaphore_t;

/** event handle object */
typedef void *event_t;

/** pointer to event handle object */
typedef event_t* pevent_t;

/** pointer to event handle object pointer */
typedef pevent_t* ppevent_t;

/** system time object */
typedef uint32_t systime_t;

/** task handle object */
typedef void os_task_t;

/** task body prototype */
typedef void (*os_task_code_t)(void *params);

/** OS Abstraction System Initialise Kernel
 *  @brief     Generic error handler, allows use to continue execution.
 *  @param[in] file - file in which the error occurred.
 *  @param[in] line - line where the error occurred.
 *  @retval    NONE.
*/
void R_OS_AssertCalled( volatile const char * pcFile, volatile unsigned long ulLine );

/* Kernel management */
/** OS Abstraction System Initialise Memory Manager
 *  @brief     Function to configure memory resources for the connected OS or scheduler,
 *             or configured an OS-Less sample.
 *  @retval    NONE.
*/
void R_OS_InitMemManager(void);

/** OS Abstraction System Initialise Kernel
 *  @brief     Function to configure critical resources for the connected OS or scheduler,
 *             or configure an OS-Less sample.
 *  @retval    NONE.
*/
void R_OS_InitKernel(void);

/** OS Abstraction System Start Kernel
 *  @brief     Function to enable the connected OS or scheduler, or configure an OS-Less sample.
 *  @retval    NONE.
*/
void R_OS_StartKernel(void);

/** OS Abstraction System Stop Kernel
 *  @brief     Function to stop the connected OS or scheduler, or configure an OS-Less sample. Provided for
 *             completeness, may never be used. When powering down a system safely this function should
 *             be called.
 *  @retval    NONE.
*/
void R_OS_StopKernel(void);

/* Task management */
/** OS Abstraction Create Task Function
 *  @brief Function to create a new task.
 *  @param[in] name ASCII character representation for the name of the Task.
 *  @warning   name string may be subject to length limitations. There is a security risk if the name
 *             is not bounded effectively in the implementation.
 *  @param[in] task_code Function pointer to the implementation of the Task.
 *  @param[in] params Structure to be used by the Task.
 *  @param[in] stack_size Stack size for allocation to the Task.
 *  @param[in] priority Task priority in system context.
 *  @retval    os_task_t The task object.
 */
os_task_t *R_OS_CreateTask (const char_t *name, os_task_code_t task_code, void *params, size_t stack_size,
        int_t priority);

/** OS Abstraction Delete Task Function
 *  @brief Function to delete a task.
 *  @warning   The target OS is responsible for verifying the Task is valid to delete.
 *  @param[in] task the task object.
 *  @retval    None.
*/
void   R_OS_DeleteTask(os_task_t* task);

/** OS Abstraction Sleep Task Function
 *  @brief     Function to cause a task to suspend and pass control back to the OS / scheduler for a requested period.
 *  @warning   The time stated is a minimum, higher priority tasks may prevent this Task form being restored immediately.
 *  @param[in] sleep_ms Time in ms (uint32 => max ~49 Days).
 *  @retval    None.
*/
void   R_OS_TaskSleep(uint32_t sleep_ms);

/** OS Abstraction Yield Function
 *  @brief     Function to cause a task to suspend and pass control back to the OS / scheduler.
 *  @retval    None.
*/
void   R_OS_Yield(void);

/** OS Abstraction Suspend Task Function
 *  @brief     Function to cause a task to suspend and pass control back to the OS / scheduler.
 *  @param[in] task the task object.
 *  @retval    None.
*/
bool_t   R_OS_SuspendTask(os_task_t* task);

/** OS Abstraction Resume Task Function
 *  @brief     Function to cause a task to suspend and pass control back to the OS / scheduler.
 *  @param[in] task the task object.
 *  @retval    None.
*/
bool_t   R_OS_ResumeTask(os_task_t* task);


void   R_OS_SuspendAllTasks(void);

void   R_OS_ResumeAllTasks(void);


/** OS Abstraction
 *  @brief Function to obtain total number of active tasks defined in the system, only attempted if the operating
 *  system is running.
 *  @retval    NONE.
*/
void R_OS_TaskUsesFloatingPoint(void);

/** OS Abstraction Get Number of defined tasks Function
 *  @brief Function to obtain total number of active tasks defined in the system, only attempted if the operating
 *  system is running.
 *  @retval    NONE.
*/
uint32_t   R_OS_GetNumberOfTasks(void);

/* Locking management */
/** OS Abstraction System Lock Function
 *  @brief Function to lock a critical section.
 *  @warning This function must prevent the OS or scheduler from swapping context. This is often implemented by
 *           preventing system interrupts form occurring, and so pending any OS timer interruptions. Timing is
 *           critical, code protected by this function must be able to complete in the minimum time possible and
 *           never block.
 *  @param[in] p - Pointer to an operating system locking object or NULL if not required.
 *  @retval    lock value.
*/
int_t  R_OS_SysLock(void *p);

/** OS Abstraction System UnLock Function
 *  This function releases the OS or scheduler to normal operation. Timing is critical, code
 *  proceeding this function must be able to complete in the minimum time possible and never block.  */
void   R_OS_SysUnlock(void *p, int_t n);

void   R_OS_SysWaitAccess(void);

void   R_OS_SysReleaseAccess(void);

/* Memory management */
void *R_OS_AllocMem (size_t size, uint32_t region);

void   R_OS_FreeMem(void *p);

/* Semaphore management */
/** OS Abstraction CreateSemaphore Function
 *  @brief     Create a semaphore.
 *  @param[in] semaphore_ptr Pointer to a associated semaphore.
 *  @param[in] count The maximum count for the semaphore object. This value must be greater than zero
 *  @return    The function returns TRUE if the semaphore object was successfully created. Otherwise, FALSE is returned
 */
bool_t R_OS_CreateSemaphore (semaphore_t semaphore_ptr, uint32_t count);

/** OS Abstraction DeleteSemaphore Function
 *  @brief     Delete a semaphore, freeing any associated resources.
 *  @param[in] semaphore_ptr Pointer to a associated semaphore.
 *  @return    none.
*/
void   R_OS_DeleteSemaphore(semaphore_t  semaphore_ptr);

/** OS Abstraction WaitSemaphore Function
 *  @brief     Blocks operation until one of the following occurs <br>
 *             A timeout occurs. <br>
 *             The associated semaphore has been set. <br>
 *  @param[in] semaphore_ptr Pointer to a associated semaphore.
 *  @param[in] timeout Maximum time to wait for associated event to occur.
 *  @return    The function returns TRUE if the semaphore object was successfully set. Otherwise, FALSE is returned.
*/
bool_t R_OS_WaitForSemaphore(semaphore_t semaphore_ptr, systime_t timeout);

/** OS Abstraction DeleteSemaphore Function
 *  @brief     Release a semaphore, freeing freeing it to be used by another task.
 *  @param[in] semaphore_ptr Pointer to a associated semaphore.
 *  @return    none.
*/
void   R_OS_ReleaseSemaphore(semaphore_t semaphore_ptr);

/* Mutex management */
void * R_OS_CreateMutex (void);

void   R_OS_DeleteMutex(void *mutex);

void   R_OS_AcquireMutex(void *mutex);

void   R_OS_ReleaseMutex(void *mutex);

bool_t R_OS_EventWaitMutex(pevent_t pEvent, uint32_t dwTimeOut);

void   R_OS_EventReleaseMutex(pevent_t pEvent);

/* Queue management */

/** OS Abstraction CreateMessageQueue Function
 *  @brief     Create an Message Queue.
 *  @param[in] queue_sz Maximum number of elements in queue.
 *  @param[in] pp_queue_handle pointer to queue handle pointer.
 *  @return    The function returns TRUE if the message queue was successfully created. Otherwise, FALSE is returned
 */
bool_t R_OS_CreateMessageQueue (uint32_t queue_sz, os_msg_queue_handle_t *p_queue_handle);

/** OS Abstraction PutMessageQueue Function
 *  @brief     Put a message onto a queue.
 *  @param[in] p_queue_handle pointer to queue handle.
 *  @param[in] p_message pointer to message.
 *  @return    The function returns TRUE if the event object was successfully added to the queue. Otherwise, FALSE is returned
 */
bool_t R_OS_PutMessageQueue (os_msg_queue_handle_t p_queue_handle, os_msg_t p_message);

/** OS Abstraction GetMessageQueue Function
 *  @brief     Get a message from a queue.
 *  @param[in] p_queue_handle pointer to queue handle.
 *  @param[out] ppmsg pointer to message pointer. Pointer will point to NULL if no message and times out.
 *  @param[in] timeout in system ticks.
 *  @param[in] blocking true = block thread/task until message received.False = not blocking
 *  @return    The function returns TRUE if the event object was successfully retrieved from the queue. Otherwise, FALSE is returned
 */
bool_t R_OS_GetMessageQueue (os_msg_queue_handle_t queue, os_msg_t *ppmsg, uint32_t timeout, bool_t blocking);

/** OS Abstraction ClearMessageQueue Function
 *  @brief     Clear a message queue.
 *  @param[in] p_queue_handle pointer to queue handle.
 *  @return    The function returns TRUE if the event object was successfully cleared. Otherwise, FALSE is returned
 */
bool_t R_OS_ClearMessageQueue (os_msg_queue_handle_t *p_queue_handle);

/** OS Abstraction DeleteMessageQueue Function
 *  @brief     Delete a message queue. The message queue pointer argument will be set to NULL.
 *  @param[in] pp_queue_handle pointer to queue handle pointer.
 *  @return    The function returns TRUE if the event object was successfully deleted. Otherwise, FALSE is returned
 */
bool_t R_OS_DeleteMessageQueue (os_msg_queue_handle_t *pp_queue_handle);

/** OS Abstraction DeleteMessageQueue Function
 *  @brief     Enter critical area of code - prevent context switches.
 */
void R_OS_EnterCritical (void);

/** OS Abstraction ExitCritical Function
 *  @brief     Exit critical area.
 */
void R_OS_ExitCritical (void);

/* Event Management */
/** OS Abstraction CreateEvent Function
 *  @brief     Create an event.
 *  @param[in] event_ptr Pointer to a associated event.
 *  @return    The function returns TRUE if the event object was successfully created. Otherwise, FALSE is returned
*/
bool_t R_OS_CreateEvent(pevent_t event_ptr);

/** OS Abstraction DeleteEvent Function
 *  @brief     Delete an event, freeing any associated resources.
 *  @param[in] event_ptr Pointer to a associated event.
 *  @return    none
*/
void   R_OS_DeleteEvent(pevent_t event_ptr);

/** OS Abstraction SetEvent Function
 *  @brief     Sets the state on the associated event.
 *  @param[in] event_ptr Pointer to a associated event.
 *  @return    none.
*/
void   R_OS_SetEvent(pevent_t event_ptr);

/** OS Abstraction ResetEvent Function
 *  @brief     Clears the state on the associated event.
 *  @param[in] event_ptr Pointer to a associated event.
 *  @return    none.
*/
void   R_OS_ResetEvent(pevent_t event_ptr);

/** OS Abstraction EventState Function
 *  @brief     Returns the state on the associated event.
 *  @param[in] event_ptr Pointer to a associated event.
 *  @return    EV_RESET
 *             EV_SET
 *             EV_INVALID
 *  .
*/
e_event_state_t R_OS_EventState(pevent_t event_ptr);

/** OS Abstraction WaitForEvent Function
 *  @brief     Blocks operation until one of the following occurs <br>
 *             A timeout occurs. <br>
 *             The associated event has been set. <br>
 *  @param[in] event_ptr Pointer to a associated event.
 *  @param[in] timeout Maximum time to wait for associated event to occur.
 *  @return    The function returns TRUE if the event object was set, Otherwise, FALSE is returned
*/
bool_t R_OS_WaitForEvent(pevent_t event_ptr, systime_t timeout);

/** OS Abstraction SetEventFromIsr Function
 *  @brief     Sets the state on the associated event
 *  @warning   Function shall only be caller from within an ISR routine
 *  @param[in] event_ptr Pointer to a associated event
 *  @return    The function returns TRUE if the event object was successfully set. Otherwise, FALSE is returned
*/
bool_t R_OS_SetEventFromIsr(pevent_t event_ptr);

/** OS Abstraction R_OS_TaskGetPriority Function
 *  @brief     Gets current task priority
 *  @warning   Function shall when the scheduler is running
 *  @param[in] uiTaskID desired Task
 *  @return    The function returns the task priority of the specified uiTaskID
 *  @return    -1 if the uiTaskID can not be found
*/
int32_t R_OS_TaskGetPriority(uint32_t uiTaskID);

/** OS Abstraction R_OS_TaskSetPriority Function
 *  @brief     Sets current task priority
 *  @warning   Function shall when the scheduler is running
 *  @param[in] uiTaskID desired task
 *  @param[in] uiPriority desired priority
 *  @return    true if priority is set
 *  @return    false if priority can not be set
*/
bool_t R_OS_TaskSetPriority(uint32_t uiTaskID, uint32_t uiPriority);

/** OS Abstraction R_OS_GetCurrentTask Function
 *  @brief     Gets current task
 *  @warning   Function shall when the scheduler is running
 *  @param[in] none
 *  @return    The function returns the current running task
*/
os_task_t* R_OS_GetCurrentTask (void);

/** OS Abstraction R_OS_GetCurrentTask Function
 *  @brief     Gets text name of current task
 *  @warning   Function shall when the scheduler is running
 *  @param[in] none
 *  @return    The function returns a pointer to the text name of the current task
*/
char *R_OS_GetCurrentTaskName (void);

/** OS Abstraction R_OS_GetTaskState Function
 *  @brief     Gets status information on selected task in human readable form.
 *  @warning   Function shall when the scheduler is running
 *  @param[in] task task name in human readable form.
 *  @return    The function returns a character string that can be displayed on a console.
*/
char * R_OS_GetTaskState (const char* task);


/** OS Abstraction GetVersion Function
 *  @brief     Obtains the version information from this module
 *  @param[in] pinfo Structure containing module version information.
 *  @return    The function returns TRUE if the event object was successfully set. Otherwise, FALSE is returned
*/
//int32_t R_OS_GetVersion(st_drv_info_t* pinfo);

#endif /* SRC_RENESAS_APPLICATION_INC_R_OS_ABSTRACTION_API_H_ */

/*******************************************************************************************************************//**
 * @} (end addtogroup OS_ABSTRACT)
 ***********************************************************************************************************************/
