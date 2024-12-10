#include <timers/timer.h>
#include <stdio.h>
#include <event/periodic.h>

void my_callback(void *arg) {
    printf("Callback triggered!\n");
}

int main(void) {
    // Define an event_periodic_t structure
    event_periodic_t my_periodic_event;

    // Initialize the periodic event
    event_periodic_init(&my_periodic_event, my_callback, NULL);

    // Start the periodic event every second
    event_periodic_start(&my_periodic_event, 1);

    // Main loop
    while (1) {
        // Sleep or perform other tasks
    }

    return 0;
}