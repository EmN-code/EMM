#include <thread.h>
#include <stdio.h>

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread running with priority: %d\n", thread_get_priority(thread_getpid()));
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, NULL, "thread2");

    while (1) {
        puts("Main loop running.");
        thread_sleep(); // Simulate doing some work

        // Dynamically change thread priorities
        thread_set_priority(thread1, THREAD_PRIORITY_MAIN - 3);
        thread_set_priority(thread2, THREAD_PRIORITY_MAIN - 1);

        printf("Updated priorities: thread1=%d, thread2=%d\n",
               thread_get_priority(thread1),
               thread_get_priority(thread2));
    }

    return 0;
}