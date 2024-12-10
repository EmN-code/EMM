
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <kernel_defines.h>
#include <dht_params.h>
#include <mma8x5x_params.h>
#include <timex.h>
#include <time_units.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static volatile bool timer_triggered = false;

static void timer_callback(void *arg) {
    (void)arg;
    timer_triggered = true;
}

int main(void) {
    // Initialize the event timer
    evtimer_t et;
    evtimer_init(&et);

    // Set up the periodic timer
    evtimer_add_msg(&et, TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        if (timer_triggered) {
            printf("Timer triggered!\n");
            timer_triggered = false;
            evtimer_add_msg(&et, TIMER_INTERVAL, timer_callback, NULL);
        }
        // Other tasks can be performed here
    }

    return 0;
}
