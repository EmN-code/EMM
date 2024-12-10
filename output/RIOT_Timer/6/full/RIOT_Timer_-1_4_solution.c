#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "time_units.h"

#define TIMER_INTERVAL 1000000 // 1 second

static void timer_callback(void *arg) {
    (void)arg;
    // Simulate LED toggle with a print statement
    printf("LED toggled\n");
}

int main(void) {
    ztimer_t timer;
    msg_t msg_queue[8];

    // Initialize message queue
    msg_init_queue(msg_queue, sizeof(msg_queue)/sizeof(msg_queue[0]));

    // Setup the timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);
    printf("Timer set for %d microseconds\n", TIMER_INTERVAL);

    while (1) {
        xtimer_sleep(1); // Reduce CPU usage by sleeping
    }
    return 0;
}