#include <thread.h>
#include <stdbool.h>
#include <stdio.h>

char thread_one_stack[THREAD_STACKSIZE_DEFAULT];
char thread_two_stack[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    printf("Thread one starting with priority 5\n");
    // Simulate some work
    for (int i = 0; i < 5; i++) {
        printf("Thread one running iteration %d\n", i);
        thread_yield();
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    printf("Thread two starting with priority 3\n");
    // Simulate some work
    for (int i = 0; i < 5; i++) {
        printf("Thread two running iteration %d\n", i);
        thread_yield();
    }
    return NULL;
}

int main(void) {
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack), 5, 
                      THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack), 3, 
                      THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    // Change priority at runtime
    printf("Changing priorities at runtime\n");

    // Change thread one priority to 2
    sched_change_priority(thread_one_pid, 2);

    // Change thread two priority to 6
    sched_change_priority(thread_two_pid, 6);

    while (1) {
        // Main thread work can be added here
        thread_yield();
    }
    return 0;
}