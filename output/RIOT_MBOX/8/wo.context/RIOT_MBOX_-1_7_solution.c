#include <stdlib.h>
#include "ztimer.h"
#include "msg.h"
#include "kernel/types.h"

// Define a message queue and a thread stack
#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

// Thread stack for the message handler
#define THREAD_STACKSIZE    (THREAD_STACKSIZE_MAIN)
static char thread_stack[THREAD_STACKSIZE];

// Event callback function for periodic timer
static void evtimer_callback(void *arg) {
    (void)arg;
    puts("Event triggered!");
}

// Main function
int main(void) {
    // Initialize the main message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize a periodic timer
    ztimer_t timer;
    ztimer_init(&timer, ZTIMER_MSEC, evtimer_callback, NULL);
    ztimer_set(&timer, 1000);

    // Main loop
    while (1) {
        // Wait for messages
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == 0x1234) {
            puts("Message received!");
        }
    }

    return 0;
}