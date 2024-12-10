
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t periodic_timer;
static msg_t periodic_msg;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    puts("Starting periodic event example");

    // Initialize the periodic timer
    periodic_msg.content.ptr = NULL;
    periodic_msg.type = 0;

    evtimer_init_msg(&periodic_timer);
    evtimer_add_msg(&periodic_timer, &periodic_msg, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the periodic timer
    evtimer_add_msg(&periodic_timer, &periodic_msg, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Yield to allow the event to be processed
    thread_yield();

    return 0;
}
