
#include "xtimer.h"
#include "evtimer.h"
#include "evtimer_msg.h"
#include "thread.h"

#define EVENT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define EVENT_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

static char event_thread_stack[EVENT_THREAD_STACKSIZE];
static evtimer_msg_t evtimer;
static evtimer_msg_event_t event;

static void event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event
    event.event.callback = event_callback;
    event.event.arg = NULL;

    // Add the event to the timer to trigger every second
    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000);
    evtimer_add_msg(&evtimer, &event, interval, interval, thread_getpid());

    // Start the event thread
    thread_create(event_thread_stack, sizeof(event_thread_stack), EVENT_THREAD_PRIO,
                  THREAD_CREATE_STACKTEST, event_thread_init, &evtimer, "event_thread");

    return 0;
}
