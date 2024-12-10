#include "xtimer.h"
#include "msg.h"
#include "kernel_defines.h"

int main(void) {
    // Define a message box and its queue
    mbox_t mbox;
    msg_t queue[8]; // Ensure the queue size is a power of two as required
    
    // Initialize the message box
    mbox_init(&mbox, queue, 8);
    
    // Set up an event timer to send messages
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Create and initialize evtimer event
    evtimer_msg_event_t event;
    event.msg.sender_pid = KERNEL_PID_UNDEF;  // Use undefined sender PID for simplicity
    
    // Define a function to be called back when the timer expires
    void event_callback(void *arg) {
        puts("Message box event triggered!");
    }

    // Set up a periodic timer for event
    ztimer_t timer;
    timer.callback = event_callback;
    timer.arg = NULL;
    
    // Define the interval period
    ztimer_set_wakeup(ztimer_periph_systimer, &timer, 1000000); // 1 second intervals
    
    // Run the event loop
    while (1) {
        msg_receive(&event.msg); // Blocks until a message is received and processed
    }
    
    return 0;
}