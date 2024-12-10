#include <thread_arch.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN (1024)
#define THREAD_PRIORITY_MAIN (6)

char thread_stack_1[THREAD_STACKSIZE_MAIN];
char thread_stack_2[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    int thread_number = *(int *)arg;
    printf("Thread %d: running!\n", thread_number);
    while (1) {
        // Simulate thread work
        printf("Thread %d: is alive\n", thread_number);
        thread_yield();
    }
    return NULL;
}

int main(void) {
    printf("Starting two threads\n");

    kernel_pid_t thread_pid_1, thread_pid_2;
    int arg1 = 1, arg2 = 2;

    thread_pid_1 = thread_create(thread_stack_1, sizeof(thread_stack_1),
                                 THREAD_PRIORITY_MAIN - 1, 0,
                                 thread_handler, &arg1, "thread_1");

    thread_pid_2 = thread_create(thread_stack_2, sizeof(thread_stack_2),
                                 THREAD_PRIORITY_MAIN - 2, 0,
                                 thread_handler, &arg2, "thread_2");

    printf("Created threads with PIDs: %d, %d\n", thread_pid_1, thread_pid_2);

    printf("Changing priority of thread 1\n");
    sched_change_priority(thread_pid_1, THREAD_PRIORITY_MAIN - 3);

    // Let the threads run (infinite loop)
    while (1) {}

    return 0;
}