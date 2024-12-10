
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mma8x5x_regs.h>
#include <stddef.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static mbox_t mbox;
static msg_t msg_array[8];
static evtimer_t evtimer;

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
    // Re-add the event to keep the periodic behavior
    evtimer_add_msg(&evtimer, &msg_array[0], PERIODIC_INTERVAL);
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Add the initial event to the event timer
    evtimer_add_msg(&evtimer, &msg_array[0], PERIODIC_INTERVAL);

    // Start the periodic event
    periodic_callback(NULL);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg, 1) == 1) {
            // Handle the message if needed
        }
    }

    return 0;
}
