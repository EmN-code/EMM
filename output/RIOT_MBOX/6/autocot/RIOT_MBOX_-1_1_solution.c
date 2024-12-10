#include <ztimer.h>
#include <evtimer.h>
#include <msg.h>
#include <stdio.h>

#define INTERVAL_USEC (1000000U) // 1 second
#define MAIN_QUEUE_SIZE (8)

static char _main_msg_stack[THREAD_STACKSIZE_MAIN];
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

// Event timer callback function
void on_event(evqueue_event_t *event) {
    // Handle the event
    puts("Event triggered!");
}

// Main function
int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Set up evtimer
    evtimer_t evtimer;
    evtimer_event_t events[1];

    evtimer_init(&evtimer);
    evtimer_set(&evtimer, &events[0], INTERVAL_USEC, &on_event);
    evtimer_add_event(&evtimer, &events[0]);

    // Start the event loop
    while (1) {
        // Wait for the next message or event
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}