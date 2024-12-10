
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static evtimer_msg_event_t event;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event
    event.msg.sender_pid = thread_getpid();
    event.event.callback = periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer with a periodic interval
    while (1) {
        evtimer_add_msg(&evtimer, &event, thread_getpid());
        xtimer_sleep(PERIODIC_INTERVAL / US_PER_SEC);
    }

    return 0;
}
