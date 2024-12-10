#include "kernel_types.h"
#include "thread.h"

#include <stdio.h>
#include <stdlib.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_OTHER   (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_OTHER    (THREAD_PRIORITY_MAIN - 2)

char thread_stack_other[THREAD_STACKSIZE_OTHER];

void *thread_handler(void *arg) {
    (void)arg;
    printf("Thread started with priority: %d\n", thread_getpid());
    while (1) {
        // Thread activity, e.g., blinking LED
    }
    return NULL;
}

int main(void) {
    puts("RIOT ESP32 Thread Management Example");

    kernel_pid_t thread_pid = thread_create(
        thread_stack_other, sizeof(thread_stack_other),
        THREAD_PRIORITY_OTHER, THREAD_CREATE_WOUT_YIELD,
        thread_handler, NULL, "other_thread");

    printf("Thread started with PID: %d\n", thread_pid);

    // Change the priority of the new thread
    sched_change_priority(thread_get(thread_pid), THREAD_PRIORITY_MAIN);

    printf("Changed thread priority to: %d\n", THREAD_PRIORITY_MAIN);

    return 0;
}