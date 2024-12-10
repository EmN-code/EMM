#include <unistd.h> // Needs to be first alphabetically
#include <stdlib.h> // For memory allocation and random number generation
#include <string.h> // For string operations
#include <stdbool.h> // For boolean type
#include <stddef.h> // For size_t
#include <stdatomic.h> // For atomic types and operations
#include <stdio.h> // For input/output operations
#include "event/thread.h" // For thread management
#include "thread.h" // For threading functionalities
#include "msg.h" // For messaging functionalities

#define HIGH_PRIORITY 5
#define LOW_PRIORITY 2

#define SENDER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char sender_stack[SENDER_STACKSIZE];
static char receiver_stack[RECEIVER_STACKSIZE];

#define NUM_MESSAGES 5

msg_t msg_queue[NUM_MESSAGES];

// Receiver thread function
void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (true) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with type: %u\n", msg.type);
        }
    }
    return NULL;
}

// Sender thread function
void *sender_thread(void *arg) {
    kernel_pid_t receiver_pid = thread_getpid_of((thread_t *)arg);
    for (int i = 0; i < NUM_MESSAGES; ++i) {
        msg_t msg;
        msg.type = (i % 2 == 0) ? HIGH_PRIORITY : LOW_PRIORITY;
        msg.content.value = i; // Assign message content
        if (msg_send(&msg, receiver_pid) == 1) {
            printf("Successfully sent message with type: %u\n", msg.type);
        } else {
            printf("Failed to send message\n");
        }
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, RECEIVER_STACKSIZE, THREAD_PRIORITY_MAIN + 1,
                                              CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    if (receiver_pid == EINVAL) {
        printf("Error creating receiver thread\n");
        return -1;
    }

    kernel_pid_t sender_pid = thread_create(sender_stack, SENDER_STACKSIZE, THREAD_PRIORITY_MAIN + 2,
                                            CREATE_STACKTEST, sender_thread, receiver_pid, "sender");
    if (sender_pid == EINVAL) {
        printf("Error creating sender thread\n");
        return -1;
    }

    return 0;
}