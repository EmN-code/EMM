
#include <stdio.h>
#include <xtimer.h>
#include <msg.h>
#include <evtimer.h>

#define INTERVAL_MS (1000000U)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Create events for the event timer
    evtimer_msg_event_t events[3];
    for (int i = 0; i < 3; i++) {
        evtimer_msg_event_init_msg(&events[i], event_handler, NULL, &mbox);
    }

    // Add events to the event timer
    for (int i = 0; i < 3; i++) {
        evtimer_add_msg(&evtimer, &events[i], (i + 1) * INTERVAL_MS);
    }

    // Main loop to receive messages from the mailbox
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received!\n");
    }

    return 0;
}
