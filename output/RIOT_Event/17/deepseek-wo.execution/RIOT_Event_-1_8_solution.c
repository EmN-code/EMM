
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds

void event_periodic_callback(void) {
    printf("Event callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    msg_t msg;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        msg_receive(&msg);
        event_periodic_callback();
    }

    return 0;
}
