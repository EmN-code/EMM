#include "thread.h"
#include "kernel.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack1[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];

void *thread1_function(void *arg) {
    (void)arg;
    puts("Thread 1 running");
    return NULL;
}

void *thread2_function(void *arg) {
    (void)arg;
    puts("Thread 2 running");
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(
        stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0,
        thread1_function, NULL, "thread1");

    kernel_pid_t thread2 = thread_create(
        stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0,
        thread2_function, NULL, "thread2");

    /* Assume interrupts are disabled and safe to change priority */
    if (thread1 >= 0) {
        sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);
    }

    if (thread2 >= 0) {
        sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 4);
    }
    
    return 0;
}