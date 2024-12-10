#include <stdio.h>
#include <stdlib.h>
#include <thread.h>

#define MY_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE + THREAD_EXTRA_STACKSIZE_PRINTF)
#define MY_THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 1)

void *thread_func(void *arg) {
    (void)arg;
    printf("Hello from thread!\n");
    return NULL;
}

int main(void) {
    printf("Main thread running.\n");

    char *stack1 = malloc(MY_THREAD_STACKSIZE);
    char *stack2 = malloc(MY_THREAD_STACKSIZE);

    if (!stack1 || !stack2) {
        printf("Stack allocation failed!\n");
        return 1;
    }

    kernel_pid_t thread1 = thread_create(stack1, MY_THREAD_STACKSIZE, MY_THREAD_PRIORITY, 0, thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, MY_THREAD_STACKSIZE, MY_THREAD_PRIORITY, 0, thread_func, NULL, "thread2");

    if (thread1 < 0 || thread2 < 0) {
        printf("Thread creation failed!\n");
        return 1;
    }

    printf("Threads started successfully. Changing priorities...\n");
    
    // Example changing priority at runtime
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 3);

    free(stack1);
    free(stack2);

    return 0;
}