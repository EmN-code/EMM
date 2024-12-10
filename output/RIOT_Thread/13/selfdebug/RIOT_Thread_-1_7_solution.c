#include <thread.h>
#include <msg.h>
#include <stdio.h>

// Define stack sizes and thread priorities
#define MAIN_THREAD_STACKSIZE  (THREAD_STACKSIZE_DEFAULT)
#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

#define MAIN_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 3)

#define MSG_QUEUE_SIZE          (8)

// Message type
typedef struct msg {
    uint32_t content;
} msg_t;

// Thread stack
char sender_thread_stack[SENDER_THREAD_STACKSIZE];
char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

// Message queue for the receiver thread
msg_t msg_queue[MSG_QUEUE_SIZE];

// Receiver thread function
void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message: %lu\n", msg.content);
    }

    return NULL;
}

// Sender thread function
void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content = i;
        msg_send(&msg, thread_get(RECEIVER_THREAD_PRIORITY));
        printf("Sent message: %lu\n", msg.content);
    }

    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the receiver thread
    thread_create(receiver_thread_stack, RECEIVER_THREAD_STACKSIZE,
                  RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  receiver, NULL, "receiver_thread");

    // Create the sender thread
    thread_create(sender_thread_stack, SENDER_THREAD_STACKSIZE,
                  SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  sender, NULL, "sender_thread");

    // Run main thread loop
    while (1) {}

    return 0;
}
