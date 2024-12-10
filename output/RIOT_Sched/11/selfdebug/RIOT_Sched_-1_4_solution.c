#include "thread.h"

#define MY_THREAD_PRIORITY_1  (THREAD_PRIORITY_MAIN - 1)
#define MY_THREAD_PRIORITY_2  (THREAD_PRIORITY_MAIN - 2)
#define MY_THREAD_STACKSIZE   (THREAD_STACKSIZE_DEFAULT)

static char stack1[MY_THREAD_STACKSIZE];
static char stack2[MY_THREAD_STACKSIZE];

kernel_pid_t thread1;
kernel_pid_t thread2;

void *thread_handler(void *arg) {
    (void)arg; // Suppress unused variable warning
    while (1) {
        // Execute some task repeatedly
    }
    return NULL;
}

int main(void) {
    // Create the first thread
    thread1 = thread_create(stack1, MY_THREAD_STACKSIZE, MY_THREAD_PRIORITY_1,
                            THREAD_CREATE_STACKTEST, thread_handler, NULL,"thread1");

    // Create the second thread
    thread2 = thread_create(stack2, MY_THREAD_STACKSIZE, MY_THREAD_PRIORITY_2,
                            THREAD_CREATE_STACKTEST, thread_handler, NULL,"thread2");

    // At some later point in time, change the priority of thread1
    sched_set_priority(thread1, THREAD_PRIORITY_MAIN - 3);

    // Continue with the rest of the program
    return 0;
}