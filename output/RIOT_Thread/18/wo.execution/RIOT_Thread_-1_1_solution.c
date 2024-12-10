#include "thread.h"
#include "msg.h"
#include <stdio.h>

// Stack sizes for the threads
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

// Message queue size
#define MSG_QUEUE_SIZE           (8)

// Declare the two threads, and declare their stacks
static char stack_thread_one[THREAD_STACKSIZE_MAIN];
static char stack_thread_two[THREAD_STACKSIZE_MAIN];

// Define the priority of the two threads
static int prio_thread_one = THREAD_PRIORITY_MAIN - 1;
static int prio_thread_two = THREAD_PRIORITY_MAIN - 2;

// Message queue
msg_t msg_queue[MSG_QUEUE_SIZE];

// Function for thread_two
void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;
    printf("Thread two is running and waiting for messages...\n");

    while (1) {
        // Wait for the message
        msg_receive(&msg);
        printf("Thread two received message: %ld\n", (intptr_t)msg.content.value);
    }
    return NULL;
}

// Function for thread_one
void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0xABCD; // Arbitrary message type
    msg.content.value = (intptr_t)42; // Arbitrary message content

    printf("Thread one is running and sending a message...\n");

    for (int i = 0; i < MSG_QUEUE_SIZE; i++) {
        // Send a message to the main thread
        if (msg_send(&msg, thread_getpid() + 1) == 1) {
            printf("Thread one sent message %d\n", i);
        }
        else {
            printf("Thread one failed to send message %d\n", i);
        }
    }

    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the two threads
    thread_create(stack_thread_one, sizeof(stack_thread_one), prio_thread_one,
                  THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    thread_create(stack_thread_two, sizeof(stack_thread_two), prio_thread_two,
                  THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    return 0;
}
