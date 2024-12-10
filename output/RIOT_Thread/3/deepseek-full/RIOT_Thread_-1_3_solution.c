
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "unistd.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 5

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    msg_t msg, reply;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %lu\n", (unsigned long)msg.content.value);
        reply.content.value = msg.content.value * 2;
        msg_reply(&msg, &reply);
    }
    return NULL;
}

void *thread_low(void *arg)
{
    msg_t msg, reply;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %lu\n", (unsigned long)msg.content.value);
        reply.content.value = msg.content.value * 2;
        msg_reply(&msg, &reply);
    }
    return NULL;
}

int main(void)
{
    msg_t msg, reply;

    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, thread_high, NULL, "high");
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, thread_low, NULL, "low");

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send_receive(&msg, &reply, pid_high);
        } else {
            msg_send_receive(&msg, &reply, pid_low);
        }
        printf("Main thread received reply: %lu\n", (unsigned long)reply.content.value);
        xtimer_sleep(1);
    }

    return 0;
}
