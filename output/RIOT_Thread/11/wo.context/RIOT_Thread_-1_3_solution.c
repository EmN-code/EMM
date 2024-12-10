#include <xtimer.h>  // For the xtimer_sleep function
#include <stdlib.h>   // For the rand function
#include <stdio.h>    // For standard input and output functions
#include <msg.h>      // For inter-thread message communication
#include <thread.h>   // For thread creation and management

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT+THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8]; // Message queue for communication

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg.content.value = rand(); // Send a random number
        printf("Sending message with value: %d\n", msg.content.value);
        msg_send(&msg, thread_getpid()); // Send message to the receiver thread
        xtimer_sleep(2); // Sleep for 2 seconds before sending next message
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg); // Receive message
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8); // Initialize the message queue
    
    thread_create(receiver_stack, sizeof(receiver_stack),
                  THREAD_PRIORITY_RECEIVER, 0, receiver, NULL, "receiver");
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0, sender, NULL, "sender");
    return 0;
}