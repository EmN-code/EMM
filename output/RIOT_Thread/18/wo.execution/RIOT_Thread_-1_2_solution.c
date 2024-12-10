// Description: This solution demonstrates a simple RIOT application on an ESP32 where two threads communicate using message passing. The main thread creates two secondary threads with different priorities to send and receive messages using the `msg_t` structure. Additionally, the code resolves the issue with the wrong format specifier by correcting the type to match the format.

// Necessary imports for the RIOT application
#include <thread.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <evtimer_msg.h>
#include <event/thread.h>

// Stack size for each created thread
#define THREAD_STACKSIZE_DEFAULT (1024)
// Message queue size
#define QUEUE_SIZE (8)

// Thread priorities
#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)

// Global variables for threads and message queue
static msg_t msg_queue[QUEUE_SIZE];
static char sender_thread_stack[THREAD_STACKSIZE_DEFAULT];
static char receiver_thread_stack[THREAD_STACKSIZE_DEFAULT];

// Function prototypes
void *sender_thread(void *arg);
void *receiver_thread(void *arg);

int main(void) {
    // Initialize the message queue for the current thread
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Create the receiver thread
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), RECEIVER_PRIO, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    // Create the sender thread
    kernel_pid_t sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack), SENDER_PRIO, THREAD_CREATE_STACKTEST, sender_thread, &receiver_pid, "sender");

    // Let the main thread idle
    thread_yield();

    return 0;
}

void *sender_thread(void *arg) {
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    // Prepare a message
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    // Send the message to the receiver thread
    msg_send(&msg, receiver_pid);

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;

    // Initialize a message structure to receive in
    msg_t msg;

    // Wait for the message
    msg_receive(&msg);

    // Print the received message type and content
    printf("Received message type: %d\n", msg.type);
    printf("Received message value: %u\n", (unsigned int)msg.content.value);

    return NULL;
}