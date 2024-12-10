
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
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *thread_low(void *arg)
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
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    pid_high = thread_create(stack_buffer_high, STACK_SIZE, PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    pid_low = thread_create(stack_buffer_low, STACK_SIZE, PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low");

    msg_t msg;
    msg.type = 0;

    for (int i = 0; i < 10; i++) {
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
