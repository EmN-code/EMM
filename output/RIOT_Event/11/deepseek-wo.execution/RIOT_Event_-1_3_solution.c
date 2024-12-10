
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <thread.h>
#include <xtimer.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void event_periodic_callback(void *arg)
{
    printf("Event periodic callback triggered\n");
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init(&evtimer, event_periodic_callback);

    // Add a periodic event to the event timer
    evtimer_event_t event;
    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second
    evtimer_add_periodic(&evtimer, &event, interval);

    // Main loop
    while (1) {
        msg_t msg;
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Message received\n");
        }
    }

    return 0;
}
