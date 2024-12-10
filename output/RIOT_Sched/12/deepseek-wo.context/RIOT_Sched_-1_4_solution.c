
#include "thread.h"
#include "sched.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

char thread_stack1[THREAD_STACKSIZE_MAIN];
char thread_stack2[THREAD_STACKSIZE_MAIN];

void *thread_func1(void *arg) {
    (void)arg;
    while (1) {
        LOG_INFO("Thread 1 running\n");
        thread_sleep(1000000);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    while (1) {
        LOG_INFO("Thread 2 running\n");
        thread_sleep(1000000);
    }
    return NULL;
}

int main(void) {
    log_init();

    kernel_pid_t pid1 = thread_create(thread_stack1, sizeof(thread_stack1),
                                     THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                     thread_func1, NULL, "thread1");

    kernel_pid_t pid2 = thread_create(thread_stack2, sizeof(thread_stack2),
                                     THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                     thread_func2, NULL, "thread2");

    thread_sleep(5000000);

    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
