
#include <stdlib.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define LED_PIN 5  // Assuming LED is connected to GPIO 5 on ESP32

static void led_toggle(void) {
    // Dummy function to simulate LED toggle
    printf("LED toggled\n");
}

static void timer_callback(void *arg) {
    (void)arg;
    led_toggle();
}

int main(void) {
    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_set_periodic(&timer, 1 * US_PER_SEC, timer_callback, NULL);

    while (1) {
        // Main loop
    }

    return 0;
}
