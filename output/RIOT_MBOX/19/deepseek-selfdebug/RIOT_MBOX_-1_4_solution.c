
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "evtimer.h"
#include "mbox.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 10
#define EVENT_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 0; // Example content
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init(&evtimer);

    // Add an event to the timer
    evtimer_add_msg(&evtimer, &mbox, NULL, EVENT_INTERVAL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, -1); // Blocking call to get a message
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
