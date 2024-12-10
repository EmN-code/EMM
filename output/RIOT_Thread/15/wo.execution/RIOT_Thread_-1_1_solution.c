#include <thread.h>
#include <msg.h>
#include <stdio.h>

// Define thread stack sizes and message queue size
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQ_SIZE   (8)  // Must be a power of two

// Declare stacks for each thread
static char thread_stack1[STACKSIZE];
static char thread_stack2[STACKSIZE];

// Declare message queue
static msg_t msg_queue[MQ_SIZE];

// Function for Thread 1 - Sending Messages
void *thread1(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    while (1) {
        // Send a message every 2 seconds
        msg_send(&msg, thread_getpid_of(&thread_stack2));
        printf("Thread 1: Sent message with value %d\n", msg.content.value);
        xtimer_sleep(2);
    }
    return NULL;
}

// Function for Thread 2 - Receiving Messages
void *thread2(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        // Block and wait to receive a message
        msg_receive(&msg);
        printf("Thread 2: Received message with value %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, MQ_SIZE);
    
    // Create threads
    thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");
    thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 2,
                  THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");
    
    // Let main thread sleep
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}