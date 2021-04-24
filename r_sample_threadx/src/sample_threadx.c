/* This is a small demo of the high-performance ThreadX kernel.  It includes examples of eight
   threads of different priorities, using a message queue, semaphore, mutex, event flags group, 
   byte pool, and block pool.  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "compiler_settings.h"
#include "iodefine_cfg.h"
#include "control.h"

#include "tx_api.h"
#include "r_os_abstraction_api.h"

#define     DEMO_STACK_SIZE         1024
#define     DEMO_BYTE_POOL_SIZE     9120
#define     DEMO_BLOCK_POOL_SIZE    100
#define     DEMO_QUEUE_SIZE         100


/* Define the ThreadX object control blocks...  */

os_task_t               thread_0;
os_task_t               thread_1;
os_task_t               thread_2;
os_task_t               thread_3;
os_task_t               thread_4;
os_task_t               thread_5;
os_task_t               thread_6;
os_task_t               thread_7;
os_msg_queue_handle_t   queue_0;
psemaphore_t            semaphore_0;
event_t	                mutex_0;
event_t	    			event_flags_0;
TX_BYTE_POOL            byte_pool_0;
TX_BLOCK_POOL           block_pool_0;


/* Define the counters used in the demo application...  */

ULONG           thread_0_counter;
ULONG           thread_1_counter;
ULONG           thread_1_messages_sent;
ULONG           thread_2_counter;
ULONG           thread_2_messages_received;
ULONG           thread_3_counter;
ULONG           thread_4_counter;
ULONG           thread_5_counter;
ULONG           thread_6_counter;
ULONG           thread_7_counter;


/* Define thread prototypes.  */

void    thread_0_entry(ULONG thread_input);
void    thread_1_entry(ULONG thread_input);
void    thread_2_entry(ULONG thread_input);
void    thread_3_and_4_entry(ULONG thread_input);
void    thread_5_entry(ULONG thread_input);
void    thread_6_and_7_entry(ULONG thread_input);


/* Define main entry point.  */

int main(void)
{

    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}


/* Define what the initial system looks like.  */

void    tx_application_define(void *first_unused_memory)
{
	uint32_t param;
	/* Create the main thread.  */
	param = 0;
	R_OS_CreateTask ( "thread 0", thread_0_entry, 		(void*)&param, DEMO_STACK_SIZE, 1);

	/* Create threads 1 and 2. These threads pass information through a ThreadX
	   message queue.  It is also interesting to note that these threads have a time
	slice.  */
	param = 1;
	R_OS_CreateTask ( "thread 1", thread_1_entry, 		(void*)&param, DEMO_STACK_SIZE, 16);
	param = 2;
	R_OS_CreateTask ( "thread 2", thread_2_entry, 		(void*)&param, DEMO_STACK_SIZE, 16);

	/* Create threads 3 and 4.  These threads compete for a ThreadX counting semaphore.
   	   An interesting thing here is that both threads share the same instruction area.  */
	param = 3;
	R_OS_CreateTask ( "thread 3", thread_3_and_4_entry, (void*)&param, DEMO_STACK_SIZE, 8);
	param = 4;
	R_OS_CreateTask ( "thread 4", thread_3_and_4_entry, (void*)&param, DEMO_STACK_SIZE, 8);

	/* Create thread 5.  This thread simply pends on an event flag which will be set
	   by thread_0.  */
	param = 5;
	R_OS_CreateTask ( "thread 5", thread_5_entry, 		(void*)&param, DEMO_STACK_SIZE, 4);

	/* Create threads 6 and 7.  These threads compete for a ThreadX mutex.  */
	param = 6;
	R_OS_CreateTask ( "thread 6", thread_6_and_7_entry, (void*)&param, DEMO_STACK_SIZE, 8);
	param = 7;
	R_OS_CreateTask ( "thread 1", thread_6_and_7_entry, (void*)&param, DEMO_STACK_SIZE, 8);

	/* Create the event flags group used by threads 1 and 5.  */
	R_OS_CreateEvent ( &event_flags_0 );

	/* Create the message queue shared by threads 1 and 2.  */
	R_OS_CreateMessageQueue( DEMO_QUEUE_SIZE, &queue_0 );

	/* Create the semaphore used by threads 3 and 4.  */
	R_OS_CreateSemaphore(&semaphore_0, 1);

	/* Create the mutex used by thread 6 and 7 without priority inheritance.  */
	mutex_0 = R_OS_CreateMutex();

}


/* Define the test threads.  */

