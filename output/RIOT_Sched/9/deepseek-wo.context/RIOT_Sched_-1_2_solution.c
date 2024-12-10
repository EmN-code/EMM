
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1_func(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Thread 1 received message\n");
    }

    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Thread 2 received message\n");
    }

    return NULL;
}

int main(void)
{
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread1 to LOW and thread2 to HIGH\n");
    sched_change_priority(pid1, PRIORITY_LOW);
    sched_change_priority(pid2, PRIORITY_HIGH);

    xtimer_sleep(5);

    printf("Changing priority of thread1 back to HIGH and thread2 back to LOW\n");
    sched_change_priority(pid1, PRIORITY_HIGH);
    sched_change_priority(pid2, PRIORITY_LOW);

    return 0;
}
