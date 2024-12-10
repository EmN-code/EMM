#include "ztimer.h"
#include "stdio.h"
#include "thread.h"
#include "evtimer.h"

#define INTERVAL_SEC   (1U)

static char stack[THREAD_STACKSIZE_MAIN];
static evtimer_t event_timer;
static evtimer_msg_event_t timer_event;

void callback(void *arg) {
    (void)arg;  // unused
    puts("Timer triggered!");
}

int main(void) {
    // Initialize event timer for handling with messages
    evtimer_init_msg(&event_timer);
    
    // Create the message queue for the current thread
    // Use 8 as the number of messages since it's a power of 2
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    // Setup the event
    timer_event.msg.type = 0;
    timer_event.event.period = INTERVAL_SEC * 1000000U; // convert to microseconds
    timer_event.event.callback = callback;
    
    // Add event to event timer
    evtimer_add_msg(&event_timer, &timer_event, thread_getpid());

    // Start the event timer
    evtimer_set(&event_timer, &timer_event.event);

    puts("Infinite loop to keep the program running...");

    // Keep the program running
    while (1) {
        evtimer_sleep(&event_timer);
    }

    return 0;
}