#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "msg.h"
#include "mbox.h"
#include "sched.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "thread_arch.h"
#include "thread.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_IDLE + 1)
#define THREAD_PRIORITY_WORKER  (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_STACKSIZE_WORKER (THREAD_STACKSIZE_DEFAULT)

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        puts("Working thread executing.");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack_main[THREAD_STACKSIZE_MAIN];
    char stack_worker[THREAD_STACKSIZE_WORKER];

    kernel_pid_t main_pid = thread_create(stack_main, sizeof(stack_main),
                                          THREAD_PRIORITY_MAIN, 0,
                                          thread_handler, NULL, "main_thread");

    kernel_pid_t worker_pid = thread_create(stack_worker, sizeof(stack_worker),
                                            THREAD_PRIORITY_WORKER, 0,
                                            thread_handler, NULL, "worker_thread");

    // Change priority runtime
    printf("Changing priority of worker thread\n");
    int new_priority = THREAD_PRIORITY_MAIN + 2;
    sched_change_priority(thread_get(worker_pid), new_priority);

    // Check priority change
    xtimer_sleep(2);
    printf("Current worker thread priority: %d\n", (int)thread_get_priority(thread_get(worker_pid)));

    // Main thread operations
    while (1) {
        puts("Main thread executing.");
        xtimer_sleep(1);
    }

    return 0;
}