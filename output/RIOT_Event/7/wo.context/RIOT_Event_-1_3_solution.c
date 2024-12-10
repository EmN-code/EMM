#include <thread.h>
#include <sys/types.h>
#include <stdio.h>
#include <event.h>
#include <stdbool.h>
#include <stdlib.h>

// Define an event and a periodic timer
static event_t periodic_event;

// Function to be called every second
void periodic_event_callback(void *arg) {
    printf("Periodic event occurred\n");
}

int main(void) {
    puts("RIOT event_periodic example starting...");

    // Initialize the periodic event
    periodic_event.handler = periodic_event_callback;
    periodic_event.arg = NULL;

    // Start periodic event every 1 second
    event_periodic_callback_init(&periodic_event, 1 * US_PER_SEC);

    // Main loop
    while (true) {
        // Let the thread sleep to simulate asynchronous behavior
        thread_sleep();
    }

    return 0;
}