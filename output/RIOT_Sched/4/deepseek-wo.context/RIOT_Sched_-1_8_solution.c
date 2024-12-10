
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD1_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD2_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD1_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char thread1_stack[THREAD1_STACKSIZE];
char thread2_stack[THREAD2_STACKSIZE];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD1_PRIORITY,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD2_PRIORITY,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing thread priorities\n");
    sched_change_priority(thread_getpid_of(thread_get(thread1_pid)), THREAD2_PRIORITY);
    sched_change_priority(thread_getpid_of(thread_get(thread2_pid)), THREAD1_PRIORITY);

    return 0;
}
