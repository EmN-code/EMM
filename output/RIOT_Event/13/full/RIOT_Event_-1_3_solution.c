#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "time_units.h"
#include "event/thread.h"
#include "event/callback.h"
#include "event/periodic.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "ztimer.h"

#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)

// The event timer struct
static evtimer_t evtimer;

void event_callback(void) {
    puts("Event triggered every second!");
}

void *event_handler(void *arg) {
    (void)arg;
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        evtimer_init_msg(&evtimer);
        // Wait 1 second
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        // Call the callback function
        event_callback();
    }
    return NULL;
}

int main(void) {
    char handler_stack[THREAD_STACKSIZE_MAIN];

    thread_create(handler_stack, sizeof(handler_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_handler, NULL, "event_handler");

    // Start the event loop
    xtimer_sleep(10); // Let it run for 10 seconds just for demonstration purposes

    return 0;
}