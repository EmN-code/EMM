
#include "thread.h"
#include "xtimer.h"
#include "log.h"

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid, thread2_pid;
char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        log_info("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        log_info("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    log_init();

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1, 0,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2, 0,
                                thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
