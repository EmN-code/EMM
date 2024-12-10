#include <thread_arch.h>
#include <stdio.h>
#include <stdint.h>

#define STACK_SIZE THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN 5

void *thread_entry_point(void *arg) {
    printf("Thread %d is running with priority %d\n", (intptr_t)arg, (int)thread_get_priority((thread_t *)sched_active_thread)); 
    return NULL;
}

int main(void) {
    char stack1[STACK_SIZE];
    char stack2[STACK_SIZE];

    thread_t thread1;
    thread_t thread2;

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1),
                                      THREAD_PRIORITY_MAIN,
                                      CREATE_STACKTEST,
                                      thread_entry_point, (void *)1,
                                      "thread1");
    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2),
                                      THREAD_PRIORITY_MAIN - 1,
                                      CREATE_STACKTEST,
                                      thread_entry_point, (void *)2,
                                      "thread2");

    printf("Main: Created threads with pids %d and %d\n", pid1, pid2);

    // Simulate some processing time
    for (volatile int i = 0; i < 1000000; i++);

    printf("Main: Changing priorities of the threads\n");
    sched_set_priority(pid1, THREAD_PRIORITY_MAIN - 1);
    sched_set_priority(pid2, THREAD_PRIORITY_MAIN);

    // Simulate more processing time
    for (volatile int i = 0; i < 1000000; i++);

    return 0;
}