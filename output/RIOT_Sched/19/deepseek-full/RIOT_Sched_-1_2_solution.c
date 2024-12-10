
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

static void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running with priority %d\n", thread_get_priority(thread_getpid()));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

static void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running with priority %d\n", thread_get_priority(thread_getpid()));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    ztimer_sleep(ZTIMER_MSEC, 5000);

    printf("Changing priorities...\n");
    thread_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    thread_change_priority(pid2, THREAD_PRIORITY_MAIN - 1);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
