
#include "thread.h"
#include "msg.h"
#include "fmt.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

kernel_pid_t pid_high, pid_low;

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %" PRIu32 "\n", (uint32_t)msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %" PRIu32 "\n", (uint32_t)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;
    char stack_high[STACKSIZE];
    char stack_low[STACKSIZE];

    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high_priority");

    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low_priority");

    msg.type = 1;
    msg.content.value = 12345;

    msg_send(&msg, pid_high);
    msg_send(&msg, pid_low);

    return 0;
}
