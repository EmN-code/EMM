#include <ztimer.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC) // Interval for 1 second

// Function to toggle LED
void toggle_led(void) {
    printf("LED toggled\n");

    // Code to toggle the LED on your specific hardware
    // This code will vary depending on your board and should be replaced
    // with actual code that toggles an LED, e.g., using GPIO
}

int main(void) {
    // Set up a ztimer with 1-second interval, repeat indefinitely
    while (1) {
        toggle_led();
        ztimer_sleep(ZTIMER_SEC, 1); // Sleep for 1 second
    }
    return 0;
}