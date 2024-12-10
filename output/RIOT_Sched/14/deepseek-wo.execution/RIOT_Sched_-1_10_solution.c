
#include "thread.h"
#include "sched.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, STACKSIZE, PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, STACKSIZE, PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    // Change priority of thread1 to low
    sched_change_priority(thread_get(thread1_pid), PRIORITY_LOW);

    // Change priority of thread2 to high
    sched_change_priority(thread_get(thread2_pid), PRIORITY_HIGH);

    return 0;
}
