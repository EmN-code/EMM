
#include "thread.h"
#include "msg.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

kernel_pid_t thread1_pid, thread2_pid;
char thread1_stack[THREAD_STACKSIZE_MAIN];
char thread2_stack[THREAD_STACKSIZE_MAIN];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        thread_yield();
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
                                THREAD_PRIORITY_MAIN - 1, 0,
                                thread2_func, NULL, "thread2");

    // Change priority of thread1 at runtime
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);

    return 0;
}
