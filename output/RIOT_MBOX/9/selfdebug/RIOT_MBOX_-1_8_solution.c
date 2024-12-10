#include "msg.h"
#include "kernel.h"
#include "thread.h"
#include "fmt.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_MSGS (8)
#define INTERVAL (1000000U) // 1 second

static msg_t queue[NUM_OF_MSGS];

// Thread function for sending messages
tstatic void *sender(void *arg) {
    (void) arg;
    msg_t msg;
    msg.content.value = 0;
    while (1) {
        printf("Sending message with value %d\n", (int)msg.content.value);
        msg_send(&msg, thread_getpid()); // send to message queue of this thread
        msg.content.value++;
        usleep(INTERVAL); // wait for the specified interval
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(queue, NUM_OF_MSGS);

    // Create a thread for message sending
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    // Main loop: receive messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value %d\n", (int)msg.content.value);
    }

    return 0;
}
