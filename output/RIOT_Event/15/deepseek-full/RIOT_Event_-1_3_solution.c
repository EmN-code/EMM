
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static msg_t msg_array[8];
static msg_t msg_queue[8];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Create a periodic event
    evtimer_msg_event_t event;
    event.event.handler = (evtimer_callback_t)periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer with a 1-second interval
    ztimer_t timer;
    timer.callback = (ztimer_callback_t)evtimer_add_msg;
    timer.arg = &event;
    timer.target_pid = thread_getpid();

    while (1) {
        // Set the timer to trigger the event every second
        ztimer_set(ZTIMER_USEC, &timer, PERIODIC_INTERVAL);

        // Wait for the event to be triggered
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
