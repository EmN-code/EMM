#include <ztimer.h>
#include <stdio.h>

#define INTERVAL (1U * ZTIMER_SEC)  // Set the interval to 1 second

void event_periodic(void* arg) {
    printf("Periodic event executed.\n");
}

int main(void) {
    ztimer_t timer;
    ztimer_set_msg(&timer, INTERVAL, thread_getpid(), NULL);  // Initialize the timer

    while (1) {
        ztimer_sleep(ZTIMER_SEC);  // Sleep for 1 second
        event_periodic(NULL);      // Call the periodic event
    }

    return 0;
}