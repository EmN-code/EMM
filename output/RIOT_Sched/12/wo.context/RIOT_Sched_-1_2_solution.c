#include <thread_config.h>

void *thread_func(void *arg) {
    // Thread function logic goes here.
    return NULL;
}

int main(void) {
    // Create two threads with known storage sizes
    char thread_stack_1[THREAD_STACKSIZE_DEFAULT];
    char thread_stack_2[THREAD_STACKSIZE_DEFAULT];

    thread_t my_thread1;
    thread_t my_thread2;

    kernel_pid_t thread_pid1 = thread_create(thread_stack_1, sizeof(thread_stack_1), THREAD_PRIORITY_MAIN - 1,
                                             THREAD_CREATE_STACKTEST, thread_func, NULL, "thread1");
    kernel_pid_t thread_pid2 = thread_create(thread_stack_2, sizeof(thread_stack_2), THREAD_PRIORITY_MAIN - 2,
                                             THREAD_CREATE_STACKTEST, thread_func, NULL, "thread2");

    // Change the priority of the threads
    sched_change_priority(thread_get(thread_pid1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread_pid2), THREAD_PRIORITY_MAIN - 4);

    while (1) {
        // Main thread loop logic goes here.
    }

    return 0;
}