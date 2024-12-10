#include <thread.h> // Importing threading library
#include <stdio.h>  // Including standard IO library

// Define stack size for threads
#define STACK_SIZE THREAD_STACKSIZE_DEFAULT

// Define priorities
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW  (THREAD_PRIORITY_MAIN - 2)

// Declare threads and stacks
char stack_high[STACK_SIZE];
char stack_low[STACK_SIZE];

kernel_pid_t thread_high_pid;
kernel_pid_t thread_low_pid;

// Define message queue size
#define QUEUE_SIZE 8
msg_t msg_queue[QUEUE_SIZE];

void *thread_high(void *arg) {
    (void)arg;
    msg_t msg;
    printf("High priority thread is running!\n");
    while (1) {
        // Receive message
        msg_receive(&msg);
        // Print the received value
        printf("High thread received: %ld\n", msg.content.value);
    }
    return NULL;
}

void *thread_low(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    printf("Low priority thread is running!\n");
    while (1) {
        // Send a message
        msg_send(&msg, thread_high_pid);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);
    
    // Create high priority thread
    thread_high_pid = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, thread_high, NULL, "high");
    
    // Create low priority thread
    thread_low_pid = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, thread_low, NULL, "low");

    return 0;
}