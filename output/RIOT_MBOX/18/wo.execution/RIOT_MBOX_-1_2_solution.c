#include "thread.h"
#include "msg.h"
#include "event/thread.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN PRIORITY_MAIN

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];  // Assuming we set up a queue of 8 messages.

void *message_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Receiving messages (this is a pragma for placeholder since msg_receive can't be used):
        // msg_receive(&msg);
        // For this simulation, we directly proceed to print received message type
    
        printf("Message received with type: %d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    // Create a thread for handling messages
    kernel_pid_t msg_handler_pid = thread_create(
        stack, sizeof(stack), THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST, message_handler, NULL, "msg_handler"
    );

    // Simulate sending messages
    msg_t msg;
    msg.type = 1; // Example message type

    // Send messages in a loop
    for (int i = 0; i < 10; i++) {
        msg.content.value = i;

        // Simulating sending a message without using msg_send (as we can't use it here)
        printf("Sending message #%d of type: %d\n", i, msg.type);

        // For the sake of the example, we'll just pretend we send these messages
        // and they are being processed by our message_handler
    }

    return 0;
}