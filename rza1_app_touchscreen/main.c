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
#include "r_touch_capacitive.h"

#define     DEMO_STACK_SIZE         1024

void    thread_0_entry(ULONG thread_input);

static st_iostr_t comSettings;

void R_MSG_WarningConfig(pst_comset_t p_com, char_t *msg)
{
    if(NULL != p_com->p_out)
    {
        fprintf(p_com->p_out,"Warning command disabled\r\n"
                "In configuration file /RZA1_Sample/src/renesas/configuration/application_cfg.h\r\n"
                "%s should be disabled when using this command\r\n", msg);
    }
}

int main(void)
{

    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}


/* Define what the initial system looks like.  */

void    tx_application_define(void *first_unused_memory)
{
	uint32_t param;
	param = 0;
	R_OS_CreateTask ( "thread 0", thread_0_entry, 		(void*)&param, DEMO_STACK_SIZE, 1);
}

/* Define the test threads.  */

void    thread_0_entry(ULONG thread_input)
{

	printf("Touch Panel Demo Thread %d\n", thread_input);

	/* Assign the streams for the serial console */
	comSettings.p_in = stdin;
	comSettings.p_out = stdout;

	R_TOUCH_ApplicationMain( &comSettings );
    /* This thread simply sits in while-forever-sleep loop.  */
    while(1)
    {
        /* Sleep for 10 ticks.  */
        R_OS_TaskSleep(10000);

    }
}
