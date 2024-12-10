#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "mutex.h"
#include "msg.h"

/*
In RIOT, `xtimer` is commonly used for managing time. However, given that it’s blacklisted in this context,
we'll need to use evtimer-related functions.

The evtimer functions are likely for managing events based on time but implemented differently from xtimer.
This code snippet will demonstrate initializing an event timer and leveraging it to execute a function periodically
without using xtimer.
*/

// Event Timer structure
static evtimer_t my_timer;

// Function to be called periodically
void callback_function(void *arg)
{
    (void) arg;
    printf("Callback function executed every second.\n");
}

// Initialize and start event timer
void initialize_event_periodic(void)
{
    // Assuming evtimer_init and evtimer_add are relevant functions from API.
    evtimer_init_msg(&my_timer);

    static evtimer_msg_event_t event;
    event.event.callback = callback_function;
    event.event.offset = 1*US_PER_SEC;  // Assume US_PER_SEC is defined suitably to express 1 second

    evtimer_add_msg(&my_timer, &event, thread_getpid());
}

int main(void)
{
    printf("Starting event periodic timer example...\n");
    initialize_event_periodic();

    // Keep the program running to allow periodic callback execution
    // In a real RIOT application, the main loop will likely do useful work.
    while (1) {
        // Simulate doing other work in main
        usleep(1000000); // Sleep for 1 second to avoid busy waiting
    }

    return 0;
}