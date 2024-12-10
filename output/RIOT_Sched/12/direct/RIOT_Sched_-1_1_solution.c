#include <thread.h>
#include <stdio.h>

#define STACK_SIZE 1024

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread1_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY_MAIN - 1, CREATE_STACKTEST, thread1_handler, NULL, "thread_one");
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY_MAIN - 2, CREATE_STACKTEST, thread2_handler, NULL, "thread_two");

    // Simulate changing priorities at runtime
    xtimer_sleep(5);
    printf("Changing priority of thread 1\n");
    thread_set_priority(thread1_pid, PRIORITY_MAIN - 3);

    xtimer_sleep(5);
    printf("Changing priority of thread 2\n");
    thread_set_priority(thread2_pid, PRIORITY_MAIN - 4);

    while (1) {
        // Main thread loop
        xtimer_sleep(1);
    }

    return 0;
}