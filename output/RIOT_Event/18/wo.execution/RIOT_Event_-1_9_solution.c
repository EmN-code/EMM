#include <xtimer.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <stdio.h>

// Define a period of one second (in microseconds)
#define PERIOD (1000000U)

// Placeholder callback function to be triggered
void callback(void *arg) {
    puts("Callback function called");
}

int main(void) {
    // Create and configure a message queue
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    // Define and initialize the timer event
    evtimer_t timer;
    evtimer_mbox_event_t event;

    evtimer_init_mbox(&timer);

    // Start periodic execution of the callback event every second
    while (1) {
        // Add the event to the event timer
        evtimer_add_mbox(&timer, &event, msg_queue);
        msg_receive(NULL);  // Wait for the event
        callback(NULL);     // Call the callback if needed
    }

    return 0;
}