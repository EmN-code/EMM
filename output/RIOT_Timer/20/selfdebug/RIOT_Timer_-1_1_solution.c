#include <thread_routines.h> // hypothetical header for thread related functions
#include <stdio.h> // for printf function
#include <timers.h> // hypothetical header for timers
#include <led_control.h> // hypothetical header for controlling LED

// Define the timer and LED constants
#define TIMER_DURATION 1 // Timer interval duration in seconds
#define LED_PIN 2 // Using pin 2 for the LED for demonstration

static void timer_callback(void *arg) {
    // Turn on the LED
    led_on(LED_PIN);
    printf("LED turned on\n");
}

int main(void) {
    printf("Starting RIOT application\n");

    // Initialize the LED
    led_init(LED_PIN);

    // Initialize and start the timer with a 1-second interval
    tim_t timer_dev = timer_create(TIMER_DURATION, timer_callback, NULL);
    if (timer_dev == -1) {
        printf("Failed to create timer\n");
        return -1;
    }
    timer_start(timer_dev);

    // Enter an infinite loop to keep the application running
    while (1) {
        thread_yield();
    }

    return 0;
}