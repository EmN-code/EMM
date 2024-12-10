
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <mma8x5x_params.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg_queue[8];
static mbox_t mbox;

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&evtimer, periodic_callback, NULL);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Add a periodic event to the event timer
    evtimer_event_t event;
    event.next = (evtimer_event_t *)ztimer_now(ZTIMER_USEC) + PERIODIC_INTERVAL;
    evtimer_add(&evtimer, &event);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg, 0) == 1) {
            // Handle the received message
            printf("Message received\n");
        }
    }

    return 0;
}
