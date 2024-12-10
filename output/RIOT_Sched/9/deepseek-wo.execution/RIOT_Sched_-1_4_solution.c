
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <thread.h>
#include <sched.h>
#include <msg.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <time_units.h>

#define THREAD_STACKSIZE_DEFAULT (512)

void *thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN - 1, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN - 2, 0, thread_func_2, NULL, "thread_2");

    printf("Threads created with PIDs: %d and %d\n", pid_1, pid_2);

    // Change priority of thread 1 to higher
    sched_change_priority(thread_get(pid_1), THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread 1 to higher\n");

    // Change priority of thread 2 to lower
    sched_change_priority(thread_get(pid_2), THREAD_PRIORITY_MAIN - 1);
    printf("Changed priority of thread 2 to lower\n");

    return 0;
}
