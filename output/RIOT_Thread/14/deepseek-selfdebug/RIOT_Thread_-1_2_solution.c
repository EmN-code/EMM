
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
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
        printf("High priority thread received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    pid_high = thread_create(stack_buffer_high, sizeof(stack_buffer_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "thread_high");

    pid_low = thread_create(stack_buffer_low, sizeof(stack_buffer_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "thread_low");

    msg_t msg;
    msg.content.value = 123;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, pid_high);
        xtimer_sleep(1);
        msg_send(&msg, pid_low);
    }

    return 0;
}
