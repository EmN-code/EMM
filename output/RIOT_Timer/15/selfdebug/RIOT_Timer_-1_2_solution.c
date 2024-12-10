#include <timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <led.h>

#define TIMER_FREQUENCY 1

void timer_callback(void *arg) {
    (void)arg;
    LED_ON();
}

int main(void) {
    puts("RIOT Timer Example: ESP32 turning on LED every second");

    // Assuming a generic LED initialization
    if (LED_COUNT > 0) {
        printf("Using %d available LEDs\n", LED_COUNT);
    }

    // Initialize timer
    if (timer_init(TIMER_DEV(0), TIMER_FREQUENCY, timer_callback, NULL) < 0) {
        puts("Timer initialization failed!");
        return EXIT_FAILURE;
    }

    // Start timer
    if (timer_start(TIMER_DEV(0)) < 0) {
        puts("Failed to start timer!");
        return EXIT_FAILURE;
    }

    // Main loop does nothing, timer_callback will handle the LED
    while (1) {
        // Optional: sleep or delay to save power
    }

    return 0;
}