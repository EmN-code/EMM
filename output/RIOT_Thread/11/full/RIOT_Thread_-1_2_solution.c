/* This program demonstrates inter-thread communication using msg_t structures in RIOT OS on ESP32,
   where one thread sends messages to another thread. The threads are assigned different priorities. */

// Including necessary header files for the program
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"

// Define stack sizes for threads
#define STACK_SIZE_THREAD_1   (THREAD_STACKSIZE_DEFAULT)
#define STACK_SIZE_THREAD_2   (THREAD_STACKSIZE_DEFAULT)

// Define priorities for the threads
#define PRIORITY_THREAD_1     (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD_2     (THREAD_PRIORITY_MAIN - 2)

// Define the message queue size
#define MESSAGE_QUEUE_SIZE    (4)

// Declare stacks for the threads
char stack_thread_1[STACK_SIZE_THREAD_1];
char stack_thread_2[STACK_SIZE_THREAD_2];

// Message queue for the receiving thread
msg_t msg_queue[MESSAGE_QUEUE_SIZE];

void *thread_sender(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        // Prepare the message to send
        msg.type = 0x1234;  // Set a message type
        msg.content.value = 42;  // Set a message content value

        // Send message to the receiver thread
        if (msg_send(&msg, thread_getpid()) < 0) {
            // Handle error
            printf("Failed to send message\n");
        }
        
        // Sleep for a while before sending the next message
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

void *thread_receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Initialize the message queue
        msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);

        // Receive the message (this will block until a message is received)
        msg_receive(&msg);

        // Process the received message
        printf("Received message of type: %d, with content: %lu\n", msg.type, msg.content.value);
    }

    return NULL;
}

int main(void) {
    // Initialize queues
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);

    // Create the sender thread
    if (thread_create(stack_thread_1, sizeof(stack_thread_1), PRIORITY_THREAD_1, 0,
                      thread_receiver, NULL, "thread_receiver") <= KERNEL_PID_UNDEF) {
        // Handle error
        printf("Failed to create sender thread\n");
        return 1;
    }

    // Create the receiver thread
    if (thread_create(stack_thread_2, sizeof(stack_thread_2), PRIORITY_THREAD_2, 0,
                      thread_sender, NULL, "thread_sender") <= KERNEL_PID_UNDEF) {
        // Handle error
        printf("Failed to create receiver thread\n");
        return 1;
    }

    // Main thread will do nothing and let other threads handle the communication
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}