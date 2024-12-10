
#include "evtimer_msg.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "mbox.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void send_event(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example event value
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Set up the event timer to send events at intervals
    evtimer_msg_t evtimer;
    evtimer_init_msg(&evtimer);

    // Add a message to the event timer to be sent every 1 second
    ztimer_t timer;
    timer.callback = send_event;
    timer.arg = NULL;
    timer.interval = ZTIMER_SEC(1);
    evtimer_add_msg(&evtimer, &timer);

    // Main loop to handle incoming messages
    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Received event with value: %d\n", msg.content.value);
        }
    }

    return 0;
}
