#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include <stdio.h>

// Define the stack size and priorities
#define THREAD_STACKSIZE_MAIN   THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define SECONDARY_THREAD_PRIORITY (THREAD_PRIORITY_MAIN + 1)

// Define the stack for the threads
char stack[THREAD_STACKSIZE_MAIN];
char secondary_stack[THREAD_STACKSIZE_MAIN];

// Function for the secondary thread
void *secondary_thread(void *arg) {
    msg_t msg;
    (void)arg;
    
    while (1) {
        if (msg_receive(&msg) < 0) {
            printf("Error receiving message\n");
        } else {
            printf("Secondary thread received message with content: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

// Main function
int main(void) {
    msg_t msg;
    kernel_pid_t secondary_pid;

    // Create the secondary thread
    secondary_pid = thread_create(secondary_stack, sizeof(secondary_stack),
                                  SECONDARY_THREAD_PRIORITY,
                                  THREAD_CREATE_STACKTEST,
                                  secondary_thread, NULL, "secondary");

    if (secondary_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating secondary thread\n");
        return 1;
    }

    while (1) {
        // Send a message to the secondary thread
        msg.content.value = xtimer_now_usec();
        if (msg_send(&msg, secondary_pid) < 1) {
            printf("Error sending message\n");
        }
        xtimer_sleep(1);
    }
    return 0;
}