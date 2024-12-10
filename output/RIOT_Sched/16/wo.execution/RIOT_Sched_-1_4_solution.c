#include "time_units.h"
#include "timex.h"
#include "thread_arch.h"
#include "thread.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "sched.h"
#include "unistd.h"
#include "xtimer.h"
#include "paho_mqtt.h"
#include "mm8x5x_params.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "architecture.h"
#include "event/thread.h"

#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAX - 1)
#define STACKSIZE_MAIN         THREAD_STACKSIZE_MAIN

static char stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread running with priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;

    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, thread_handler, NULL, "new_thread");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    xtimer_sleep(5);

    if (sched_change_priority(thread_get(thread_pid), THREAD_PRIORITY_MAIN + 1)) {
        printf("Thread priority changed successfully\n");
    } else {
        printf("Failed to change thread priority\n");
    }

    return 0;
}