#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "stdio.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer interval of 1 second

void timer_callback(void *arg) {
    (void)arg; // suppress unused parameter warning
    printf("Timer triggered. Turning on LED.\n");
    // Assume the LED is toggled here. The actual implementation will depend on the specific GPIO handling.
}

int main(void) {
    evtimer_msg_event_t timer_event;
    msg_t msg;

    // Initialize the event timer
    ztimer_clock_t *clock = ZTIMER_USEC;
    evtimer_msg_init(&timer_event);
    evtimer_mbox_evtimer_init(&timer_event);
    mbox_register(&timer_event.mbox, evtimer_msg_handler, &timer_event.mbox);

    // Setup the timer to trigger every second
    ztimer_set(&clock, &timer_event.timer, TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        // Wait for message - this is where the timer events are processed
        mbox_get(&timer_event.mbox, &msg);
        // process the message, in our case, it will simply call the callback function
        evtimer_msg_run(&timer_event);
    }

    return 0;
}