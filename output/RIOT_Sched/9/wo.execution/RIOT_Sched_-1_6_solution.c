#include <ztimer.h>
#include <xtimer.h>
#include <thread_arch.h>
#include <thread_config.h>
#include <thread.h>
#include <time_units.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

// Dummy thread function
void *thread_func(void *arg) {
    while (1) {
        // To simulate work in the thread
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

int main(void) {
    // Create two threads
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0,
                                      thread_func, NULL, "thread1");
    if (pid1 < 0) {
        puts("Error creating thread 1\n");
    }

    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0,
                                      thread_func, NULL, "thread2");
    if (pid2 < 0) {
        puts("Error creating thread 2\n");
    }

    // Change the priority of the first thread
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);
    // Change the priority of the second thread
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}