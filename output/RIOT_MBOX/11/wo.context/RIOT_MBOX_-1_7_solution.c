#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "kernel.h"
#include "msg.h"
#include "evtimer.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define TIMER_INTERVAL (1U * US_PER_SEC)

static evtimer_t event_timer;
static msg_t msg_q[4]; // Assuming a fixed message queue size

void callback(void *arg) {
    (void)arg;
    printf("Event timer callback executed\n");
}

int main(void) {
    puts("RIOT example for ESP32 with message box and event timer\n");

    // Initialize the message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize event timer with the callback
    evtimer_init(&event_timer, callback);

    // Setting up a periodic timer
    evtimer_msg_event_t event;
    evtimer_add_msg(&event_timer, &event, getpid());
    evtimer_add_timeout(&event_timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);  // Receive and process messages
        printf("Message received from PID: %d\n", msg.sender_pid);
    }

    return 0;
}