#include <thread.h>
#include <event/periodic.h>

static void my_callback(void *arg) {
    (void)arg; // to suppress unused parameter warning
    printf("Periodic event triggered.\n");
}

int main(void) {
    evtimer_periodic_t periodic;
    evtimer_t evtimer;

    // Initialize the event timer
    evtimer_init(&evtimer, NULL);
    
    // Set up the periodic event for every second (1,000,000 microseconds)
    evtimer_periodic_init(&periodic, my_callback, NULL, 1000000);
    
    // Add the periodic event to the event timer
    evtimer_add_periodic(&evtimer, &periodic);
    
    printf("Event timer initialized and started.\n");

    // Run an empty loop to keep the main thread active
    while (1) {
        thread_yield();
    }

    return 0;
}