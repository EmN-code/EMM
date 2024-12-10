
#include "xtimer.h"
#include "evtimer.h"
#include "thread.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_t evtimer;
static evtimer_event_t event;

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init(&evtimer);

    // Set up the event
    event.callback = event_periodic_callback;
    event.arg = NULL;

    // Add the event to the event timer with a 1-second interval
    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000);
    evtimer_add_absolute(&evtimer, &event, xtimer_now() + interval);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
