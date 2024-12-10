#include <stdio.h>  // Include stdio.h for printf and integer conversion
#include <stdlib.h> // Include stdlib.h for malloc and free functions
#include <stdint.h> // Include stdint.h for uint32_t type
#include "thread.h" // Required to handle threading functionalities

// Define thread stacks and their size
#define THREAD_STACKSIZE_MAIN    THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)


// Declare two thread stacks
static char sender_thread_stack[THREAD_STACKSIZE_MAIN];
static char receiver_thread_stack[THREAD_STACKSIZE_MAIN];

// Message structure
typedef struct msg {
    uint32_t content;
} msg_t;

// Function executed by the sender thread
void *sender_thread(void *arg) {
    (void)arg; // Avoid unused variable warning
    msg_t message;
    message.content = 12345;  // Assign a test value
    printf("Sender: Sending message with content %lu\n", (unsigned long)message.content);
    return NULL;
}

// Function executed by the receiver thread
void *receiver_thread(void *arg) {
    (void)arg; // Avoid unused variable warning
    msg_t message;
    message.content = 0;  // Initialize to zero
    printf("Receiver: Received message with content %lu\n", (unsigned long)message.content);
    return NULL;
}

int main(void) {
    // Create sender thread
    kernel_pid_t sender_pid = thread_create(
        sender_thread_stack,
        sizeof(sender_thread_stack),
        SENDER_THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender_thread");

    // Create receiver thread
    kernel_pid_t receiver_pid = thread_create(
        receiver_thread_stack,
        sizeof(receiver_thread_stack),
        RECEIVER_THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver_thread");

    printf("Sender thread PID: %u, Receiver thread PID: %u\n", (unsigned int)sender_pid, (unsigned int)receiver_pid);

    // Sleep to allow threads to run
    thread_sleep();
    return 0;
}