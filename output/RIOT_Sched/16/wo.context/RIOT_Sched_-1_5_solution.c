#include <thread.h>

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define MSG_QUEUE_SIZE          (8)
#define STACK_SIZE              (THREAD_STACKSIZE_DEFAULT)

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread1(void *arg) {
    // Thread 1 functionality
    (void)arg; 
    while (1) {
        // Simulate work
    }
    return NULL;
}

void *thread2(void *arg) {
    // Thread 2 functionality
    (void)arg;
    while (1) {
        // Simulate work
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Create the threads with different stack sizes
    pid1 = thread_create(stack1, STACK_SIZE, THREAD_PRIORITY_MAIN - 1, 0, thread1, NULL, "Thread 1");
    pid2 = thread_create(stack2, STACK_SIZE, THREAD_PRIORITY_MAIN - 2, 0, thread2, NULL, "Thread 2");

    // Obtaining thread control block
    thread_t *thread1_tcb;
    thread1_tcb = thread_get(pid1);

    thread_t *thread2_tcb;
    thread2_tcb = thread_get(pid2);

    // Change priority to the threads
    sched_change_priority(thread1_tcb, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread2_tcb, THREAD_PRIORITY_MAIN - 1);

    return 0;
}