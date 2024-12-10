
#include "event/thread.h"
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "mutex.h"
#include "stdio.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "stddef.h"
#include "mbox.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static evtimer_event_t event;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&evtimer, event_periodic_callback, NULL);

    // Set the event to trigger every second
    xtimer_ticks32_t interval = xtimer_ticks_from_usec(TIMER_INTERVAL);
    event.event.offset = interval;
    event.event.next = &event.event;

    // Add the event to the event timer
    evtimer_add(&evtimer, &event.event);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
