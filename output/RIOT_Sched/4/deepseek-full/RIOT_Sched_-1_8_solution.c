
#include "thread.h"
#include "sched.h"
#include "msg.h"

#define THREAD_STACKSIZE_DEFAULT 512

static char thread1_stack[THREAD_STACKSIZE_DEFAULT];
static char thread2_stack[THREAD_STACKSIZE_DEFAULT];

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD_PRIORITY_MAIN + 1, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD_PRIORITY_MAIN + 2, 0, thread2_func, NULL, "thread2");

    // Change priority of thread1 to a lower priority
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN + 3);

    // Change priority of thread2 to a higher priority
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN);

    while (1) {
        thread_yield();
    }

    return 0;
}
