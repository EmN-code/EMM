
#include "thread.h"
#include "sched.h"
#include "stdio.h"
#include "unistd.h"

#define STACKSIZE 512

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, thread2_func, NULL, "thread2");

    sleep(5);

    printf("Changing priority of Thread 1 to lower\n");
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    sleep(5);

    printf("Changing priority of Thread 2 to higher\n");
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);

    while (1) {
        printf("Main thread is running\n");
        sleep(1);
    }

    return 0;
}
