
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define STACK_SIZE_HIGH (THREAD_STACKSIZE_DEFAULT + 512)
#define STACK_SIZE_LOW (THREAD_STACKSIZE_DEFAULT)

#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN - 2)

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
    msg.content.ptr = "Hello from low priority thread!";

    while (1) {
        xtimer_sleep(5);
        msg_send(&msg, pid_high);
    }

    return NULL;
}

int main(void)
{
    pid_high = thread_create(stack_high, STACK_SIZE_HIGH, PRIORITY_HIGH, 0, thread_high, NULL, "high_prio");
    pid_low = thread_create(stack_low, STACK_SIZE_LOW, PRIORITY_LOW, 0, thread_low, NULL, "low_prio");

    return 0;
}
