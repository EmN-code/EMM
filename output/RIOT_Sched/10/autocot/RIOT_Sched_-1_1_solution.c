#include thread_arch.h
#include thread_config.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREAD_STACKSIZE_MAIN  (RIOT_THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND (THREAD_PRIORITY_MAIN - 2)

char thread_stack_one[THREAD_STACKSIZE_MAIN];
char thread_stack_two[THREAD_STACKSIZE_MAIN];

void *thread_function_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One is running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_function_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two is running\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(thread_stack_one, sizeof(thread_stack_one),
                                        THREAD_PRIORITY_MAIN, 0,
                                        thread_function_one, NULL, "Thread One");

    kernel_pid_t pid_two = thread_create(thread_stack_two, sizeof(thread_stack_two),
                                        THREAD_PRIORITY_SECOND, 0,
                                        thread_function_two, NULL, "Thread Two");

    printf("Threads started. Now changing priority of Thread One\n");
    sched_set_priority(pid_one, THREAD_PRIORITY_SECOND);

    printf("Now changing priority of Thread Two\n");
    sched_set_priority(pid_two, THREAD_PRIORITY_MAIN);

    while (1) {
        // Main thread loop
        printf("Main thread is running\n");
        thread_yield();
    }

    return 0;
}