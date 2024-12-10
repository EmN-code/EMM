// Description: This code demonstrates how to create and run two threads on an ESP32 using RIOT OS, and then change their priority at runtime. The two threads perform different tasks and have their priorities modified to show dynamic priority management.
#include "kernel_defines.h"
#include "kernel.h"
#include "stdio.h"

#define THREAD_STACKSIZE_DEFAULT (8192)
#define THREAD_PRIORITY_MAIN (0)

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    printf("Starting threads\n");

    // Create thread 1
    thread1_pid = thread_create(
        stack1, sizeof(stack1),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread1_func, NULL, "thread1");
    
    if (thread1_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    // Create thread 2
    thread2_pid = thread_create(
        stack2, sizeof(stack2),
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        thread2_func, NULL, "thread2");

    if (thread2_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    // Change priorities at runtime
    xtimer_sleep(5); // Wait for 5 seconds before changing priorities
    printf("Changing priorities\n");
    thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);
    thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    printf("Priority change complete\n");

    // Prevent the main function from exiting
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}