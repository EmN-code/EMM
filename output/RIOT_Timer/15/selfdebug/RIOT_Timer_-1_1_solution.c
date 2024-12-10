#include <timers.h> // For handling timer operations
#include <stdio.h> // For standard I/O operations
#include <led.h> // For LED operations on the ESP32

#define TIMER_DEV 0 // Assume the timer device to be used is 0

void timer_callback(void *arg) {
    // Turning on the LED, assuming there's a function LED_ON in led.h
    LED_ON();
}

int main(void) {
    // Initialize the timer
    if (timer_init(TIMER_DEV, 1, timer_callback, NULL) < 0) {
        printf("Error initializing timer\n");
        return 1;
    }
    
    // Start the timer
    timer_start(TIMER_DEV);

    // Keep the application running
    while (1) {
        // Sleep or perform other tasks
    }
    return 0;
}