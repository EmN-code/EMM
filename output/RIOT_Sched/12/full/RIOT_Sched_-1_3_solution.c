#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <thread_config.h>
#include <thread.h>
#include <time_units.h>
#include <sched.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#include <stdio.h>
#include <stdint.h>

char stack_one[THREAD_STACKSIZE_DEFAULT];
char stack_two[THREAD_STACKSIZE_DEFAULT];

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                                                THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread_one");
    if (thread_one_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread one\n");
        return 1;
    }

    kernel_pid_t thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2,
                                                THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread_two");
    if (thread_two_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread two\n");
        return 1;
    }

    printf("Changing priority of thread one\n");
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN - 3);

    printf("Changing priority of thread two\n");
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 4);

    return 0;
}