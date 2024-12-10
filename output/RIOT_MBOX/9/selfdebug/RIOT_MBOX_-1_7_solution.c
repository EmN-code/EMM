#include "msg.h"
#include "mbox.h"
#include "kernel.h"
#include "xtimer.h"
#include "stdio.h"

// Define the queue size (must be a power of two)
#define QUEUE_SIZE        (8)
// Define the interval for sending messages (in microseconds)
#define SEND_INTERVAL     (1000000)

// Create the message buffer
static msg_t msg_queue[QUEUE_SIZE];

static mbox_t my_mbox;

// Function to send a message
static void *sender_thread(void *arg) {
    (void)arg;  // Ignore the argument
    msg_t my_msg;

    puts("Sender thread started");

    while (1) {
        // Fill and send the message
        my_msg.content.value = 42;  // Example value
        mbox_send(&my_mbox, &my_msg);
        printf("Sent message with value: %d\n", my_msg.content.value);

        // Wait for the defined interval
        xtimer_usleep(SEND_INTERVAL);
    }

    return NULL;
}

// Message processing thread
static void *receiver_thread(void *arg) {
    (void)arg;  // Ignore the argument
    msg_t received_msg;

    puts("Receiver thread started");

    while (1) {
        // Block until a message is received
        mbox_get(&my_mbox, &received_msg);
        printf("Received message with value: %d\n", received_msg.content.value);
    }

    return NULL;
}

int main(void) {
    // Initialize the message box
    mbox_init(&my_mbox, msg_queue, QUEUE_SIZE);

    // Create the sender and receiver threads
    char sender_thread_stack[THREAD_STACKSIZE_MAIN];
    char receiver_thread_stack[THREAD_STACKSIZE_MAIN];

    thread_create(sender_thread_stack, sizeof(sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  sender_thread,
                  NULL, "sender_thread");

    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  receiver_thread,
                  NULL, "receiver_thread");

    return 0;
}
