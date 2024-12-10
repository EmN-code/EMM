#include <xtimer.h>  // Include the xtimer library for time-related functions
#include <stdio.h>   // Include standard input/output library
#include "msg.h"    // Include the message library

#define MSG_QUEUE_SIZE 8
 msg_t queue[MSG_QUEUE_SIZE];  // Message queue array
 static msg_t msg;

int main(void) {
    msg_init_queue(queue, MSG_QUEUE_SIZE);  // Initialize message queue
    
    while (1) {
        // Simulate sending a message
        puts("Sending message...");
        msg_send(&msg, KERNEL_PID_COOP);  // Send message to cooperative thread (or adjust PID)

        // Wait for 1 second
        xtimer_sleep(1);
    }
    return 0;
}