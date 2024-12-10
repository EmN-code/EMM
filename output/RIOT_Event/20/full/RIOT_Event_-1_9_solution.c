#include "xtimer.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "stdio.h"
#include <stdlib.h>
#include <unistd.h>

void callback(void *arg) {
    (void)arg;
    printf("Callback triggered every second.\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    msg_t msg_queue[8];
    kernel_pid_t pid = thread_getpid();

    // Set up the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue)/sizeof(msg_t));

    // Initialize the event timer for message based events
    evtimer_init_msg(&evtimer);

    // Set up the event
    evtimer_msg_event_init(&event);
    evtimer_msg_event_set(&event, XTIMER_SEC(1), pid, callback, NULL);
    
    // Add the event to the timer
    evtimer_add_msg(&evtimer, &event, pid);

    // Main loop
    while (1) {
        msg_t msg;
        msg_receive(&msg); // Block until a message arrives
    }

    return 0;
}