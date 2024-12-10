#include <stdio.h>
#include <event/periodic.h>

void callback(void *arg) {
    printf("Callback function triggered.\n");
}

int main(void) {
    // Define the event periodic structure
    event_periodic_t event_periodic;

    // Initialize the event periodic structure with the callback function
    event_periodic_init(&event_periodic);

    // Configure the callback for the event periodic structure
    event_periodic_callback_set(&event_periodic, callback, NULL);

    // Start the periodic event with a 1-second interval
    uint32_t interval = 1000000; // 1 second in microseconds
    event_periodic_start(&event_periodic, interval);

    // Entering an infinite loop
    while(1) {
        ; // The program keeps running, firing the callback every second
    }

    return 0;
}