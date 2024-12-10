
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
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define LED_PIN 5  // Assuming GPIO5 is connected to the LED

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set the timer to trigger every second
    xtimer_set_periodic(&timer, 1 * US_PER_SEC, timer_callback, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1 * US_PER_SEC);
    }

    return 0;
}
