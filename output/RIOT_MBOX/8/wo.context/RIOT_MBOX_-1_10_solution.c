// Required headers in descending order
#include "xtimer.h"
#include "stdio.h"
#include "msg.h"
#include "kernel.h"

// Constants
#define MAIN_QUEUE_SIZE     (8)
#define MSG_INTERVAL        (1000000U) // Interval in microseconds

// Global message queue for main thread
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

// Thread stack
char stack[THREAD_STACKSIZE_MAIN];

// Function that sends a message periodically
void *thread_func(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        xtimer_usleep(MSG_INTERVAL);
        msg.content.value = 42;  // Example message content
        printf("Sending message.\n");
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue of this thread
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    
    printf("RIOT message box example\n");

    // Create a thread that sends messages
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_func, NULL, "sender thread");

    // Main loop: Receive and handle messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}