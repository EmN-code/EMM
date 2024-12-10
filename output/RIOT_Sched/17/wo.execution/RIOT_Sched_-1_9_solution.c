#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <msg.h>

#define TASK_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define TASK_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

static char stack1[TASK_STACKSIZE];
static char stack2[TASK_STACKSIZE];
kernel_pid_t thread1_pid, thread2_pid;

void *thread_func(void *arg) {
    (void)arg;
    /* Thread code here */
    while (1) {
        // do some work
    }
    return NULL;
}

int main(void) {
    /* Create two threads */
    thread1_pid = thread_create(stack1, sizeof(stack1),
                                TASK_PRIORITY_MAIN + 1, 0,
                                thread_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2),
                                TASK_PRIORITY_MAIN + 2, 0,
                                thread_func, NULL, "thread2");

    /* Change priority of the first thread at runtime */
    thread_t *thread1 = thread_get(thread1_pid);
    if (thread1 != NULL) {
        sched_change_priority(thread1, TASK_PRIORITY_MAIN + 3);
    }

    /* Main thread work here if needed */
    while (1) {
        // main work loop
    }

    return 0;
}