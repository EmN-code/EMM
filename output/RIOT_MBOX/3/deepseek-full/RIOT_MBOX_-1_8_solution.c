
#include <stdio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <mma8x5x_regs.h>
#include <timex.h>
#include <ztimer.h>
#include <msg.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mbox.h>
#include <xtimer.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void _evtimer_msg_handler(evtimer_event_t *event)
{
    evtimer_mbox_event_t *mbox_event = (evtimer_mbox_event_t *)event;
    msg_t msg;
    msg.content.value = 1; // Example message content
    msg_send(&msg, mbox_event->mbox);
}

int main(void)
{
    // Initialize the message queue and mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_msg(&evtimer);

    // Create an event to be added to the timer
    evtimer_mbox_event_t event;
    event.mbox = &mbox;

    // Add the event to the timer with a 1-second interval
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Main loop to receive messages
    while (1) {
        msg_t received_msg;
        msg_receive(&received_msg);
        printf("Received message with value: %d\n", received_msg.content.value);
    }

    return 0;
}
