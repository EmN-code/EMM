#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <kernel.h>

#define INTERVAL (1000000U) // Define the interval for the messages in microseconds
#define RECEIVER_QUEUE_SIZE (8) // Define the size of the message queue

// Initialize a message queue
static msg_t _msg_queue[RECEIVER_QUEUE_SIZE];

// The thread that will be receiving messages
static char stack[THREAD_STACKSIZE_DEFAULT];

// Function to handle received messages
void *_receiver_thread(void *arg) {
    (void) arg; // Unused parameter
    msg_t msg;

    while (1) {
        // Wait for a message to be received
        if (msg_receive(&msg) < 0) {
            puts("Error receiving message");
        } else {
            printf("Received message with value: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    // Initialize message queue for the receiver thread
    msg_init_queue(_msg_queue, RECEIVER_QUEUE_SIZE);

    // Create a thread for receiving messages
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _receiver_thread, NULL, "receiver");

    // Timer for periodic messages
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Send a message to the receiver thread
        msg_t msg;
        msg.content.value = xtimer_usec_from_ticks(xtimer_now());
        if (msg_send(&msg, thread_get(RECEIVER_QUEUE_SIZE - 1)) < 0) {
            puts("Error sending message");
        }

        // Wait for the next interval
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}