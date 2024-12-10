/* All necessary imports are included in alphabetical order */
#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

/* Event timer callback function */
void on_timer_event(void *arg) {
    (void)arg;  // suppress unused parameter warning
    printf("Periodic Event Triggered\n");
}

int main(void) {
    /* Define a timer */
    ztimer_periodic_t my_timer;

    /* Initialize the periodic timer with a period of 1 second (1,000,000 microseconds) */
    ztimer_periodic_init(ZTIMER_MSEC, &my_timer, 1000);

    while (1) {
        /* Start the periodic timer */
        ztimer_periodic_start(&my_timer);

        /* Sleeep to simulate waiting for the next tick */
        ztimer_sleep(ZTIMER_MSEC, 1 * US_PER_SEC);

        /* Call the callback function */
        on_timer_event(NULL);
    }

    return 0;
}