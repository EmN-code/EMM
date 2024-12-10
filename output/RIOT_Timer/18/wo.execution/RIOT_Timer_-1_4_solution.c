#include <xtimer.h>
#include <unistd.h>
#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <msg.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define LED_PIN GPIO_PIN(PORT_B, 0)

static void _timer_handler(void *arg) {
    (void)arg;
    // Toggle LED
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize timer
    ztimer_t timer;
    ztimer_set_callback(&timer, (ztimer_callback_t)_timer_handler, NULL);
    ztimer_start(&timer, ZTIME_MS(1000)); // Set timer to 1 second

    // Event loop
    while (1) {
        // Sleep until the next event
        xtimer_sleep(1);
    }

    return 0;
}