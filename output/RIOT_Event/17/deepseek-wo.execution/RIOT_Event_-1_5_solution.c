
#include <stdlib.h>
#include <msg.h>
#include <xtimer.h>
#include <thread.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <stddef.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL, event_periodic_callback, NULL);

    while (true) {
        // Receive a message (blocking call)
        msg_receive(&msg);
        // Yield to higher priority threads
        thread_yield();
    }

    return 0;
}
