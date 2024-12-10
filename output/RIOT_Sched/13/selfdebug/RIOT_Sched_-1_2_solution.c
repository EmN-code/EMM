#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_LARGE  (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

char stack1[THREAD_STACKSIZE_LARGE];
char stack2[THREAD_STACKSIZE_LARGE];

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Running in thread: %u\n", thread_getpid());
        thread_yield();
    }

    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                         THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        puts("Error creating thread1\n");
        return 1;
    }

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN,
                         THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        puts("Error creating thread2\n");
        return 1;
    }

    printf("Created threads with PIDs %d and %d\n", pid1, pid2);

    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 3);
    
    while (1) {
        /* Additional application logic can be added here */
    }

    return 0;
}