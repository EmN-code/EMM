#include <stdio.h>
#include <thread.h>
#include <msg.h>

// Define stack sizes for threads
#define STACK_SIZE_THREAD_A (THREAD_STACKSIZE_DEFAULT)
#define STACK_SIZE_THREAD_B (THREAD_STACKSIZE_DEFAULT)

// Define priorities for threads
#define PRIORITY_THREAD_A (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_THREAD_B (THREAD_PRIORITY_MAIN + 1)

// Define message queue size
#define MSG_QUEUE_SIZE (8)

// Declare message queue
static msg_t msg_queue[MSG_QUEUE_SIZE];

// Semaphores for inter-thread communication
kernel_pid_t pid_thread_a;
kernel_pid_t pid_thread_b;

// Function prototype for thread A
void *thread_a(void *arg);

// Function prototype for thread B
void *thread_b(void *arg);

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    
    // Create thread A
    pid_thread_a = thread_create((char *)(((char *)malloc(STACK_SIZE_THREAD_A)) + STACK_SIZE_THREAD_A),
                                 STACK_SIZE_THREAD_A,
                                 PRIORITY_THREAD_A,
                                 THREAD_CREATE_STACKTEST,
                                 thread_a,
                                 NULL,
                                 "thread_a");

    // Create thread B
    pid_thread_b = thread_create((char *)(((char *)malloc(STACK_SIZE_THREAD_B)) + STACK_SIZE_THREAD_B),
                                 STACK_SIZE_THREAD_B,
                                 PRIORITY_THREAD_B,
                                 THREAD_CREATE_STACKTEST,
                                 thread_b,
                                 NULL,
                                 "thread_b");

    return 0;
}

void *thread_a(void *arg) {
    (void)arg;
    msg_t msg;
    while(1) {
        // Send a message to thread B
        msg.content.value = 1;
        thread_yield_higher();
        msg_send_receive(&msg, NULL, pid_thread_b);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_b(void *arg) {
    (void)arg;
    msg_t msg;
    while(1) {
        // Wait to receive a message
        msg_receive(&msg);
        printf("Message received with value: %d\n", msg.content.value);
        thread_yield_higher();
        xtimer_sleep(1);
    }
    return NULL;
}
