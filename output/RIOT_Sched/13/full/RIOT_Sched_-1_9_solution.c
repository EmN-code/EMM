/*
 * Example program for RIOT OS on ESP32 with threads
 * and priority management
 */

#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

// Thread stack sizes
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECONDARY (THREAD_PRIORITY_MAIN - 2)

char stack_thread1[THREAD_STACKSIZE_MAIN];
char stack_thread2[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void)arg;
    for (int i = 0; i < 10; ++i) {
        printf("Thread %lu running with priority %u\n", (unsigned long)thread_getpid(), thread_get_priority(thread_getpid()));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid;
    kernel_pid_t thread2_pid;

    // Create two threads
    thread1_pid = thread_create(stack_thread1, sizeof(stack_thread1),
                                THREAD_PRIORITY_MAIN,
                                CREATE_STACKTEST,
                                thread_handler, NULL, "thread1");

    thread2_pid = thread_create(stack_thread2, sizeof(stack_thread2),
                                THREAD_PRIORITY_SECONDARY,
                                CREATE_STACKTEST,
                                thread_handler, NULL, "thread2");

    // Run the threads with initial priority
    printf("Threads created, thread1 PID: %d, thread2 PID: %d\n", thread1_pid, thread2_pid);

    // Change the priority of the threads at runtime
    thread_set_priority(thread1_pid, THREAD_PRIORITY_SECONDARY);
    thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN);

    // Yield to new orders
    thread_yield();

    return 0;
}