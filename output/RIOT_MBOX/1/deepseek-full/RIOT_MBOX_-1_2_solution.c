
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "timex.h"

#define MSG_QUEUE_SIZE 16
#define EVENT_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;
static evtimer_msg_event_t ev_event;

void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value for the message
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message queue and mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event to trigger every EVENT_INTERVAL
    ev_event.event.offset = EVENT_INTERVAL;
    ev_event.event.callback = event_callback;
    evtimer_add_msg(&evtimer, &ev_event);

    // Main loop to handle received messages
    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, TIME_T_NEVER) == 1) {
            printf("Received message with value: %d\n", (int)msg.content.value);
        }
    }

    return 0;
}
