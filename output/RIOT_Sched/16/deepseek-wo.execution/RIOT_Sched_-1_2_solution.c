
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE 512
#define PRIORITY 5

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Thread 1 received message\n");
    }

    return NULL;
}

void *thread2(void *arg)
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
    // Create thread 1
    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1, NULL, "thread1");

    // Create thread 2
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2, NULL, "thread2");

    // Change priority of thread 1
    thread_change_priority(pid1, PRIORITY + 1);

    // Send a message to thread 1
    msg_t msg1;
    msg1.content.value = 0;
    msg_send(&msg1, pid1);

    // Send a message to thread 2
    msg_t msg2;
    msg2.content.value = 0;
    msg_send(&msg2, pid2);

    return 0;
}
