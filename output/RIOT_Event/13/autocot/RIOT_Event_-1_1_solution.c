#include <thread.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <esp_partition.h>
#include <esp_idf_gpio.h>

// Define a function for the periodic callback
void periodic_callback(void *arg) {
    // Place your periodic task here, for example, toggle an LED or send a message
    printf("Periodic callback triggered\n");
}

int main(void) {
    // Define and initialize an event_periodic_t object
    event_periodic_t periodic_event;
    event_callback_t callback_event = {.cb = periodic_callback, .arg = NULL};

    // Set the periodic interval to 1 second
    unsigned int interval = 1;

    // Initialize the periodic event with the callback and interval
    event_periodic_init(&periodic_event, &callback_event, interval);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Normally, the main function would not return in an embedded application
    // For simulation purposes, we'll create an endless loop
    while (1) {
        thread_yield();
    }

    return 0;
}