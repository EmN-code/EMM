#include <stdio.h>
#include "xtimer.h"
#include "msg.h"

#define INTERVAL (1U * US_PER_SEC) // Set an interval of 1 second
static msg_t message;
static msg_t queue[8]; // Define a message queue

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        // Prepare the message
        message.content.value = xtimer_now_usec();
        
        // Send the message to the current thread's queue
        msg_send(&message, thread_getpid());
        
        // Sleep for the defined interval
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(queue, 8);
    
    // Create a thread to handle message sending
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL, "msg_sender");
    
    while(1) {
        // Receive messages
        msg_t msg;
        msg_receive(&msg);
        
        // Print the received message content
        printf("Received message with timestamp: %ld\n", msg.content.value);
    }
    return 0;
}