#include <timer.h>
#include <stdio.h>
#include "board.h"

#define TIMER_DEVICE (TIMER_DEV(0))
#define TIMER_FREQ   (1U)  // 1 Hz frequency for 1-second intervals
#define LED_PIN      (GPIO_PIN(PORT_C, 0)) // Define LED pin, assuming it's PORT C PIN 0

static void timer_callback(void *arg) {
    (void)arg;  // Unused
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    puts("LED toggled");
}

int main(void) {
    // Initialize the board
    board_init();
    
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer with the specified frequency and callback
    if (timer_init(TIMER_DEVICE, TIMER_FREQ, timer_callback, NULL) < 0) {
        puts("Error: unable to initialize timer");
        return -1;
    }
    
    // Start the timer
    timer_start(TIMER_DEVICE);
    puts("Timer started");

    while (1) {
        // Just sleep to save power.
        thread_sleep();
    }

    return 0;
}