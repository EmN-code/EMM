#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>

#define INTERVAL (1U * ZTIMER_USEC_PER_SEC)  // 1-second interval

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    ztimer_t timer;
    msg_t msg;
    msg.content.ptr = &timer;

    // Initialize the timer
    ztimer_init(ZTIMER_MSEC, &timer);

    // Set the timer with a proper message type and timer interval
    ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL, &msg, thread_getpid());

    printf("Timer initialized and started with 1 second interval.\n");

    // Endless loop
    while(1) {
        // Main thread can perform other tasks
        msg_receive(&msg);

        // Reset the timer for periodic callback
        ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL, &msg, thread_getpid());
    }

    return 0;
}