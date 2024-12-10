#include <thread.h>
#include <msg.h>
#include <stdio.h>
#include <xtimer.h>

// Define a constant for the maximum number of messages in the queue
#define MSG_QUEUE_SIZE 8

// Declare message queues for the two threads
msg_t msg_queue_a[MSG_QUEUE_SIZE];
msg_t msg_queue_b[MSG_QUEUE_SIZE];

// Define stack sizes and thread priorities
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_A (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_B (THREAD_PRIORITY_MAIN - 2)

// Stack declarations
char stack_a[THREAD_STACKSIZE];
char stack_b[THREAD_STACKSIZE];

void *thread_a(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Arbitrary payload

    while (1) {
        msg_send(&msg, thread_getpid_of((thread_t *)arg));  // Send message to thread B
        printf("Thread A: Message sent\n");
        xtimer_sleep(2);  // Sleep for 2 seconds
    }

    return NULL;
}

void *thread_b(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == 1) {  // Attempt to receive a message
            printf("Thread B: Message received with value %d\n", msg.content.value);
        }
        xtimer_sleep(1);  // Sleep for 1 second
    }

    return NULL;
}

int main(void) {
    // Initialize message queues
    msg_init_queue(msg_queue_a, MSG_QUEUE_SIZE);
    msg_init_queue(msg_queue_b, MSG_QUEUE_SIZE);

    // Create threads
    kernel_pid_t pid_a = thread_create(stack_a, sizeof(stack_a), THREAD_PRIORITY_A, 0, thread_a, (void *)msg_queue_b, "thread_a");
    kernel_pid_t pid_b = thread_create(stack_b, sizeof(stack_b), THREAD_PRIORITY_B, 0, thread_b, (void *)msg_queue_a, "thread_b");

    if (pid_a <= KERNEL_PID_UNDEF || pid_b <= KERNEL_PID_UNDEF) {
        printf("Failed to create threads\n");
        return 1;
    }

    // Main thread does nothing
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}