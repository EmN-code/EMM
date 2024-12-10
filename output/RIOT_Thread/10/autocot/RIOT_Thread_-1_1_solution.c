#include "periph/uart.h"
#include "periph/tim.h"
#include "msg.h"
#include "kernel_types.h"

// Define the stack size for threads
#define THREAD_STACKSIZE_MAIN    (1024)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW      (THREAD_PRIORITY_MAIN - 2)

static char stack[THREAD_STACKSIZE_MAIN];

// Define the message queue
#define MESSAGE_QUEUE_SIZE 4
static msg_t message_queue[MESSAGE_QUEUE_SIZE];

// Thread function to send messages
void *send_thread(void *arg) {
    (void)arg;
    msg_t msg;

    // Initialize the message queue
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    for (int i = 0; i < 10; ++i) {
        // Prepare a message with type and content
        msg.type = i;  // Just an example to vary the message type

        // Send the message to the receiving thread with higher priority
        msg_send_receive(&msg, NULL, thread_getpid());

        tim_sleep(1);
    }
    return NULL;
}

// Thread function to receive messages
void *receive_thread(void *arg) {
    (void)arg;
    msg_t msg;

    // Initialize the message queue
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    while (1) {
        // Wait for incoming messages
        if (msg_receive(&msg) >= 0) {
            printf("Received message, Type: %d\n", msg.type);
        }
        thread_yield_higher(); // Yield so that a higher priority sender can run
    }
    return NULL;
}

int main(void) {
    // Launch the receiver thread with higher priority
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, receive_thread, NULL, "receiver");

    // Launch the sender thread with lower priority
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_LOW, 0, send_thread, NULL, "sender");

    // Scheduler should handle the threads
    return 0;
}
