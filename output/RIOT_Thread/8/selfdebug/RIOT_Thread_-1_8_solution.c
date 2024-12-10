#include <xtimer.h>  // xtimer_sleep for delay
#include <stdio.h>  // Standard input/output
#include "thread.h"  // For thread related functions
#include "msg.h"  // For message passing

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN + 256) // Size for sender thread
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN + 256) // Size for receiver thread

// Message queue size
#define MESSAGE_QUEUE_SIZE (8)

static char sender_thread_stack[SENDER_THREAD_STACKSIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];

// Function prototypes
void *sender_thread(void *arg);
void *receiver_thread(void *arg);

// The receiver's message queue
msg_t msg_queue[MESSAGE_QUEUE_SIZE];

int main(void)
{
    // Initialize the message queue for the receiver thread
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);

    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, RECEIVER_THREAD_STACKSIZE,
                                              THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    // Check if receiver thread created successfully
    if (receiver_pid <= KERNEL_PID_UNDEF) {
        puts("[Error] Failed to create receiver thread.");
        return 1;
    }

    // Create sender thread
    kernel_pid_t sender_pid = thread_create(sender_thread_stack, SENDER_THREAD_STACKSIZE,
                                            THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                            sender_thread, &receiver_pid, "sender");

    // Check if sender thread created successfully
    if (sender_pid <= KERNEL_PID_UNDEF) {
        puts("[Error] Failed to create sender thread.");
        return 1;
    }

    return 0;
}

// Sender thread function
void *sender_thread(void *arg)
{
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg_t message;
    message.type = 1; // Set a message type

    while (1) {
        puts("[Sender] Sending message to receiver.");
        msg_send(&message, receiver_pid);
        xtimer_sleep(1); // Pause for a second before next send
    }

    return NULL;
}

// Receiver thread function
void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        // Receive message (blocking call)
        msg_receive(&msg);
        printf("[Receiver] Received message with type: %d\n", msg.type);
    }

    return NULL;
}