
#include "thread.h"
#include "xtimer.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        xtimer_sleep(1);
        LOG_INFO("Thread 1 running\n");
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        xtimer_sleep(2);
        LOG_INFO("Thread 2 running\n");
    }
    return NULL;
}

int main(void) {
    log_init();

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN,
                                      THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN,
                                      THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
