
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (512)  // Define the stack size explicitly

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    char thread1_stack[THREAD_STACKSIZE_DEFAULT];
    char thread2_stack[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    // Change priority of thread1 at runtime
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    return 0;
}
