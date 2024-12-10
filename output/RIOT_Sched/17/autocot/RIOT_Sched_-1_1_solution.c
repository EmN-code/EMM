#include "thread_arch.h"
#include "thread_config.h"

#define THREAD_STACKSIZE_MAIN    1024
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND   (THREAD_PRIORITY_MAIN - 3)

char thread_stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        thread_stack_print();
        thread_yield();
    }
    return NULL;
}

int main(void) {
    printf("Starting the RIOT example for thread management...\n");
    
    kernel_pid_t pid_thread;

    /* Create a new thread */
    pid_thread = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN, 0,
                               thread_handler, NULL, "thread1");
    if (pid_thread <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    /* Change the priority of the thread at runtime */
    int res = sched_change_priority(pid_thread, THREAD_PRIORITY_SECOND);
    if (res < 0) {
        printf("Failed to change priority of thread 1\n");
        return 1;
    }

    printf("Successfully changed priority of thread 1\n");
    while (1) {
        /* Main thread work */
        thread_yield();
    }

    return 0;
}