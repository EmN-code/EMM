
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

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Timer triggered!\n");
    // Assuming LED is connected to GPIO pin 5
    gpio_toggle(GPIO_PIN(0, 5));
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(GPIO_PIN(0, 5), GPIO_OUT);

    // Set up the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
