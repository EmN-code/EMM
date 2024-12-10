#include <thread_arch.h>
#include <thread.h>
#include <kernel_types.h>

// Define stack size and priorities
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define HIGH_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char stack_one[STACK_SIZE];
char stack_two[STACK_SIZE];

void *thread_one_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread one logic
        thread_yield();
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread two logic
        thread_yield();
    }
    return NULL;
}

int main(void) {
    printf("Starting threads\n");

    thread_create(stack_one, sizeof(stack_one), LOW_PRIORITY, THREAD_CREATE_STACKTEST,
                  thread_one_func, NULL, "thread_one");
    thread_create(stack_two, sizeof(stack_two), HIGH_PRIORITY, THREAD_CREATE_STACKTEST,
                  thread_two_func, NULL, "thread_two");

    // Simulate priority change at runtime
    kernel_pid_t pid_one = thread_getpid_of((thread_t *)stack_one);
    kernel_pid_t pid_two = thread_getpid_of((thread_t *)stack_two);

    printf("Changing thread priorities\n");

    sched_change_priority(pid_one, HIGH_PRIORITY);
    sched_change_priority(pid_two, LOW_PRIORITY);

    while (1) {
        // Main loop logic
        thread_sleep();
    }

    return 0;
}