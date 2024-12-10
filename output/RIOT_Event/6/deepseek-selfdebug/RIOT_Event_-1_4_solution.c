
#include "xtimer.h"
#include "evtimer.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "msg.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t evtimer;
static msg_t msg_queue[8];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Create a periodic event
    evtimer_msg_event_t periodic_event;
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the periodic event to the event timer
    evtimer_add_msg(&evtimer, &periodic_event, thread_getpid());

    // Set the periodic event to trigger every second
    xtimer_t timer;
    timer.callback = (xtimer_callback_t)evtimer_add_msg;
    timer.arg = &periodic_event;
    xtimer_set(&timer, PERIODIC_INTERVAL);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_TYPE) {
            periodic_callback(msg.content.ptr);
        }
    }

    return 0;
}
