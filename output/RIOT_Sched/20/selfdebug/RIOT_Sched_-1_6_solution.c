#include "thread.h"
#include "sched.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2 (THREAD_PRIORITY_MAIN - 2)

char thread_stack1[STACKSIZE];
char thread_stack2[STACKSIZE];

void *thread_handler(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    while (1) {
        // Simulate thread operations
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;

    // Creating first thread
    pid1 = thread_create(thread_stack1, sizeof(thread_stack1),
                         THREAD_PRIORITY_1, THREAD_CREATE_STACKTEST,
                         thread_handler, NULL, "thread1");

    // Creating second thread
    pid2 = thread_create(thread_stack2, sizeof(thread_stack2),
                         THREAD_PRIORITY_2, THREAD_CREATE_STACKTEST,
                         thread_handler, NULL, "thread2");

    // Run loop to prevent program from terminating
    while (1) {
        // Simulate changes in priorities at runtime
        sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN);
    }

    return 0;
}