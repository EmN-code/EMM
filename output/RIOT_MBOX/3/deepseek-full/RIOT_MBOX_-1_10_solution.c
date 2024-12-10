
#include <stdio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <ztimer.h>
#include <msg.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mbox.h>
#include <xtimer.h>

// Define the message queue size
#define MSG_QUEUE_SIZE 8

// Define the event timer
evtimer_t evtimer;

// Define the message box
mbox_t mbox;

// Define the message queue
msg_t msg_queue[MSG_QUEUE_SIZE];

// Define the event
evtimer_mbox_event_t event;

// Event handler function
void _evtimer_msg_handler(evtimer_event_t *event)
{
    // Cast the event to the correct type
    evtimer_mbox_event_t *mbox_event = (evtimer_mbox_event_t *)event;

    // Send the message to the message box
    msg_t msg;
    msg.content.value = 1; // Example message content
    mbox_put(mbox_event->mbox, &msg);
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event
    event.mbox = &mbox;
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Set the timer to trigger the event every 2 seconds
    xtimer_t timer;
    timer.target = xtimer_now();
    timer.long_target = timer.target + (2 * US_PER_SEC);
    xtimer_set(&timer, 2 * US_PER_SEC);

    // Main loop to receive messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}
