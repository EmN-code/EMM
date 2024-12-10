#include <timex.h>
#include <tim.h>
#include <stdio.h>
#include <event/periodic.h>

void callback(void *arg) {
    printf("Callback function executed.\n");
}

int main(void) {
    event_periodic_t periodic_event;
    timex_t interval = {1, 0};  // 1 second interval
    timex_t time_now;
    
    // Initialize the periodic event
    if (event_periodic_init(&periodic_event, &interval, callback, NULL) != 0) {
        printf("Failed to initialize periodic event.\n");
        return 1;
    }

    // Get the current time
    if (xtimer_now_timex(&time_now) != 0) {
        printf("Failed to get current time.\n");
        return 1;
    }

    // Start the periodic event
    if (event_periodic_start(&periodic_event, time_now) != 0) {
        printf("Failed to start periodic event.\n");
        return 1;
    }

    // Main loop
    while (1) {
        // The callback function will be invoked every second
    }

    return 0;
}