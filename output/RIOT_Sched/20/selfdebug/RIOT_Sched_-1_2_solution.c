#include <stdio.h>
#include <thread_config.h>

void *thread_function1(void *arg) {
    printf("Thread 1 is running with initial priority.\n");
    sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN - 2); // Lower the priority
    printf("Thread 1 changed its priority and yields.\n");
    thread_yield();
    return NULL;
}

void *thread_function2(void *arg) {
    printf("Thread 2 is running with initial priority.\n");
    sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN - 1); // Changer the priority
    printf("Thread 2 changed its priority and yields.\n");
    thread_yield();
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_function1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 1, 0, thread_function2, NULL, "thread2");

    while (1) {
        thread_yield(); // Main thread yields control to other threads
    }
    return 0;
}