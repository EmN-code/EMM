#include <thread.h>
#include <msg.h>
#include <sched.h>

// Define a message type
#define MSG_TYPE (0x1234)

char stack[THREAD_STACKSIZE_MAIN];  // Stack for the main thread

void thread_function(void *arg) {
    msg_t msg;
    while (1) {
        // Wait indefinitely for a message
        msg_receive(&msg);

        if (msg.type == MSG_TYPE) {
            printf("Received message with content: %d\n", msg.content.value);
        }
    }
}

int main(void) {
    msg_t msg_queue[8];  // Message queue

    // Initialize the message queue
    msg_init_queue(msg_queue, 8);

    // Create a new thread with a higher priority than main
    thread_create(stack, sizeof(stack), (THREAD_PRIORITY_MAIN - 1), THREAD_CREATE_STACKTEST,
                  thread_function, NULL, "receiver_thread");

    while (1) {
        // Simulate sending a message
        msg_t msg;
        msg.type = MSG_TYPE;
        msg.content.value = 42; // Example content

        // Send the message to the receiver thread
        msg_send(&msg, thread_getpid());

        // Sleep for a while
        xtimer_sleep(2);
    }

    return 0;
}