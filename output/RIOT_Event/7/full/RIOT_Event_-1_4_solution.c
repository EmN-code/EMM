#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE_DEFAULT 1024

static void my_event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered every second\n");
}

int main(void)
{
    msg_t msg_array[32];  // Ensure the length is a power of two
    msg_init_queue(msg_array, 32);
    
    evtimer_t evtimer;
    evtimer_msg_event_t evt;

    evtimer_set_msg(&evtimer, &evt, my_event_callback, NULL);
    
    while(1) {
        xtimer_sleep(1);
        my_event_callback(NULL);
    }

    return 0;  // This will never be reached
}