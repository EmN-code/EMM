// Required header files in descending alphabetical order
#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <event.h>

#define INTERVAL (1U * US_PER_SEC) // Set the interval to 1 second

enum { THREAD_STACKSIZE_MAIN = THREAD_STACKSIZE_DEFAULT }; // Define default stack size

static evtimer_t evtimer; // Declare the evtimer
static msg_t msg_queue[8]; // Message queue for receiving events

void event_callback(void *arg) {
    (void)arg;
    printf("Event received!\n");
}

int main(void) {
    msg_init_queue(msg_queue, 8); // Initialize the message queue

    evtimer_event_t event; // Correct structure

    xtimer_ticks32_t interval = { .ticks32 = INTERVAL }; // Timer interval

    // Initialize the evtimer and handle with message callback
    evtimer_init(&evtimer, event_callback);

    // Set evtimer to trigger at INTERVAL
    evtimer_set(&evtimer, &event, interval, EVTIMER_ONESHOT); // Use existing functionalities

    while (1) {
        msg_t msg;

        // Block and wait for an event to occur
        msg_receive(&msg);
        evtimer_add(&evtimer, &event);
    }
    return 0;
}