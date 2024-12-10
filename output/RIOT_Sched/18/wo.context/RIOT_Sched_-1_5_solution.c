#include <thread.h>
#include <stdio.h>

#define STACK_SIZE THREAD_STACKSIZE_MAIN
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

char thread_stack[STACK_SIZE];

void *thread_func(void *arg) {
    printf("Thread running\n");
    return NULL;
}

int main(void) {
    /* Create a thread */
    thread_t *t1 = thread_create(thread_stack, STACK_SIZE, THREAD_PRIORITY, 0, thread_func, NULL, "thread_1");
    if (t1 == NULL) {
        printf("Failed to create thread\n");
        return 1;
    }

    /* Change priority of the thread */
    sched_change_priority(t1, THREAD_PRIORITY_MAIN);

    /* Wait for the thread to complete */
    thread_kill_zombie(t1->pid);

    printf("Main thread done\n");
    return 0;
}