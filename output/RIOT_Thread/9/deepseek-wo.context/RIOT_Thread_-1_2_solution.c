
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char stack_high[STACKSIZE];
char stack_low[STACKSIZE];

kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, pid_low);
        xtimer_sleep(1);
    }

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                            THREAD_CREATE_STACKTEST, thread_high, NULL, "thread_high");

    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                           THREAD_CREATE_STACKTEST, thread_low, NULL, "thread_low");

    return 0;
}
