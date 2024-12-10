#include <unistd.h>
#include <time_units.h>
#include <thread_arch.h>
#include <thread.h>
#include <thread_config.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sched.h>
#include <MQTTClient.h>

void *thread_func1(void *arg) {
    printf("Thread 1 is running with initial priority.\n");
    sleep(1); // Simulate some work
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 is running with initial priority.\n");
    sleep(1); // Simulate some work
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread1_pid, thread2_pid;

    // Create two threads
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func1, NULL, "Thread 1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread_func2, NULL, "Thread 2");

    if (thread1_pid == 0 || thread2_pid == 0) {
        printf("Error: Failed to create threads.\n");
        return 1;
    }

    printf("Threads created with initial priority.\n");

    // Change priority at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);

    printf("Thread priorities changed.\n");

    return 0;
}