
#include <stdio.h>
#include <xtimer.h>
#include <evtimer.h>
#include <mbox.h>
#include <msg.h>

#define INTERVAL_MS (1000 * 1000U)  // 1 second interval

static mbox_t mbox;
static msg_t msg_queue[8];

static void _evtimer_mbox_handler(void *arg)
{
    (void)arg;
    printf("Event received at %" PRIu32 "\n", xtimer_now_usec());
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_t evtimer;
    evtimer_init(&evtimer);

    // Create an event to be added to the event timer
    evtimer_mbox_event_t event;
    event.event.handler = (event_handler_t)_evtimer_mbox_handler;
    event.event.arg = NULL;
    event.mbox = &mbox;

    // Add the event to the event timer with the specified interval
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Main loop to keep the program running
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        // Handle the received message if needed
    }

    return 0;
}
