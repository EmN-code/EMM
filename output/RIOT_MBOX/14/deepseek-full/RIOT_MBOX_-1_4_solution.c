
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
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
#include "stdatomic.h"
#include "MQTTClient.h"

#define QUEUE_SIZE (16)  // Must be a power of two

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
    // Initialize the message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Create an event
    evtimer_mbox_event_t event;
    event.event.callback = event_callback;
    event.event.arg = NULL;

    // Add the event to the timer with a 1-second interval
    ztimer_set(ZTIMER_MSEC, &event.event.timer, 1000);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Main loop to receive messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received!\n");
    }

    return 0;
}
