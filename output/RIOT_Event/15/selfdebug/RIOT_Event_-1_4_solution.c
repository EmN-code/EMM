#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <event/thread.h>

void callback(void *arg) {
    printf("Timer callback executed\n");
}

int main(void) {
    // Define the timer
    ztimer_t timer;
    
    // Initialize the timer with a callback
    ztimer_init_ztimer(&timer, ZTIMER_MSEC, callback, NULL);

    // Set the timer to execute every second (1000 ms)
    xtimer_set(&timer, 1000 * ZTIMER_USEC_PER_MSEC);

    // Main loop to keep the program running
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}