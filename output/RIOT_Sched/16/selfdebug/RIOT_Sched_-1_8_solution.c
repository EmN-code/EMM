#include "thread_arch.h"
#include "kernel_defines.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char thread1_stack[STACKSIZE];
char thread2_stack[STACKSIZE];

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        /* Thread logic */
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                           THREAD_PRIORITY_MAIN - 1,
                                           THREAD_CREATE_STACKTEST,
                                           thread_handler,
                                           NULL, "thread1");

    kernel_pid_t thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                           THREAD_PRIORITY_MAIN - 1,
                                           THREAD_CREATE_STACKTEST,
                                           thread_handler,
                                           NULL, "thread2");

    /* Adjust priority at runtime */
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 3);

    return 0;
}