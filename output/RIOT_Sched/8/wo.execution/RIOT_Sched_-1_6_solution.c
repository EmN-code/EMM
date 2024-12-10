#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "thread.h"
#include "sched.h"
#include "stdio.h"

// Define stack size for threads
#define STACKSIZE_THREAD     (1024)

// Declare stack memory for the threads
char stack1[STACKSIZE_THREAD];
char stack2[STACKSIZE_THREAD];

// Thread entry function
void *thread_function(void *arg) {
    (void)arg;  // Unused parameter
    while(1) {
        // Print thread ID and simulate work
        printf("Thread [PID: %d] is running\n", thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    puts("RIOT Threads Example");

    // Create threads
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                          THREAD_CREATE_STACKTEST, thread_function, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                          THREAD_CREATE_STACKTEST, thread_function, NULL, "thread2");

    // Change thread priority
    ztimer_sleep(ZTIMER_MSEC, 5000);  // Let the threads run for a while
    printf("Changing priority of thread 2\n");
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 3);

    // Main thread loops forever
    while (1) {
        printf("Main thread running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}