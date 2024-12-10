#include <thread.h>
#include <event/periodic.h>

void my_callback(void *arg) {
    // Code to execute every second
    printf("Callback executed\n");
}

int main(void) {
    evtimer_t my_timer;
    evtimer_periodic_event_t my_event;

    // Initialize the periodic event
    evtimer_periodic_init(&my_timer, &my_event, my_callback, NULL, 1);

    // Start the timer
    evtimer_periodic_start(&my_event);

    while (1) {
        // Main loop can perform other tasks, but ensure periodic callback is maintained
        xtimer_usleep(1000000); // Sleep to simulate workload
    }
    return 0;
}