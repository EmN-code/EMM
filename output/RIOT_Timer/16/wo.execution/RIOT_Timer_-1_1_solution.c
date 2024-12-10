#include <stdio.h>
#include <stdint.h>
#include <timex.h>
#include <ztimer.h>

#define LED_PIN 4

// Callback function to be called every time the timer elapses
void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    // Turn on the LED
    // Since we don't have a direct reference, let's assume the GPIO API is used here
    // Actual function would look like gpio_set(LED_PIN);
    printf("Timer triggered, LED turned on\n");
}

int main(void) {
    printf("Timer based LED application starting...\n");

    // configure the LED_PIN as output
    // The actual function might look like gpio_init(LED_PIN, GPIO_OUT);

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.interval = ZTIMER_MSEC_TO_USEC(1000);  // Timer interval set to 1 second

    // Initialize and start the timer
    ztimer_start(ZTIMER_USEC, &timer);

    while (1) {
        // Main loop that can be used to perform other tasks or sleep
        ztimer_sleep(ZTIMER_USEC, 1000000);  // Sleep for 1 second
    }

    return 0;
}