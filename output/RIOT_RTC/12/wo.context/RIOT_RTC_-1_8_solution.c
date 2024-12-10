#include <xtimer.h>  // Required for timer functions
#include <stdio.h>  // Required for printf function and standard I/O operations
#include <stdlib.h>  // Required for using standard utility functions

#define ALARM_DURATION (5U * US_PER_SEC) // Alarm duration of 5 seconds

static void alarm_callback(void *arg) {
    (void) arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Setting RTC alarm...\n");

    xtimer_t timer;
    xtimer_set_msg(&timer, ALARM_DURATION, thread_getpid(), NULL);

    while (1) {
        // Simulate blocking wait
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}