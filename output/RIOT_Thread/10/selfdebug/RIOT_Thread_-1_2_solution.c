// Description of the problem and approach
// This RIOT code demonstrates inter-thread communication using msg_t on an ESP32. We have two threads:
// - Main thread: Initializes the message queue and creates a secondary thread.
// - Secondary thread: Sends messages to the main thread with different priorities, showing how message
//   passing can be handled in a priority-based manner.

#include <stdio.h>
#include "thread.h"
#include "msg.h"

// Define stack sizes and priorities
#define MAIN_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SECOND_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define SECOND_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

// Message queue size - Must be a power of two!
#define QUEUE_SIZE (8)

// Define a message type for demonstration
#define MESSAGE_TYPE (0x1234)

static char second_thread_stack[SECOND_THREAD_STACKSIZE];

void *second_thread(void *arg) {
    msg_t msg;
    msg.type = MESSAGE_TYPE;
    msg.content.value = 42; // Example message content

    // Send message with priority handling
    msg_send(&msg, thread_getpid());

    return NULL;
}

int main(void) {
    msg_t queue[QUEUE_SIZE];
    msg_init_queue(queue, QUEUE_SIZE);

    // Create second thread
    kernel_pid_t second_pid = thread_create(second_thread_stack, sizeof(second_thread_stack),
                                            SECOND_THREAD_PRIO, THREAD_CREATE_STACKTEST,
                                            second_thread, NULL, "second_thread");

    // Handle messages in main thread
    while (1) {
        msg_t msg;
        // Blocking receive
        msg_receive(&msg);
        if (msg.type == MESSAGE_TYPE) {
            printf("Received message with value: %lu\n", msg.content.value);
        }
    }
    return 0;
}