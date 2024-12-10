#include "ztimer.h"
#include "xtimer.h"
#include "thread_config.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "sched.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];
kernel_pid_t pid1;
kernel_pid_t pid2;

void *thread_func(void *arg) {
    (void)arg;
    while(1) {
        // Simulate doing some work
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

int main(void) {
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, NULL, "thread2");

    // Sleep for 3 seconds before changing priority to simulate runtime change
    ztimer_sleep(ZTIMER_SEC, 3);

    printf("Changing priorities of threads at runtime\n");

    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);

    return 0;
}