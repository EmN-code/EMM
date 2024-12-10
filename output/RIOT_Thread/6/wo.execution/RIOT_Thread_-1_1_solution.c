#include "thread.h"
#include "msg.h"
#include <stdio.h>

// Define thread stack sizes
#define SENDER_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define RECEIVER_STACKSIZE (THREAD_STACKSIZE_MAIN)

// Define message queue size
#define MSG_QUEUE_SIZE (8)

// Declare message queue
msg_t msg_queue[MSG_QUEUE_SIZE];

// Thread function for the receiver
void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    
    // Initialize message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        // Block until message is received
        if (msg_receive(&msg) == 1) {
            printf("Received message: %d\n", msg.content.value);
        }
    }
    return NULL;
}

// Thread function for the sender
void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Prepare message content
        msg.content.value = 42; // example value

        // Send message to receiver with its PID
        if (msg_send(&msg, thread_getpid_of((thread_t *)arg)) == 1) {
            printf("Sent message: %d\n", msg.content.value);
        }

        // Simulate periodic send
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Declare thread variables
    char sender_stack[SENDER_STACKSIZE];
    char receiver_stack[RECEIVER_STACKSIZE];
    kernel_pid_t receiver_pid;
    kernel_pid_t sender_pid;

    // Create receiver thread
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_MAIN - 1, 0, receiver, NULL, "receiver");

    // Create sender thread
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_MAIN, 0, sender, (void *)receiver_pid, "sender");

    while (1) {
        // You can add more application logic here
        xtimer_sleep(1);
    }
    return 0;
}