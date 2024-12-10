
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "stdio.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

char stack_high[STACKSIZE];
char stack_low[STACKSIZE];

kernel_pid_t high_priority_pid;
kernel_pid_t low_priority_pid;

void *high_priority_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);

        msg_t reply;
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);

        msg_t reply;
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void)
{
    high_priority_pid = thread_create(stack_high, sizeof(stack_high),
                                      THREAD_PRIORITY_MAIN - 1, 0,
                                      high_priority_thread, NULL, "high_priority");

    low_priority_pid = thread_create(stack_low, sizeof(stack_low),
                                     THREAD_PRIORITY_MAIN - 2, 0,
                                     low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.content.value = 100;

    msg_t reply;
    msg_send_receive(&msg, &reply, high_priority_pid);
    printf("Main thread received reply from high priority thread: %d\n", reply.content.value);

    msg.content.value = 200;
    msg_send_receive(&msg, &reply, low_priority_pid);
    printf("Main thread received reply from low priority thread: %d\n", reply.content.value);

    return 0;
}
