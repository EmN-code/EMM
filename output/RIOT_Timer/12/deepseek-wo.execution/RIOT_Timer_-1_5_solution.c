
#include <stdlib.h>
#include <thread.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static xtimer_t timer;
static bool led_state = false;

void timer_callback(void *arg) {
    (void)arg;
    led_state = !led_state;
    // Assuming there is a function to toggle the LED state
    // led_toggle(LED_PIN);
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop to keep the thread alive
    while (1) {
        // Yield to allow other threads to run
        thread_yield();
    }

    return 0;
}
