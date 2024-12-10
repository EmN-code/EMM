
#include <stdio.h>
#include "event.h"  // Include the event header file
#include "xtimer.h" // Include xtimer for timing functionality

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static void periodic_callback(void *arg) {
    (void)arg; // Suppress unused parameter warning
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_queue_t queue;
    char stack[STACKSIZE];

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the event thread
    event_thread_init(&queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    // Create a periodic event
    xtimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, 1000000); // 1 second in microseconds

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
