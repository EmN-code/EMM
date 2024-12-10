#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "architecture.h"
#include "time_units.h"

#define STACK_SIZE_THREAD_ONE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define STACK_SIZE_THREAD_TWO (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

void *thread_logic(void *arg) {
    (void)arg;
    while (1) {
        // Perform some operation
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack_one[STACK_SIZE_THREAD_ONE];
    char stack_two[STACK_SIZE_THREAD_TWO];

    kernel_pid_t thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                                                 THREAD_CREATE_STACKTEST, thread_logic, NULL, "Thread One");
    kernel_pid_t thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2,
                                                 THREAD_CREATE_STACKTEST, thread_logic, NULL, "Thread Two");

    if (thread_one_pid == KERNEL_PID_UNDEF || thread_two_pid == KERNEL_PID_UNDEF) {
        // Handle error
    }

    // Change priority of threads at runtime
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
