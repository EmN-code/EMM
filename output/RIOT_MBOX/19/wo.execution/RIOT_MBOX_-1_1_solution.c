#include <ztimer.h> // Required for timer functionality
#include <stdio.h>  // Standard input/output definitions
#include <string.h> // Required for memset

#define INTERVAL (1U * ZTIMER_SEC) // define interval for timer in seconds

static msg_t msg_queue[8]; //define a message queue

// define an event structure that can hold messages
etimer_event_t event;

// Event handler callback
void evtimer_handler(evtimer_event_t *event) {
    printf("Event triggered with message: %d\n", event->msg.content.value);
}

int main(void) {
    // Initialize message
    msg_t msg;
    msg.type = 0;
    msg.content.value = 42;

    // Initialize evtimer_t and set callback
    evtimer_t evtimer;
    memset(&evtimer, 0, sizeof(etimer_t));
    evtimer.callback = evtimer_handler;
    evtimer.events = &event;

    // Create a repeating timer
    ztimer_t timer;
    ztimer_set_msg(ZTIMER_SEC, &timer, INTERVAL, &msg_send_receive, msg); 

    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }

    return 0;
}