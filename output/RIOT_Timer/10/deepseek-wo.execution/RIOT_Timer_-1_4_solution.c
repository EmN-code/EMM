
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stddef.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static volatile bool led_state = false;

void timer_callback(void *arg)
{
    (void)arg;
    led_state = !led_state;
    printf("LED state: %s\n", led_state ? "ON" : "OFF");
}

int main(void)
{
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        // Main loop to keep the program running
        xtimer_usleep(100 * US_PER_SEC);  // Sleep for 100 seconds to keep the loop running
    }

    return 0;
}
