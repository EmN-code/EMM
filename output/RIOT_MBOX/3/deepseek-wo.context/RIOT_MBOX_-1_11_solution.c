
#include <stdio.h>
#include "event/periodic.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL (1000000U)  // 1 second interval

static void callback(void *arg) {
    printf("Event triggered with argument: %s\n", (char *)arg);
}

int main(void) {
    event_periodic_callback_t event;
    ztimer_clock_t *clock = ZTIMER_USEC;
    event_queue_t queue;
    char stack[THREAD_STACKSIZE_DEFAULT];
    char arg[] = "Hello, RIOT!";

    // Initialize the event queue
    event_thread_init(&queue, stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1);

    // Initialize and start the periodic callback event
    event_periodic_callback_create(&event, clock, INTERVAL, &queue, callback, arg);

    // Keep the main thread running
    while (1) {
        thread_yield();
    }

    return 0;
}
