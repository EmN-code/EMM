#include <thread.h>        // Ensure necessary RIOT headers are at the top
#include <sched.h>         // Include scheduler for priority
#include <stdio.h>         // For print functions

#define STACK_SIZE THREAD_STACKSIZE_DEFAULT

char stack1[STACK_SIZE];   // Allocate stack size for threads
char stack2[STACK_SIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_handler(void *arg) {
    (void) arg;  // arg is unused
    printf("Thread started\n");
    while (1) {
        // Perform thread tasks
    }
    return NULL;
}

int main(void) {
    thread1_pid = thread_create(stack1, STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread_handler, NULL, "Thread1");
    thread2_pid = thread_create(stack2, STACK_SIZE, THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread_handler, NULL, "Thread2");

    // Change the priority of thread2 at runtime
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 3);

    while (1) {
        // Main loop tasks
    }
    return 0;
}