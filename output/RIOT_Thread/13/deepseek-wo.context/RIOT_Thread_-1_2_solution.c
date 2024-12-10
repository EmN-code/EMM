
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

kernel_pid_t thread_pid_high, thread_pid_low;
char thread_stack_high[STACKSIZE];
char thread_stack_low[STACKSIZE];

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
        msg_send(&msg, thread_pid_high);
    }

    return NULL;
}

int main(void)
{
    thread_pid_high = thread_create(thread_stack_high, sizeof(thread_stack_high), PRIORITY, 0, thread_high, NULL, "thread_high");
    thread_pid_low = thread_create(thread_stack_low, sizeof(thread_stack_low), PRIORITY + 1, 0, thread_low, NULL, "thread_low");

    return 0;
}
