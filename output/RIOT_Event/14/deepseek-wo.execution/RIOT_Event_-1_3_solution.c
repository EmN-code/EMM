
#include <stdlib.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <stddef.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg_queue[8];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer with the periodic callback
    evtimer_init_mbox(&evtimer);

    // Set up the periodic event
    evtimer_mbox_add_msg(&evtimer, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the periodic event
    evtimer_mbox_start(&evtimer);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
