#include <thread.h>

// Ensure THREAD_PRIORITY_MAIN only defined once
#ifndef THREAD_PRIORITY_MAIN
#define THREAD_PRIORITY_MAIN  (5)
#endif

// Ensure THREAD_STACKSIZE_MAIN only defined once
#ifndef THREAD_STACKSIZE_MAIN
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#endif

// Ensure THREAD_CREATE_STACKTEST only defined once
#ifndef THREAD_CREATE_STACKTEST
#define THREAD_CREATE_STACKTEST
#endif

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void* thread_func(void* arg) {
    (void)arg;
    while (1) {
        printf("Thread running\n");
        thread_yield_higher();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid = thread_create(stack1, sizeof(stack1),
                                              THREAD_PRIORITY_MAIN,
                                              THREAD_CREATE_STACKTEST,
                                              thread_func, NULL,
                                              "thread1");

    kernel_pid_t thread2_pid = thread_create(stack2, sizeof(stack2),
                                              THREAD_PRIORITY_MAIN + 1,
                                              THREAD_CREATE_STACKTEST,
                                              thread_func, NULL,
                                              "thread2");

    printf("Threads started\n");

    // Change priority after threads have been started
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}