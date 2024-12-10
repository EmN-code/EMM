
#include <stdlib.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[1];
static evtimer_t evtimer;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    // Initialize the event timer
    evtimer_init(&evtimer, periodic_callback, NULL);

    // Add a periodic event to the event timer
    evtimer_add_periodic(&evtimer, xtimer_now(), PERIODIC_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
