#include "xtimer.h"
#include "timex.h"
#include "thread_arch.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "unistd.h"
#include "ztimer.h"
#include "timex.h"
#include "msg.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "mbox.h"
#include "thread_config.h"
#include "sched.h"
#include "architecture.h"

#define THREAD_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (5)
#define THREAD_PRIORITY_CHANGING (6)

char stack1[THREAD_STACK_SIZE];
char stack2[THREAD_STACK_SIZE];

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        puts("Thread running");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0,
                                thread_handler, NULL, "thread1");

    if (thread1_pid <= KERNEL_PID_UNDEF) {
        puts("Thread 1 creation failed");
        return 1;
    }

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, 0,
                                thread_handler, NULL, "thread2");

    if (thread2_pid <= KERNEL_PID_UNDEF) {
        puts("Thread 2 creation failed");
        return 1;
    }

    // Change the priority of thread2 at runtime
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_CHANGING);

    return 0;
}