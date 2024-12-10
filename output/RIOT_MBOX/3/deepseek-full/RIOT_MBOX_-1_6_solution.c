
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <mma8x5x_regs.h>
#include <timex.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    // Initialize the message queue and mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Create an event to be added to the event timer
    evtimer_mbox_event_t event;
    event.event.handler = event_handler;
    event.event.arg = NULL;

    // Add the event to the event timer with a 2-second interval
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        if (msg_receive(&msg) > 0) {
            printf("Message received!\n");
        }
        // Simulate some other work
        xtimer_sleep(1);
    }

    return 0;
}
