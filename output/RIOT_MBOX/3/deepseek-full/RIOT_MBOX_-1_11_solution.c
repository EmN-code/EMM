
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

// Define the event timer and message box
evtimer_t evtimer;
mbox_t mbox;
msg_t msg_queue[8];

// Event handler for the event timer
void _evtimer_msg_handler(evtimer_event_t *event)
{
    msg_t msg;
    msg.content.value = 1; // Example message content
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Create an event to be added to the event timer
    evtimer_mbox_event_t event;
    event.mbox = &mbox;
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Set a timer to trigger the event at intervals
    xtimer_t timer;
    timer.target = xtimer_now_usec() + 1000000; // 1 second interval
    timer.long_target = timer.target;
    xtimer_set(&timer, 1000000); // 1 second interval

    // Receive messages from the message box
    msg_t received_msg;
    while (1) {
        msg_receive(&received_msg);
        printf("Received message with value: %d\n", (int)received_msg.content.value);
    }

    return 0;
}
