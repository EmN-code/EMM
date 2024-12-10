
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <MQTTClient.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init(&evtimer);

    // Create an event
    evtimer_mbox_event_t event;
    event.event.callback = event_callback;
    event.event.arg = NULL;

    // Add the event to the timer with a 2-second interval
    ztimer_set(ZTIMER_MSEC, &event.event.timer, 2000);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Main loop to process events
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            event_callback(msg.content.ptr);
        }
    }

    return 0;
}
