#include <thread_arch.h>
#include <thread_config.h>
#include <kernel_defines.h>
#include <stdio.h>

char stack_one[THREAD_STACKSIZE_DEFAULT];
char stack_two[THREAD_STACKSIZE_DEFAULT];

void *thread_one_func(void *arg) {
    printf("Thread one is running\n");
    while (1) {
        // Simulate work
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    printf("Thread two is running\n");
    while (1) {
        // Simulate work
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(
        stack_one,
        sizeof(stack_one),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread_one_func,
        NULL,
        "thread_one"
    );

    kernel_pid_t thread_two_pid = thread_create(
        stack_two,
        sizeof(stack_two),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread_two_func,
        NULL,
        "thread_two"
    );

    // Change thread priority
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN);

    printf("Threads have been started and priorities changed.\n");

    while (1) {
        // Main thread can perform other tasks or loop indefinitely
    }

    return 0;
}