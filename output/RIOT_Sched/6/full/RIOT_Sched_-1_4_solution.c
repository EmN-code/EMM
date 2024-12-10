#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread_config.h>
#include <thread_arch.h>
#include <thread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sched.h>
#include <msg.h>

void *thread_func_1(void *arg) {
    while (1) {
        printf("Thread 1 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_func_2(void *arg) {
    while (1) {
        printf("Thread 2 running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    char stack1[KERNEL_CONF_STACKSIZE_DEFAULT];
    char stack2[KERNEL_CONF_STACKSIZE_DEFAULT];
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 
                         THREAD_CREATE_STACKTEST, thread_func_1, NULL, "thread 1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 
                         THREAD_CREATE_STACKTEST, thread_func_2, NULL, "thread 2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    /* Change priority of thread 1 */
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    
    /* Change priority of thread 2 */
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 3);

    while(1) {
        pause();  // Keep the main thread running
    }
    return 0;
}
