#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <event/periodic.h>
#include <event/callback.h>

void callback(void *arg) {
    printf("Callback triggered every second\n");
}

int main(void) {
    // Define the periodic event and callback event
    event_periodic_t periodic_event;
    event_callback_t callback_event;

    // Initialize the callback event with the handler and an optional argument
    event_callback_init(&callback_event, callback, NULL);

    // Initialize the periodic event every second (1000 ms)
    // Ensure the proper initialization by passing &callback_event as the third argument
    if (event_periodic_init(&periodic_event, 1000U, &callback_event) != 0) {
        printf("Failed to initialize periodic event\n");
        return EXIT_FAILURE;
    }

    // Start the periodic event
    // Correct function call with all necessary parameters
    if (event_periodic_start(&periodic_event) != 0) {
        printf("Failed to start periodic event\n");
        return EXIT_FAILURE;
    }

    puts("Event periodic started successfully\n");

    return EXIT_SUCCESS;
}