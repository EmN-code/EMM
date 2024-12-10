#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "time_units.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "sched.h"
#include "kernel_defines.h"

#define STACKSIZE_THREAD1 (THREAD_STACKSIZE_DEFAULT)
#define STACKSIZE_THREAD2 (THREAD_STACKSIZE_DEFAULT)

kernel_pid_t pid_thread1;
kernel_pid_t pid_thread2;

void *thread1(void *arg) {
    (void)arg;
    while (1) {
        // Simulated work by thread1
    }
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    while (1) {
        // Simulated work by thread2
    }
    return NULL;
}

int main(void) {
    /* Creating thread1 with high priority */
    char thread1_stack[STACKSIZE_THREAD1];
    pid_thread1 = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1, 0,
                                thread1, NULL, "thread1");

    /* Creating thread2 with low priority */
    char thread2_stack[STACKSIZE_THREAD2];
    pid_thread2 = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN + 1, 0,
                                thread2, NULL, "thread2");

    /* Change priority of thread2 to high */
    sched_change_priority(thread_get(pid_thread2), THREAD_PRIORITY_MAIN - 2);

    return 0;
}