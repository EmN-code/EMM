#include <xtimer.h>  // The xtimer library provides high-level timer utilities.
#include <ztimer.h>  // The ztimer library is utilized for precision timing operations.
#include <timex.h>   // For time conversion operations.
#include <evtimer_msg.h> // To handle event timer messages within the application.
#include <evtimer_mbox.h> // Provides support for event timer mbox facilities.
#include <mbox.h> // This is used to handle mailbox communication.
#include <msg.h>  // For message passing between threads and interrupts.
#include <unistd.h>  // Standard symbolic constants and types, especially for sleep function.
#include <time_units.h>  // Time unit conversions and constants.

#define INTERVAL_USEC (1U * US_PER_SEC)  // Interval for timer: 1 second.

static evtimer_t evtimer;
static evtimer_mbox_event_t evtimer_event;
static mbox_t mbox;
static msg_t mbox_queue[8];  // Queue for handling messages, queue size is a power of 2.

// Event handler function, executed every timer interval.
void event_handler(void *arg) {
    (void)arg;
    printf("Event triggered!\n");  // Print event triggered message.
}

int main(void) {
    mbox_init(&mbox, mbox_queue, 8);  // Initialize the mbox with message queue.

    evtimer_init_mbox(&evtimer);  // Initialize the event timer.
    evtimer_mbox_event_init(&evtimer_event, &mbox, event_handler, NULL); // Initialize mbox event.
    evtimer_add_mbox(&evtimer, &evtimer_event, &mbox, INTERVAL_USEC);  // Schedule event on the timer.

    while(1) {
        msg_t msg;
        msg_receive(&msg);  // Block until a message is received.
        // Handle message, for now, we do nothing here.
    }

    return 0;
}