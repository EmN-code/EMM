// Include necessary headers in alphabetical order.
#include "xtimer.h"

// Define macros for thread stack size and priorities if not already defined
#define MAIN_THREAD_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define MAIN_THREAD_PRIORITY     (THREAD_PRIORITY_MAIN + 1)
#define WORKER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN + 2)

// Declare stacks for the threads
static char main_thread_stack[MAIN_THREAD_STACKSIZE];
static char worker_thread_stack[MAIN_THREAD_STACKSIZE];

// Declare message queues
static msg_t main_msg_queue[8];  // Size of the queue (should be a power of two)
static msg_t worker_msg_queue[8];

// Function for the worker thread
to process incoming messages
void *worker_thread(void *arg) {
    (void)arg;
    msg_t m;
    msg_init_queue(worker_msg_queue, 8);
    while(1) {
        // Receive a message and process it
        msg_receive(&m);
        // Process the received message (e.g., print something)
        printf("Worker thread received message with type: %u\n", m.type);
    }
    return NULL;
}

// Function for the main thread
to send messages to the worker thread
void *main_thread(void *arg) {
    (void)arg;
    msg_t m;
    msg_init_queue(main_msg_queue, 8);
    kernel_pid_t worker_pid = thread_create(worker_thread_stack, MAIN_THREAD_STACKSIZE,
                                            WORKER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                            worker_thread, NULL, "worker_thread");

    // Send messages in a loop
    while(1) {
        // Prepare a message
to send
        m.type = 1;  // Set a message type

        // Send the message to the worker thread
        msg_send(&m, worker_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    // Start the main thread
    thread_create(main_thread_stack, MAIN_THREAD_STACKSIZE,
                  MAIN_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                  main_thread, NULL, "main_thread");

    // Allow the main function to continue doing other things (if needed)
    while (1) {
        xtimer_sleep(2);
    }
    return 0;
}