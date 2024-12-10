#include "thread.h"
#include "msg.h"

// Define stack size and thread priority
#define THREAD_STACKSIZE_MAIN 1024
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

// Message buffer length
#define MESSAGE_QUEUE_SIZE 8

// Declare message queue
msg_t message_queue[MESSAGE_QUEUE_SIZE];

void *sender_thread(void *arg) {
    // Initialize message
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;

    // Sending message to the receiver thread
    msg_send(&msg, thread_get(RECEIVER_THREAD_PRIORITY));

    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg;
    // Initialize message queue to receive messages
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    while (1) {
        // Receiving a message from the queue
        msg_receive(&msg);
        // Process received message
        printf("Received message with value: %u\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    // Create threads with different priorities
    char sender_thread_stack[THREAD_STACKSIZE_MAIN];
    char receiver_thread_stack[THREAD_STACKSIZE_MAIN];

    thread_create(sender_thread_stack, THREAD_STACKSIZE_MAIN, SENDER_THREAD_PRIORITY, 0, sender_thread, NULL, "sender");

    thread_create(receiver_thread_stack, THREAD_STACKSIZE_MAIN, RECEIVER_THREAD_PRIORITY, 0, receiver_thread, NULL, "receiver");

    return 0;
}