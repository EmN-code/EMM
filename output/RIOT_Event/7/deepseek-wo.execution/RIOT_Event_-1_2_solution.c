
#include <stdio.h>
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "timex.h"
#include "event/thread.h"
#include "time_units.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t periodic_timer;
static msg_t periodic_msg;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the periodic timer
    evtimer_init(&periodic_timer);

    // Set up the periodic message
    periodic_msg.content.ptr = NULL;
    periodic_msg.type = 0;

    // Add the periodic event to the timer
    evtimer_add_msg(&periodic_timer, PERIODIC_INTERVAL, &periodic_msg, periodic_callback, NULL);

    // Start the periodic timer
    evtimer_start(&periodic_timer);

    // Yield to allow the event to be processed
    thread_yield();

    return 0;
}
