
#include <stdio.h>
#include <unistd.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>

#define STACKSIZE (1024)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    printf("Threads created: %d, %d\n", pid1, pid2);

    xtimer_sleep(5);

    printf("Changing priorities\n");
    thread_set_priority(pid1, PRIORITY_LOW);
    thread_set_priority(pid2, PRIORITY_HIGH);

    return 0;
}
