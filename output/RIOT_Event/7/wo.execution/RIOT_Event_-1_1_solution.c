#include <stdio.h> // For printf
#include <kernel_types.h> // For kernel functionalities
#include <event/periodic.h> // For setting up periodic events

// Function prototype for the callback
void my_periodic_callback(void *arg);

// Define the callback function
void my_periodic_callback(void *arg){
    // Print a message every time the callback is invoked
    printf("Periodic callback function triggered.\n");
}

// Main function
int main(){
    // Define the periodic event structure
    evtimer_periodic_event_t periodic_event;

    // Set up the periodic event
    periodic_event.callback = my_periodic_callback;
    periodic_event.interval = 1 * US_PER_SEC; // Event invoked every second
    periodic_event.arg = NULL;

    // Initialize and start the periodic event
    evtimer_periodic_start(&periodic_event);

    // Run an infinite loop to keep the program running
    while(1){
        // You might want to perform other tasks or go to sleep
    }

    // In RIOT, the program never reaches this point, since it continues running indefinitely
    return 0;
}