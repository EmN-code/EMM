#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <kernel.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND   (THREAD_PRIORITY_MAIN - 2)

char stack1[THREAD_STACKSIZE_MAIN];
char stack2[THREAD_STACKSIZE_MAIN];
kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread %d is running with priority %d\n", thread_getpid(), thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    puts("Starting two threads and changing their priorities at runtime.");
    
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN, 0, thread_handler, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_SECOND, 0, thread_handler, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priorities of threads.\n");
    sched_change_priority(thread1_pid, THREAD_PRIORITY_SECOND);
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN);

    while (1) {
        xtimer_sleep(10);
    }
    return 0;
}