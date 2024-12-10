#include <xtimer.h>  // for timers
#include <stdio.h>   // for standard I/O operations
#include <stdbool.h> // for boolean types
#include <msg.h>     // for message handling in RIOT
#include <ztimer.h>  // for ztimer functions
#include <kernel_defines.h> // for kernel definitions
#include <stdlib.h>  // for standard library functions
#include <time_units.h> // for time unit macros
#include <timex.h>   // for ztimer types
#include <evtimer_mbox.h> // for evtimer and mbox handling
#include <evtimer_msg.h>  // for evtimer message handling
#include <arpa/inet.h> // for network functionalities
#include <stddef.h>    // for standard definitions
#include <unistd.h>    // for standard symbolic constants
#include <netinet/in.h> // for network structures
#include <stdatomic.h> // for atomic operations

// LED GPIO pin (example GPIO_PIN(PORT, PIN) macro)
#define LED_PIN GPIO_PIN(0, 2)

// Timer interval in microseconds (1 second)
#define TIMER_INTERVAL (1U * US_PER_SEC)

// Timer structure
static ztimer_t my_timer;

// Function to toggle LED state
void toggle_led(void) {
    gpio_set(LED_PIN);
}

// Callback function for timer
void timer_callback(void *arg) {
    (void)arg;
    toggle_led();
    // Reschedule the timer
    ztimer_set(ZTIMER_MSEC, &my_timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer to call `timer_callback` every second
    my_timer.callback = timer_callback;
    my_timer.arg = NULL;

    // Schedule the first timer event
    ztimer_set(ZTIMER_MSEC, &my_timer, TIMER_INTERVAL);

    // Start the scheduler
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    // Main loop (optional if other operations are needed)
    while (1) {
        // Implement additional functionality here
        usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}