#include <stdio.h>  // For standard input and output functions
#include <thread.h>  // For thread related functions
#include <msg.h>     // For messaging functions
#include <xtimer.h>  // For time-related functions

#define MSG_QUEUE_SIZE 8  // Define the size of the message queue

static msg_t msg_queue[MSG_QUEUE_SIZE];  // Create a message queue array
static kernel_pid_t event_pid;           // PID for the event thread

void *event_thread(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    msg_t msg;

    while (1) {
        xtimer_sleep(1);  // Wait for 1 second
        printf("Sending event...\n");

        // Prepare a message
        msg.content.value = 1;

        // Send a message to the main thread
        msg_send(&msg, event_pid);
    }

    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create the event thread
    char event_stack[THREAD_STACKSIZE_DEFAULT];
    event_pid = thread_create(event_stack, sizeof(event_stack),
                              THREAD_PRIORITY_MAIN - 1,
                              THREAD_CREATE_STACKTEST,
                              event_thread, NULL, "event_thread");

    // Handle messages in the main thread
    msg_t msg;
    while (1) {
        // Wait for a message
        msg_receive(&msg);

        // Process the message
        if (msg.content.value == 1) {
            printf("Received event.\n");
        }
    }

    return 0;
}