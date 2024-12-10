#include "ztimer.h"
#include "timex.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"
#include "event/thread.h"
#include "xtimer.h"
#include "mbox.h"
#include "event/callback.h"

#define MAIN_QUEUE_SIZE     (8)
#define TIMER_PERIOD_MS     (1000)  // 1 second period

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_t evtimer;
static evtimer_mbox_event_t mbox_event;
static mbox_t mbox;

void _event_periodic_callback(void *arg)
{
    (void)arg; // Unused parameter
    printf("Periodic event fired\n");
}

int main(void)
{
    // Initialize Messaging Queue
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize mbox
    mbox_init(&mbox, main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize event timer with message queue
    evtimer_init_msg(&evtimer);

    // Configure the event timer mbox event
evtimer_add_mbox(&evtimer, &mbox_event, &mbox);

    // The evtimer_mbox_event_t structure which is the correct type for evtimer_add_mbox
    mbox_event.event.interval = ZTIMER_MSEC_TO_USEC(TIMER_PERIOD_MS);
    mbox_event.event.callback = _event_periodic_callback;
    mbox_event.event.args = NULL;

    // Start the event timer
    evtimer_add(&evtimer, (evtimer_event_t*)&mbox_event);

    while (1) {
        // Loop forever, handling events when necessary
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}