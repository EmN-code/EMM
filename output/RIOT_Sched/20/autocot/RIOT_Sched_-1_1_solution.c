#include <thread_arch.h>
#include <thread_config.h>
#include <stdio.h>

#define THREAD_PRIORITY_MAIN 5  // Base priority for main thread
#define THREAD_STACKSIZE_DEFAULT 1024  // Default stack size for threads

static char stack_one[THREAD_STACKSIZE_DEFAULT];
static char stack_two[THREAD_STACKSIZE_DEFAULT];

void *thread_one_func(void *arg) {
    (void)arg;
    printf("Thread One Initial Priority: %d\n", thread_get_priority(thread_getpid()));
    // Simulate work with a sleep
    while (1) {
        printf("Thread One is running\n");
        thread_sleep();
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    (void)arg;
    printf("Thread Two Initial Priority: %d\n", thread_get_priority(thread_getpid()));
    // Simulate work with a sleep
    while (1) {
        printf("Thread Two is running\n");
        thread_sleep();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1, 0,
                                          thread_one_func, NULL, "thread_one");
    kernel_pid_t pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2, 0,
                                          thread_two_func, NULL, "thread_two");

    printf("Threads have been started\n");

    // Change priority of threads at runtime
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 3);
    printf("Thread One New Priority: %d\n", thread_get_priority(pid_one));

    sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN);
    printf("Thread Two New Priority: %d\n", thread_get_priority(pid_two));

    while (1) {
        thread_sleep();  // Main thread idle
    }

    return 0;
}
