// Required Imports
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <kernel.h>
#include <msg.h>

#define THREAD_STACKSIZE         (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE           (8)

// Message queue for thread communication
static msg_t msg_queue[MSG_QUEUE_SIZE];

// Thread stack declaration
static char sender_thread_stack[THREAD_STACKSIZE];
static char receiver_thread_stack[THREAD_STACKSIZE];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Receive a message (blocks until a message is available)
        msg_receive(&msg);
        printf("Received message: %ld\n", (long)msg.content.value);
    }

    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Prepare a message
        msg.content.value = (kernel_pid_t) 42;
        // Send the message to the receiver thread
        msg_send(&msg, thread_get(receiver_pid));

        // Wait for a period before sending the next message
        xtimer_sleep(2);
    }

    return NULL;
}

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create receiver thread
    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                                 THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                 receiver_thread, NULL, "receiver");

    // Create sender thread
    sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                               THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                               sender_thread, NULL, "sender");

    return 0;
}