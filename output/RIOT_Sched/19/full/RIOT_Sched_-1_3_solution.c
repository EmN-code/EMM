#include "unistd.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "sched.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "architecture.h"
#include "event/thread.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN)
#define THREAD_PRIORITY_SECOND  (THREAD_PRIORITY_MAIN + 1)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

static void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread is running with priority: %d\n", thread_get_priority(thread_get(thread_getpid())));
        usleep_time_units(1);
    }
    return NULL;
}

int main(void) {
    thread1_pid = thread_create(stack1, sizeof(stack1),
                                THREAD_PRIORITY_MAIN, 0,
                                thread_handler, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2),
                                THREAD_PRIORITY_SECOND, 0,
                                thread_handler, NULL, "thread2");

    printf("Threads started\n");

    // Change priority of the first thread
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_SECOND);

    return 0;
}
