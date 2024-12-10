
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdbool.h>
#include <mbox.h>
#include <event/thread.h>
#include <time_units.h>
#include <thread.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&evtimer);

    // Set up the periodic event
    msg.content.ptr = NULL;
    evtimer_add_msg(&evtimer, &msg, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the event timer
    evtimer_start(&evtimer);

    // Yield to allow the event timer to process events
    thread_yield();

    return 0;
}
