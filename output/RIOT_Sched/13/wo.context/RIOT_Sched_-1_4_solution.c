#include <thread_arch.h>
#include <kernel_defines.h>

#define MY_THREAD_STACKSIZE THREAD_STACKSIZE_MAIN
#define MY_THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 1)

static char stack1[MY_THREAD_STACKSIZE];
static char stack2[MY_THREAD_STACKSIZE];

void *thread_func(void *arg) {
    while (1) {
        // Thread activity
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid;
    kernel_pid_t thread2_pid;

    thread1_pid = thread_create(stack1, MY_THREAD_STACKSIZE, MY_THREAD_PRIORITY,
                                THREAD_CREATE_SLEEPING | THREAD_CREATE_STACKTEST,
                                thread_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, MY_THREAD_STACKSIZE, MY_THREAD_PRIORITY,
                                THREAD_CREATE_SLEEPING | THREAD_CREATE_STACKTEST,
                                thread_func, NULL, "thread2");

    // Wake up threads
    thread_wakeup(thread1_pid);
    thread_wakeup(thread2_pid);

    // Change priority of thread1 at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);

    return 0;
}