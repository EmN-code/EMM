
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t pid_high, pid_low;
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1, 0, high_priority_thread, NULL, "high_priority");
    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1, 0, low_priority_thread, NULL, "low_priority");

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, pid_high);
        } else {
            msg_send(&msg, pid_low);
        }
        xtimer_sleep(1);
    }

    return 0;
}
