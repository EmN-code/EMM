#include "ztimer.h"
#include "xtimer.h"
#include "thread_config.h"
#include "thread.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define THREAD_ONE_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_TWO_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_ONE_PRIO      (THREAD_PRIORITY_MAIN - 1)
#define THREAD_TWO_PRIO      (THREAD_PRIORITY_MAIN - 2)

char thread_one_stack[THREAD_ONE_STACKSIZE];
char thread_two_stack[THREAD_TWO_STACKSIZE];

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        // Simulate thread work
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        // Simulate thread work
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                                THREAD_ONE_PRIO, 0, thread_one, NULL, "thread_one");
    kernel_pid_t thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                                THREAD_TWO_PRIO, 0, thread_two, NULL, "thread_two");

    // Sleep for a while before changing priority
    ztimer_sleep(ZTIMER_SEC, 5);

    printf("Changing priority of thread one\n");
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN - 2);

    printf("Changing priority of thread two\n");
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 1);

    while (1) {
        // Main thread doing its work
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}