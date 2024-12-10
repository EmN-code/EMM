
#include "thread.h"
#include "sched.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE_DEFAULT (512)

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        // Thread 1 logic
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        // Thread 2 logic
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1,
                         THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 2,
                         THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Change the priority of the first thread at runtime
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN + 3);

    // Change the priority of the second thread at runtime
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN + 4);

    return 0;
}
