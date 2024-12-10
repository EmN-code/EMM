
#include "thread.h"
#include "sched.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        thread_sleep(1000000); // Sleep for 1 second
        log_info("Thread 1 is running\n");
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        thread_sleep(1000000); // Sleep for 1 second
        log_info("Thread 2 is running\n");
    }
    return NULL;
}

int main(void)
{
    log_init();

    thread1_pid = thread_create(
        thread_stack(thread1_stack),
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    thread2_pid = thread_create(
        thread_stack(thread2_stack),
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    thread_sleep(5000000); // Sleep for 5 seconds

    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 2);

    return 0;
}
