#include <thread.h>  // Correcting the include error.
#include <msg.h>     // Include required for message handling.
#include <stdio.h>   // Include for standard input and output operations.

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 5

// Define message queue
#define MSG_QUEUE_SIZE 8
static msg_t msg_queue[MSG_QUEUE_SIZE];

// Declare stacks for threads
static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

// Thread function for sending messages
void *sender_thread(void *arg) {
    (void)arg;

    msg_t msg;
    msg.content.value = 42;  // Example message value

    while (1) {
        msg_send(&msg, thread_getpid());
        printf("Message sent with value %u\n", (unsigned)msg.content.value);
        xtimer_sleep(2);
    }

    return NULL;
}

// Thread function for receiving messages
void *receiver_thread(void *arg) {
    (void)arg;

    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received message with value %u\n", (unsigned)msg.content.value);
        }
    }

    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create threads with different priorities
    thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, sender_thread, NULL, "sender");
    thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, receiver_thread, NULL, "receiver");

    // Main thread loop
    while (1) {
        // Currently, the main thread does not perform additional tasks
        xtimer_sleep(1);
    }

    return 0;
}