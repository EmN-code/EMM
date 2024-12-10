#include <thread.h>
#include <sched.h>
#include <kernel_types.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_handler(void *arg) {
    // Thread specific code goes here
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                               CREATE_STACKTEST, thread_handler, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_MAIN - 2,
                               CREATE_STACKTEST, thread_handler, NULL, "thread2");

    // Change priority of an existing thread at runtime
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);

    return 0;
}