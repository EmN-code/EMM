#include <thread.h>
#include <stdio.h>

// Define thread stack sizes and priorities
#define STACK_SIZE THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

// Declare stacks and threads
char stack1[STACK_SIZE];
char stack2[STACK_SIZE];
kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

// Thread function
void *thread_function(void *arg) {
    int thread_number = (int) arg;
    printf("Thread %d running.\n", thread_number);
    return NULL;
}

int main(void) {
    // Print initial message
    puts("Starting threads on ESP32\n");

    // Create two threads
    thread1_pid = thread_create(stack1, STACK_SIZE, THREAD_PRIORITY_MAIN, 0, thread_function, (void *)1, "thread1");
    thread2_pid = thread_create(stack2, STACK_SIZE, THREAD_PRIORITY_MAIN, 0, thread_function, (void *)2, "thread2");

    printf("Created threads with PIDs: %d and %d\n", thread1_pid, thread2_pid);

    // Change priority of threads
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);

    // Keep main running
    while (1) {
        // Optionally do something in the main thread
    }

    return 0;
}