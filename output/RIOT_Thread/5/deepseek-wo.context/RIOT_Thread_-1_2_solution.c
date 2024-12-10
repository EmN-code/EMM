
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char stack_buffer_high[STACK_SIZE];
char stack_buffer_low[STACK_SIZE];

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

    pid_high = thread_create(stack_buffer_high, sizeof(stack_buffer_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high_priority_thread");

    pid_low = thread_create(stack_buffer_low, sizeof(stack_buffer_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low_priority_thread");

    msg.content.ptr = "Message to high priority thread";
    msg_send(&msg, pid_high);

    msg.content.ptr = "Message to low priority thread";
    msg_send(&msg, pid_low);

    return 0;
}
