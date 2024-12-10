#include <xtimer.h>   
#include <sched.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <thread.h>   
#include <malloc.h>

// Define thread stack size
#define THREAD_STACKSIZE_MAIN 1024

// Thread priority
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)

// declare function for the sender thread
typedef void* (*thread_task_func_t)(void*);

// message structure
msg_t msg;

// Message queues for the threads
msg_t sender_queue[4];
msg_t receiver_queue[4];

// Receiver thread function
void* receiver_thread(void *arg) {
    msg_init_queue(receiver_queue, 4);  // Initialize the queue for receiver
    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("Received message with type: %d, value: %ld\n", msg.type, msg.content.value);
        }
    }
    return NULL;
}

// Sender thread function
void* sender_thread(void *arg) {
    msg_init_queue(sender_queue, 4);  // Initialize the queue for sender
    msg_t send_msg;
    send_msg.type = 1;
    send_msg.content.value = 42;  // sending a value

    while (1) {
        msg_send(&send_msg, thread_getpid());  // Send a message to receiver
        printf("Sent message to receiver\n");
        xtimer_sleep(2);  // Sleep to simulate interval 
    }

    return NULL;
}

enum{
    THREAD_PRIORITY_MAIN = 24
};

int main(void) {
    // Thread IDs
    kernel_pid_t sender_pid, receiver_pid;

    // Allocate memory for the stacks
    char *sender_stack = malloc(THREAD_STACKSIZE_MAIN);
    char *receiver_stack = malloc(THREAD_STACKSIZE_MAIN);

    // Create threads
    receiver_pid = thread_create(receiver_stack, THREAD_STACKSIZE_MAIN,
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver_thread, NULL, "receiver");

    sender_pid = thread_create(sender_stack, THREAD_STACKSIZE_MAIN,
                               THREAD_PRIORITY_SENDER, 0,
                               sender_thread, NULL, "sender");

    while(1) {
        // Main thread can perform other tasks
        xtimer_sleep(3);
    }

    // Free allocated stacks
    free(sender_stack);
    free(receiver_stack);

    return 0;
}