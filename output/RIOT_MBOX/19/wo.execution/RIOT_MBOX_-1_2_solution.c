#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <string.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <stdio.h>

// Define the message queue size
#define QUEUE_SIZE 8

// Define a message queue
msg_t msg_queue[QUEUE_SIZE];

void *event_handler(void *arg) {
    // Simulating a simple event handler
    (void)arg; // Unused
    while (1) {
        msg_t msg;
        msg_receive(&msg); // Blocking receive for simplicity
        printf("Received message with type: %d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    puts("Message box example using RIOT!");
    
    // Initialize the message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Define a timer
    ztimer_t timer;
    
    // Initialize timer
    ztimer_set_msg(ZTIMER_MSEC, &timer, 1000, NULL, &msg_queue[0]);

    // Start the event handler
    event_handler(NULL);

    return 0;
}