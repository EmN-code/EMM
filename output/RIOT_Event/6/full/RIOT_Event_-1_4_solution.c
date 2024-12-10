#include <xtimer.h>
#include <stdbool.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <stddef.h>
#include <ztimer.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <time_units.h>
#include <event/periodic.h>
#include <thread.h>
#include <timex.h>
#include <periph/rtc.h>

#define INTERVAL (1U * US_PER_SEC) /* 1 second interval */

static void _periodic_callback(void *arg) {
    printf("Periodic Callback Triggered\n");
}

int main(void) {
    printf("Starting Periodic Timer Example\n");

    /* Initialize a periodic timer */
    evtimer_t periodic_timer;
    evtimer_msg_init(&periodic_timer);

    /* Initialize the event periodic instance */
    event_periodic_t event;
    event_periodic_init(&event, &periodic_timer, _periodic_callback, NULL);

    /* Start the periodic event with the specified interval */
    event_periodic_start(&event, INTERVAL);

    while (true) {
        /* The main loop can perform other tasks or simply idle */
        xtimer_sleep(1);
    }

    return 0;
}