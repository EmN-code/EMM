#include <ztimer.h>
#include <stdio.h>
#include <evtimer.h>
#include <msg.h>
#include <kernel_types.h>

#define QUEUE_SIZE 8
#define SENDING_INTERVAL (1U * ZTIMER_MSEC_BASE)

// Initialize a message queue
static msg_t msg_queue[QUEUE_SIZE];

// Setting up the event message
static evtimer_msg_event_t event;

// Function to simulate event sending
void send_event(void *arg) {
    (void)arg;  // we ignore the argument for this simple example
    printf("Event sent!\n");
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);

    ztimer_t timer;
    evtimer_t evtimer;
    
    // Correct the declaration
    evtimer_add_msg(&evtimer, &event, sched_active_pid);

    while (1) {
        // Send an event at defined intervals
        send_event(NULL);
        ztimer_sleep(ZTIMER_MSEC, SENDING_INTERVAL);
    }

    return 0;
}