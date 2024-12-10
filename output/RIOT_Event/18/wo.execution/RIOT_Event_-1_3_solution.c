#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <time_units.h>
#include <stddef.h>
#include <stdio.h>
#include <msg.h>
#include <mbox.h>
#include <ztimer.h>
#include <event/timeout.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <event/callback.h>
#include <MQTTClient.h>

void periodic_callback(void *arg) {
    (void)arg; // suppress unused parameter warning
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize ztimer
    ztimer_t timer;

    // Set up the callback
    ztimer_set_cb(ZTIMER_MSEC, &timer, periodic_callback, NULL);

    // Start the timer to trigger every second (1000 ms)
    ztimer_periodic_start(ZTIMER_MSEC, &timer, 1000);

    // Keep the program running to allow the callback to be triggered
    while (1) {
        sleep(10); // Sleep to reduce CPU usage
    }

    return 0;
}