void    thread_0_entry(ULONG thread_input)
{
	printf("Created Thread %d\n", thread_input);

    /* This thread simply sits in while-forever-sleep loop.  */
    while(1)
    {

        /* Increment the thread counter.  */
        thread_0_counter++;

        /* Sleep for 10 ticks.  */
        R_OS_TaskSleep(10);

        /* Set event flag 0 to wakeup thread 5.  */
        if (!R_OS_SetEvent( event_flags_0 ) )
        	break;

    }
}


void    thread_1_entry(ULONG thread_input)
{
	printf("Created Thread %d\n", thread_input);

	thread_1_messages_sent = 0;

	/* This thread retrieves messages placed on the queue by thread 1.  */
	while(1)
	{

		/* Increment the thread counter.  */
		thread_1_counter++;

		/* Sleep for 10 ticks.  */
		R_OS_TaskSleep(10);

		/* Retrieve a message from the queue.  */
		if ( !R_OS_PutMessageQueue(queue_0, (void*)&thread_1_messages_sent )) {
			/* Check completion status.  */
			break;
		}


		/* Increment the message sent.  */
		thread_1_messages_sent++;
	}

}


void    thread_2_entry(ULONG thread_input)
{
	printf("Created Thread %d\n", thread_input);

	ULONG   received_message;
	UINT    status;

	    /* This thread retrieves messages placed on the queue by thread 1.  */
	    while(1)
	    {

	        /* Increment the thread counter.  */
	        thread_2_counter++;

	        /* Retrieve a message from the queue.  */
	        if ( !R_OS_GetMessageQueue(queue_0, &received_message, TX_WAIT_FOREVER, true)) {
	        	/* Check completion status and make sure the message is what we
	        		           expected.  */
	        	break;
	        }

	        /* Otherwise, all is okay.  Increment the received message count.  */
	        thread_2_messages_received++;
	    }
}


void    thread_3_and_4_entry(ULONG thread_input)
{

	UINT    status;

	printf("Created Thread %d\n", thread_input);

    /* This function is executed from thread 3 and thread 4.  As the loop
       below shows, these function compete for ownership of semaphore_0.  */
    while(1)
    {

        /* Increment the thread counter.  */
        if (thread_input == 3)
            thread_3_counter++;
        else
            thread_4_counter++;

        /* Get the semaphore with suspension.  */
        if ( !R_OS_WaitForSemaphore( semaphore_0, TX_WAIT_FOREVER)) {
        	/* Check status.  */
        	break;
        }

        /* Sleep for 2 ticks to hold the semaphore.  */
        R_OS_TaskSleep(2);

        /* Release the semaphore.  */
        if ( !R_OS_ReleaseSemaphore( semaphore_0) ) {
        	/* Check status.  */
        	break;
        }

    }
}


void    thread_5_entry(ULONG thread_input)
{

	UINT    status;
	ULONG   actual_flags;

	printf("Created Thread %d\n", thread_input);


    /* This thread simply waits for an event in a forever loop.  */
    while(1)
    {

        /* Increment the thread counter.  */
        thread_5_counter++;

        /* Wait for event flag 0.  */
        actual_flags = R_OS_WaitForEvent ( event_flags_0, R_OS_ABSTRACTION_PRV_EV_WAIT_INFINITE);


        /* Check status.  */
        if (actual_flags != 0x1)
            break;
    }
}


void    thread_6_and_7_entry(ULONG thread_input)
{

	UINT    status;

	printf("Created Thread %d\n", thread_input);


    /* This function is executed from thread 6 and thread 7.  As the loop
       below shows, these function compete for ownership of mutex_0.  */
    while(1)
    {

        /* Increment the thread counter.  */
        if (thread_input == 6)
            thread_6_counter++;
        else
            thread_7_counter++;

        /* Get the mutex with suspension.  */
        if ( !R_OS_AcquireMutex(mutex_0)) {
        	/* Check status.  */
        	break;
        }

        /* Get the mutex again with suspension.  This shows
           that an owning thread may retrieve the mutex it
           owns multiple times.  */
        if ( !R_OS_AcquireMutex(mutex_0)) {
			/* Check status.  */
			break;
		}

        /* Sleep for 2 ticks to hold the mutex.  */
        R_OS_TaskSleep(2);

        /* Release the mutex.  */
        if ( !R_OS_ReleaseMutex(mutex_0) ) {
        	break;
        }


        /* Release the mutex again.  This will actually 
           release ownership since it was obtained twice.  */
        if ( !R_OS_ReleaseMutex(mutex_0) ) {
        	break;
        }
    }
}
