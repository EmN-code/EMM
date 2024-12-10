#include "event/periodic.h"
#include "event/callback.h"
#include "esp_partition.h"
#include "riot_version.h"
#include "net/sock/util.h"
#include <stdio.h>

// Callback function that will be called every second
void event_periodic_callback(void *arg) {
    printf("Event periodic callback triggered\n");
}

int main() {
    // Initialize the callback context for periodic events
    event_callback_t callback;
    callback.callback = event_periodic_callback;
    callback.arg = NULL;

    // Initialize the periodic event structure
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, &callback, 1000);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop
    while(1) {
        // Busy loop or perform other tasks
    }

    return 0;
}