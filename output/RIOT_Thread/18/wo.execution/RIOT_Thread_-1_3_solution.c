// Import required headers
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

// Define the stack size for threads
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

// Define thread priority levels
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN + 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 2)

// Define message queue size, ensure it's a power of two
#define MSG_QUEUE_SIZE (8)

// Allocate stacks for the threads
char stack_sender[THREAD_STACKSIZE];
char stack_receiver[THREAD_STACKSIZE];

// Message queue for the receiver thread
msg_t msg_queue[MSG_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example value

    while (1) {
        // Send message to receiver thread
        msg_send(&msg, thread_getpid_from_name("receiver"));
        // Delay to simulate periodic sending
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        // Receive message and process it
        msg_receive(&msg);
        // Handle message (here we would normally process the data)
        // Instead of using printf, use an alternative output method
        xtimer_usleep(1000);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue for receiver thread
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create sender thread
    kernel_pid_t sender_pid = thread_create(
        stack_sender, THREAD_STACKSIZE,
        THREAD_PRIORITY_SENDER,
        THREAD_CREATE_STACKTEST,
        sender_thread, NULL,
        "sender"
    );

    // Create receiver thread
    kernel_pid_t receiver_pid = thread_create(
        stack_receiver, THREAD_STACKSIZE,
        THREAD_PRIORITY_RECEIVER,
        THREAD_CREATE_STACKTEST,
        receiver_thread, NULL,
        "receiver"
    );

    // Yield to allow other threads to run
    thread_yield();

    return 0;
}