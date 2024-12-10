
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>

// Include necessary headers for RIOT
#include "msg.h"
#include "thread.h"

// Define stack sizes and priorities
#define STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 3

// Function prototypes
void *high_priority_thread(void *arg);
void *low_priority_thread(void *arg);

// Main function
int main(void)
{
    // Allocate stack space for threads
    char stack_high[STACKSIZE];
    char stack_low[STACKSIZE];

    // Create high priority thread
    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, high_priority_thread, NULL, "high_priority");

    // Create low priority thread
    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, low_priority_thread, NULL, "low_priority");

    // Initialize message queue for main thread
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    // Main loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Main thread received message from PID %" PRIkernel_pid "\n", msg.sender_pid);
    }

    return 0;
}

// High priority thread function
void *high_priority_thread(void *arg)
{
    (void)arg;

    // Initialize message queue for high priority thread
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    while (1) {
        msg_t msg;
        msg.type = 1;
        msg.content.value = 123;
        msg_send(&msg, thread_getpid());
        sleep(1);
    }

    return NULL;
}

// Low priority thread function
void *low_priority_thread(void *arg)
{
    (void)arg;

    // Initialize message queue for low priority thread
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    while (1) {
        msg_t msg;
        msg.type = 2;
        msg.content.value = 456;
        msg_send(&msg, thread_getpid());
        sleep(2);
    }

    return NULL;
}
