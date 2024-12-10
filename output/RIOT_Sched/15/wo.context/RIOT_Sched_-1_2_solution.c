#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

// Define stack size and priorities for threads
#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY_1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2 (THREAD_PRIORITY_MAIN - 2)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    puts("Starting thread demo\n");

    // Create thread 1
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_1, 0, thread1, NULL, "thread1");

    // Create thread 2
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_2, 0, thread2, NULL, "thread2");

    // Change priorities at runtime for demonstration
    while (1) {
        // Simulating changing priorities
        xtimer_sleep(10);
        sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);
        printf("Changed priority of thread 1\n");
        xtimer_sleep(10);
        sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);
        printf("Changed priority of thread 2\n");
    }

    return 0;
}