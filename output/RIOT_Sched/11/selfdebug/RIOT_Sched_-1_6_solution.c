#include <thread.h>  // Ensure that the header providing necessary definitions is included

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN_DEFAULT)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];

void *thread1_handler(void *arg) {
    (void)arg;
    // Thread 1 functionality here
    return NULL;
}

void *thread2_handler(void *arg) {
    (void)arg;
    // Thread 2 functionality here
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                           THREAD_CREATE_STACKTEST, thread1_handler, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                           THREAD_CREATE_STACKTEST, thread2_handler, NULL, "thread2");

    // Change priority at runtime
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN - 4);

    return 0;
}