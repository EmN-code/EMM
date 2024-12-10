
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Thread 1 received message\n");
        xtimer_sleep(1);
    }

    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Thread 2 received message\n");
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    msg_t msg_queue[16];
    msg_init_queue(msg_queue, 16);

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread 1 to LOW\n");
    sched_change_priority(pid1, PRIORITY_LOW);

    xtimer_sleep(5);

    printf("Changing priority of thread 2 to HIGH\n");
    sched_change_priority(pid2, PRIORITY_HIGH);

    return 0;
}
