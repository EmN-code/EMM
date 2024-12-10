#include <thread.h>     // Includes the thread handling functions
#include <sched.h>      // Includes scheduling functions for manipulating thread priorities

#define STACKSIZE_MAIN (THREAD_STACKSIZE_MAIN)
#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

char thread_stack_1[STACKSIZE_MAIN];
char thread_stack_2[STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    (void)arg;  // Just to silence unused parameter warning
    return NULL;
}

int main(void) {
    kernel_pid_t tid1 = thread_create(thread_stack_1, sizeof(thread_stack_1),
                                      PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                      thread_handler, NULL, "thread 1");
    if (tid1 < 0) {
        printf("Error creating thread 1\n");
        return 1;
    }

    kernel_pid_t tid2 = thread_create(thread_stack_2, sizeof(thread_stack_2),
                                      PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                      thread_handler, NULL, "thread 2");
    if (tid2 < 0) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change priority at runtime for demonstration
    sched_change_priority(thread_get(tid1), PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(tid2), PRIORITY_MAIN - 4);

    return 0;
}