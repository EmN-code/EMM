
#include <evtimer_msg.h>
#include <timex.h>
#include <architecture.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <xtimer.h>
#include <mma8x5x_regs.h>
#include <thread.h>
#include <ztimer.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <sched.h>
#include <net/gcoap.h>
#include <event/periodic_callback.h>

#define STACKSIZE 512

char stack0[STACKSIZE];
char stack1[STACKSIZE];

void *thread_func0(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 0 running at priority %d\n", thread_get_priority(thread_get_active()));
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid0 = thread_create(stack0, sizeof(stack0), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread_func0, NULL, "thread0");
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 2,
                                      THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    xtimer_sleep(5);

    printf("Changing priority of thread 0 to MAIN - 2\n");
    sched_change_priority(thread_get(pid0), THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(5);

    printf("Changing priority of thread 1 to MAIN - 1\n");
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
