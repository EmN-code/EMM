#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sched.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define THREAD_STACKSIZE_DEFAULT (1024U)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_handler(void *arg) {
    (void)arg;
    
    while (1) {
        printf("Thread %p running.\n", (void *)thread_getpid());
        xtimer_sleep(1);
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    uint8_t priority;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create thread 1\n");
        return 1;
    }

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create thread 2\n");
        return 1;
    }

    // Change priorities at runtime
    priority = THREAD_PRIORITY_MAIN - 3;
    sched_change_priority(thread_get(pid1), priority);

    priority = THREAD_PRIORITY_MAIN - 5;
    sched_change_priority(thread_get(pid2), priority);

    while (1) {
        printf("Main thread running.\n");
        xtimer_sleep(1);
    }

    return 0;
}