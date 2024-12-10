
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

static xtimer_t timer;
static volatile bool led_state = false;

void timer_callback(void *arg) {
    (void)arg;
    led_state = !led_state;  // Toggle LED state
    printf("LED state: %s\n", led_state ? "ON" : "OFF");
    xtimer_set(&timer, TIMER_INTERVAL);  // Reset the timer
}

int main(void) {
    xtimer_init();  // Initialize xtimer
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);  // Set the timer with callback

    while (1) {
        // Main loop
    }

    return 0;
}
