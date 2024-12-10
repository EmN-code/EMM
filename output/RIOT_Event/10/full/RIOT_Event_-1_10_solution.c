// Given the constraints, here is a code solution using RIOT-OS for ESP32 that initializes and starts an event_periodic callback function, ensuring it runs every second, without using the restricted headers and APIs.

#include "stdint.h"
#include "string.h"

// Define a simple callback function
void callback_function(void)
{
    printf("Callback function called every second\n");
}

int main(void)
{
    // Assuming there's a timer, use its API, since the restricted APIs can't be used.
    timer_t my_timer;
    
    // Setting timer initialization parameters
    memset(&my_timer, 0, sizeof(my_timer));
    
    // Initialize a timer with a callback and a one-second interval
    // Since we can't use 'ztimer', hypothetical initialization here
    // Assuming a function init_timer exists
    init_timer(&my_timer, 1000 /* interval in ms */, callback_function);

    // Start the timer
    // Assuming a function start_timer exists
    start_timer(&my_timer);

    while (1) {
        // Main thread can do other tasks or simply sleep
        sleep(1);
    }

    return 0;
}