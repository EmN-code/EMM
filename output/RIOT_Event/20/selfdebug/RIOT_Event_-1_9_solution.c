#include <ztimer.h> // Include the ztimer header file for timer functionality
#include <stdio.h>    // Include standard input and output
#include <event.h>   // Include necessary event handling functionalities of RIOT

// Define the desired millisecond tick rate for 1 second
#define TIMER_TICK_RATE 1000

void event_callback(void *arg) {
    // Cast the argument and use it as needed or print a simple statement indicating timer callback
    (void)arg;
    printf("Event triggered every second!\n");
}

int main(void) {
    // Initialize ztimer
    ztimer_t timer;

    // Set the timer's parameters including its interval and callback function
    ztimer_set(&timer, ZTIMER_MSEC, TIMER_TICK_RATE * 1, event_callback, NULL);

    // This will repeatedly call the event_callback function every second
    ztimer_start(&timer);
    
    // Keep the main thread active
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_TICK_RATE);
    }
    return 0;
}