#include <stdio.h>
#include "msg.h"
#include "thread.h"

#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE         (THREAD_STACKSIZE_MAIN)

static char sender_thread_stack[THREAD_STACKSIZE];
static char receiver_thread_stack[THREAD_STACKSIZE];

static msg_t msg_queue[8];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message content
    while (1) {
        // Send message to thread with specific pid
        msg_send(&msg, thread_getpid_of(NULL));
        printf("[Sender] Sent message with value: %d\n", msg.content.value);
        thread_yield(); // Yield to allow the receiver to run
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        // Wait for message
        msg_receive(&msg);
        printf("[Receiver] Received message with value: %d\n", msg.content.value);
        thread_yield(); // Yield to allow the sender to run
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, 8);
    
    // Create threads
    thread_create(sender_thread_stack, sizeof(sender_thread_stack), 
                  THREAD_PRIORITY_SENDER, 0, sender, NULL, "sender thread");
    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), 
                  THREAD_PRIORITY_RECEIVER, 0, receiver, NULL, "receiver thread");
    
    return 0;
}