
#include <stdio.h>
#include <thread.h>
#include <sched.h>

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_LOW, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    if (pid1 < 0) {
        printf("Error creating thread 1\n");
        return 1;
    }

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");
    if (pid2 < 0) {
        printf("Error creating thread 2\n");
        return 1;
    }

    // Change the priority of the first thread to high
    sched_change_priority(pid1, PRIORITY_HIGH);
    printf("Changed priority of thread 1 to high\n");

    // Change the priority of the second thread to low
    sched_change_priority(pid2, PRIORITY_LOW);
    printf("Changed priority of thread 2 to low\n");

    return 0;
}
