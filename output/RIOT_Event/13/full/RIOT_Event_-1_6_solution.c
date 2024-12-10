// Import necessary header files
#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <stddef.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <event/callback.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <event/periodic.h>
#include <thread.h>
#include <timex.h>
#include <periph/rtc.h>

void periodic_event_callback(void *arg) {
    printf("Periodic event triggered\n");
}

int main(void) {
    // Timer setup
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Start a periodic event every second
    evtimer_periodic_t my_periodic_event;
    evtimer_periodic_init(&my_periodic_event, EVTIMER_PERIODIC_RELATIVE, periodic_event_callback, NULL, US_PER_SEC);
    evtimer_add(&evtimer, &my_periodic_event);

    // Wait to handle events
    while (1) {
        evtimer_handle_events(&evtimer);
    }

    return 0;
}