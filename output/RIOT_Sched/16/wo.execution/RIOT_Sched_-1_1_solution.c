#include <thread_arch.h>  // Assume this includes necessary threading functions
#include <stdio.h>         // For the printf function

void *thread_function(void *arg) {
    printf("Thread running with initial priority: %d\n", thread_get_priority(thread_getpid()));
    return NULL;
}

int main(void) {
    // Define two threads
    thread_t thread1;
    thread_t thread2;

    // Stack sizes for the threads
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    // Create first thread
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread_function, NULL, "thread1");

    // Create second thread
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread_function, NULL, "thread2");

    // Allow some time for threads to start
    xtimer_sleep(1);

    // Change the priority of the first thread
    printf("Changing priority of first thread\n");
    sched_change_priority(thread_getpid_of(&thread1), THREAD_PRIORITY_MAIN - 2);
    
    // Allow some time for threads to reflect priority change
    xtimer_sleep(1);

    return 0;
}