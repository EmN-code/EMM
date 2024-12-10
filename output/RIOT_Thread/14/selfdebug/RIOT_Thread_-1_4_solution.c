#include <stdio.h> // For printf
#include <stdlib.h> // For exit
#include <thread.h> // For thread handling
#include <msg.h> // For message handling

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define SENDER_THREAD_PRIO (THREAD_PRIORITY_MAIN + 1)
#define RECEIVER_THREAD_PRIO (THREAD_PRIORITY_MAIN + 2)

static char sender_thread_stack[SENDER_THREAD_STACKSIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

static msg_t msg_queue[8]; // Message queue for communication

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    // Initialize a message queue
    msg_init_queue(msg_queue, 8);

    while (1) {
        // Receive message
        msg_receive(&msg);
        printf("Received message with type: %lu\n", msg.type);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 42; // Assigning the message type

    // Send message to receiver thread
    while (1) {
        printf("Sending message with type: %lu\n", msg.type);
        msg_send(&msg, thread_get(RECEIVER_THREAD_PRIO));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_thread_stack, 
                  RECEIVER_THREAD_STACKSIZE,
                  RECEIVER_THREAD_PRIO,
                  THREAD_CREATE_STACKTEST,
                  receiver_thread, 
                  NULL,
                  "receiver_thread");

    thread_create(sender_thread_stack, 
                  SENDER_THREAD_STACKSIZE,
                  SENDER_THREAD_PRIO,
                  THREAD_CREATE_STACKTEST,
                  sender_thread, 
                  NULL,
                  "sender_thread");

    return 0;
}