#include <stdio.h>
#include <thread_arch.h>

// Function for thread_a
void *thread_a(void *arg) {
    while (1) {
        printf("Thread A is running with priority: %d\n", thread_get_priority(thread_getpid()));
        thread_yield();
    }
    return NULL;
}

// Function for thread_b
void *thread_b(void *arg) {
    while (1) {
        printf("Thread B is running with priority: %d\n", thread_get_priority(thread_getpid()));
        thread_yield();
    }
    return NULL;
}

int main(void) {
    // Thread identifiers and stack allocation
    char stack_thread_a[THREAD_STACKSIZE_MAIN];
    char stack_thread_b[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread_a_pid;
    kernel_pid_t thread_b_pid;

    // Create thread A
    thread_a_pid = thread_create(
        stack_thread_a, sizeof(stack_thread_a),
        THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST,
        thread_a, NULL, "thread_a");

    // Create thread B
    thread_b_pid = thread_create(
        stack_thread_b, sizeof(stack_thread_b),
        THREAD_PRIORITY_MAIN + 2, THREAD_CREATE_STACKTEST,
        thread_b, NULL, "thread_b");

    printf("Threads created. Now switching priorities.\n");

    // Change the priority of threads
    if (thread_get_status(thread_a_pid) == THREAD_STATUS_RUNNING) {
        printf("Switch priority of Thread A to main priority.\n");
        sched_change_priority(thread_get(thread_a_pid), THREAD_PRIORITY_MAIN);
    }

    if (thread_get_status(thread_b_pid) == THREAD_STATUS_RUNNING) {
        printf("Switch priority of Thread B to main priority + 3.\n");
        sched_change_priority(thread_get(thread_b_pid), THREAD_PRIORITY_MAIN + 3);
    }

    while(1) {
        // Main thread can perform other tasks if needed
    }

    return 0;
}