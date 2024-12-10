#include "timex.h"
#include "timer.h"
#include "thread.h"

#include <stdio.h>

#define LED_PIN 0  // Assume pin 0 is connected to the LED

static void _timer_callback(void *arg) {
    (void)arg;
    // Code to turn on LED
    // This is placeholder logic since we don't have access to specific GPIO functions
    printf("LED turned on\n");
}

int main(void) {
    // Initialize a timer
    timer_t timer;
    timex_t interval = timex_set(1, 0); // 1 second interval

    timer_init(&timer, interval, _timer_callback, NULL);  
    timer_start(&timer);

    while (1) {
        // You'd typically sleep or do other tasks here
        thread_yield();
    }

    return 0;
}