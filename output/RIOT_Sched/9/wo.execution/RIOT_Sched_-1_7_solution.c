// The objective is to create a RIOT firmware for an ESP32 that creates two threads and changes their priority at runtime.
// Here we assume we are working with an ARM Cortex-M based RIOT system. The following example doesn't make use of ESP32-specific libraries,
// as the context is about generic RIOT threads and priority settings.

#include <xtimer.h>          // Timer functions
#include <thread.h>          // Thread control
#include <stdio.h>           // Standard input/output
#include <mutex.h>           // Mutual exclusion support

// Define stack sizes
#define STACKSIZE_MAIN    (THREAD_STACKSIZE_MAIN)
#define STACKSIZE_THREAD  (THREAD_STACKSIZE_MAIN / 2)

// Thread stack allocation
char thread_stack[2][STACKSIZE_THREAD];

// Function prototypes for thread tasks
void *thread_handler1(void *arg);
void *thread_handler2(void *arg);

int main(void) {
    kernel_pid_t first_thread, second_thread;
    // Create first thread
    first_thread = thread_create(thread_stack[0], sizeof(thread_stack[0]), THREAD_PRIORITY_MAIN - 1,
                                 THREAD_CREATE_STACKTEST, thread_handler1, NULL, "thread_1");

    // Create second thread
    second_thread = thread_create(thread_stack[1], sizeof(thread_stack[1]), THREAD_PRIORITY_MAIN - 1,
                                  THREAD_CREATE_STACKTEST, thread_handler2, NULL, "thread_2");

    // Wait for some time and then promote the first thread's priority to make it higher
    xtimer_sleep(5);
    sched_change_priority(thread_get(first_thread), THREAD_PRIORITY_MAIN - 2);

    // Wait some more and promote second thread's priority
    xtimer_sleep(5);
    sched_change_priority(thread_get(second_thread), THREAD_PRIORITY_MAIN - 3);

    // Loop forever
    while (1) {
        // Do nothing, just keep main running
        xtimer_sleep(1);
    }
    return 0;
}

void *thread_handler1(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 1 is running");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_handler2(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread 2 is running");
        xtimer_sleep(1);
    }
    return NULL;
}