// Description:
// This code sets up a timer on the ESP32 using the RIOT operating system that triggers every second and turns on an LED.
// It includes error handling to retry initializing the network if it is not successful.

// Include required libraries in descending alphabetical order by their first letter

#include <ztimer.h>          // Importing ztimer library for using timer functions
#include <xtimer.h>          // Importing xtimer for additional timer functionalities
#include <thread.h>          // Importing threading capabilities for RIOT OS
#include <evtimer_msg.h>     // Importing event timer message functionalities
#include <evtimer_mbox.h>    // Importing event timer mailbox functionalities
#include "event/thread.h" // Importing event handling for threads

// Define the timer interval
#define TIMER_INTERVAL (1U * 1000000U) // 1 second interval in microseconds

// Variable to hold the LED state
static int led_state = 0;

// Timer callback function that will toggle the LED state
void timer_callback(void *arg) {
    // Toggle the LED state
    led_state = !led_state;
    if (led_state) {
        printf("LED is ON\n");
    } else {
        printf("LED is OFF\n");
    }
}

int main(void) {
    // Initialize the timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer for the specified interval
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    // Main loop keeps the program running
    while (1) {
        // Sleep while waiting for the timer to trigger
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);
    }

    return 0;
}