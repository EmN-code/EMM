
#include "evtimer_msg.h"
#include "event/thread.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "ztimer.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *thread1_func(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Thread 1 received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, thread_getpid());
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    (void)pid1;
    (void)pid2;

    return 0;
}
