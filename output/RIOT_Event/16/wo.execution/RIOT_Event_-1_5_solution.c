#include <unistd.h>
#include <timex.h>
#include <stdio.h>
#include <stdbool.h>
#include "kernel_defines.h"
#include "ztimer.h"
// Assume <event/callback.h>, <stdatomic.h>, <stdlib.h>, <stddef.h>, <msg.h>, <time_units.h>, <thread.h> are indirectly included by these headers.

#define INTERVAL_SEC 1

// Callback function for the timer
void timer_callback(void *arg) {
    (void)arg; // Avoid unused variable warning
    puts("Timer callback triggered.");
}

int main(void) {
    printf("Starting event periodic timer example...\n");

    // Initialize a periodic timer
    ztimer_t my_timer;
    my_timer.callback = timer_callback;
    my_timer.arg = NULL; // No argument is passed to the callback

    // Trigger the callback every INTERVAL_SEC seconds
    uint32_t interval_ticks = ztimer_usec_from_sec(INTERVAL_SEC);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, interval_ticks);
        timer_callback(NULL);
    }

    return 0;
}
