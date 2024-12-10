
#include "thread.h"
#include "log.h"
#include "xtimer.h"

#define THREAD1_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD2_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

#define THREAD1_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIORITY (THREAD_PRIORITY_MAIN - 2)

char thread1_stack[THREAD1_STACKSIZE];
char thread2_stack[THREAD2_STACKSIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void* thread1_func(void* arg) {
    (void)arg;
    while (1) {
        log_info("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void* thread2_func(void* arg) {
    (void)arg;
    while (1) {
        log_info("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    log_init();

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD1_PRIORITY,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD2_PRIORITY,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    log_info("Changing thread priorities\n");
    sched_change_priority(thread1_pid, THREAD2_PRIORITY);
    sched_change_priority(thread2_pid, THREAD1_PRIORITY);

    return 0;
}
