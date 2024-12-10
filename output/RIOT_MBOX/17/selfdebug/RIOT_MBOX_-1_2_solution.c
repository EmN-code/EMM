#include <xtimer.h>  // Include xtimer header for managing timers
#include <stdio.h>   // Include standard input/output library for functions like printf
#include <evtimer.h>  // Include evtimer header for event timer utilities

// Define the event handler function for evtimer
void evtimer_event_handler(evtimer_event_t *event, void *arg) {
    // Function logic to handle evtimer events
    printf("Event triggered!\n");
}

// Main function definition
int main(void) {
    // Initialize a message box and an event timer
    mbox_t mbox;
    msg_t mbox_queue[8];  // Define a queue with a size that is a power of two
    mbox_init(&mbox, mbox_queue, 8);  // Initialize the mailbox

    evtimer_t evtimer;  // Define the event timer
    evtimer_init(&evtimer, evtimer_event_handler);  // Initialize the event timer successfully

    evtimer_event_t evtimer_event = {  // Define and initialize an event structure
        .msg = { .type = 1 },  // Message type
        .period = 5000000  // Set period of 5 seconds in microseconds
    };

    // Add the event to the timer using mailbox for events
    evtimer_add(&evtimer, &evtimer_event);

    while (1) {
        msg_t m;
        // Blocking receive call to fetch messages from the mailbox
        if (msg_receive(&m) == 1) {
            printf("Received message of type: %d\n", m.type);
        }
    }

    return 0;  // Return zero on successful conclusion
}