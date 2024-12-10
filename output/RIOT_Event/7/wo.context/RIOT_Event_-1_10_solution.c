#include <ztimer.h>  // Include the ztimer header for access to timer functions
#include <stdio.h>
#include <stdlib.h>

static void timer_callback(void *arg) {
    (void)arg; // To avoid unused parameter warning
    printf("Callback function executed\n");
}

int main(void) {
    ztimer_t timer; // Declare the timer correctly
    ztimer_init(ZTIMER_MSEC);
    ztimer_set_msg(&timer, 1000, timer_callback, NULL); // Set up a periodic timer with a 1000 ms (1 second) interval

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }

    return 0;
}