
#include <stdio.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <timex.h>

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_event_t ev_event;

void event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    // Initialize the message queue and mailbox
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&ev_event.event);

    // Set up the event to trigger every INTERVAL_MS milliseconds
    ztimer_set(ZTIMER_MSEC, &ev_event.event.timer, INTERVAL_MS);

    // Main loop to handle events
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, TIME_T_NEVER);
        if (msg.type == EVTIMER_MSG_TYPE) {
            event_callback(NULL);
        }
    }

    return 0;
}
