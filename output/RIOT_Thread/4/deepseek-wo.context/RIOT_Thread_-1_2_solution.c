
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACK_SIZE_HIGH (THREAD_STACKSIZE_DEFAULT)
#define STACK_SIZE_LOW (THREAD_STACKSIZE_DEFAULT)

char stack_high[STACK_SIZE_HIGH];
char stack_low[STACK_SIZE_LOW];

kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low");

    msg.content.ptr = "Message from main to high priority thread";
    msg_send(&msg, pid_high);

    msg.content.ptr = "Message from main to low priority thread";
    msg_send(&msg, pid_low);

    return 0;
}
