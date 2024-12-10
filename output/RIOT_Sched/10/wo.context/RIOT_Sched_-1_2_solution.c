#include <thread_config.h>
#include <sched.h>
#include <stdio.h>

#define THREAD_STACKSIZE_DEFAULT    (1024)
#define THREAD_PRIORITY_MAIN        (0)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_handler_1(void *arg) {
    printf("Thread 1 running with initial priority\n");
    return NULL;
}

void *thread_handler_2(void *arg) {
    printf("Thread 2 running with initial priority\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_handler_1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, thread_handler_2, NULL, "thread2");

    printf("Threads created\n");

    // Change priorities
    printf("Changing priorities\n");
    sched_change_priority(thread_get(thread1), THREAD_PRIORITY_MAIN + 1);
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN + 2);

    return 0;
}
