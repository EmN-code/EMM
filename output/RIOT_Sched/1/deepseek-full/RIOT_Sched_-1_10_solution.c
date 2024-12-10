
#include "thread.h"
#include "sched.h"
#include "mutex.h"

#define STACKSIZE 1024

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread 1 logic
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        // Thread 2 logic
        thread_yield();
    }
    return NULL;
}

int main(void) {
    // Create the first thread
    pid1 = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");

    // Create the second thread
    pid2 = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, thread2_func, NULL, "thread2");

    // Change the priority of the first thread
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 3);

    // Change the priority of the second thread
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 4);

    return 0;
}
