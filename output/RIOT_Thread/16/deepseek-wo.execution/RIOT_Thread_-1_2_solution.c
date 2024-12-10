
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "xtimer.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

char thread_stack_buffer_high[STACKSIZE];
char thread_stack_buffer_low[STACKSIZE];

kernel_pid_t high_priority_pid;
kernel_pid_t low_priority_pid;

msg_t msg_queue_high[MSG_QUEUE_SIZE];
msg_t msg_queue_low[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);
    msg_init_queue(msg_queue_low, MSG_QUEUE_SIZE);

    high_priority_pid = thread_create(thread_stack_buffer_high,
                                      STACKSIZE,
                                      THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST,
                                      high_priority_thread,
                                      NULL,
                                      "high_priority_thread");

    low_priority_pid = thread_create(thread_stack_buffer_low,
                                     STACKSIZE,
                                     THREAD_PRIORITY_MAIN - 2,
                                     THREAD_CREATE_STACKTEST,
                                     low_priority_thread,
                                     NULL,
                                     "low_priority_thread");

    msg_t msg;
    msg.type = 1;

    while (1) {
        msg.content.value = 100;
        msg_send(&msg, high_priority_pid);

        msg.content.value = 200;
        msg_send(&msg, low_priority_pid);

        xtimer_sleep(1);
    }

    return 0;
}
