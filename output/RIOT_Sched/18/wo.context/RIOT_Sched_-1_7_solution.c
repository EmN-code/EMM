#include <thread.h>  // For thread creation and management
#include <sched.h>   // For scheduling and changing thread priority
#include <xtimer.h>  // For using xtimer_sleep()
#include <stdio.h>   // For printf

#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_DEFAULT (1024)

char stack_one[THREAD_STACKSIZE_DEFAULT];
char stack_two[THREAD_STACKSIZE_DEFAULT];

void *thread_func_one(void *arg) {
    (void)arg; // Suppress unused variable warning
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    (void)arg;  // Suppress unused variable warning
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_func_one, NULL, "thread_1");
    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST, thread_func_two, NULL, "thread_2");

    // Change priority of thread_one during runtime
    xtimer_sleep(5);  // Wait for some time before changing the priority
    sched_change_priority(pid_one, THREAD_PRIORITY_MAIN - 3);

    // Let main run forever to keep the program alive
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}