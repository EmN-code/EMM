#include <timex.h>
#include <event/periodic.h>
#include <event/callback.h>

void example_callback(void *arg) {
    // Callback function that will be triggered every second
    printf("Callback function triggered every second\n");
}

int main(void) {
    timex_t interval;
    event_callback_t callback;
    event_periodic_t periodic;

    // Set the interval to 1 second
    interval = timex_set(1, 0);

    // Initialize the callback event
    event_callback_init(&callback, example_callback, NULL);

    // Initialize the periodic event with the interval and the callback event
    event_periodic_init(&periodic, &interval, &callback);

    // Start the periodic event
    event_periodic_start(&periodic);

    // Enter a loop
    while(1) {
        // Can add additional code here
    }
    return 0;
}