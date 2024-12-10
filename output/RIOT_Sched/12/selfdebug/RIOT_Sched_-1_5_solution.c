#include <thread.h>
#include <stdio.h>

#define THREAD_PRIORITY_ONE      (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_TWO      (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE_THREAD_ONE     THREAD_STACKSIZE_MAIN
#define STACKSIZE_THREAD_TWO     THREAD_STACKSIZE_MAIN

// Stack memory for the threads
char stack_one[STACKSIZE_THREAD_ONE];
char stack_two[STACKSIZE_THREAD_TWO];

// Thread IDs
kernel_pid_t pid_one;
kernel_pid_t pid_two;

// Thread function
void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Running thread %d\n", thread_getpid());
        thread_yield();
    }
    return NULL;
}

int main(void) {
    // Create first thread
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_ONE, 
                            0, thread_func, NULL, "thread_one");
    
    // Create second thread
    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_TWO, 
                            0, thread_func, NULL, "thread_two");

    // Change priorities at runtime
    thread_t *thread_one = thread_get(pid_one);
    thread_t *thread_two = thread_get(pid_two);

    sched_change_priority(thread_one, THREAD_PRIORITY_TWO);
    sched_change_priority(thread_two, THREAD_PRIORITY_ONE);

    return 0;
}