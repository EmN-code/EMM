
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

kernel_pid_t high_prio_pid;
kernel_pid_t low_prio_pid;

char high_prio_stack[STACKSIZE];
char low_prio_stack[STACKSIZE];

void *high_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %" PRIu32 "\n", (uint32_t)msg.content.value);
    }

    return NULL;
}

void *low_prio_thread(void *arg)
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

    high_prio_pid = thread_create(high_prio_stack, sizeof(high_prio_stack), THREAD_PRIORITY_HIGH,
                                  THREAD_CREATE_STACKTEST, high_prio_thread, NULL, "high_prio");

    low_prio_pid = thread_create(low_prio_stack, sizeof(low_prio_stack), THREAD_PRIORITY_LOW,
                                 THREAD_CREATE_STACKTEST, low_prio_thread, NULL, "low_prio");

    msg.type = 0;
    msg.content.value = 12345;

    msg_send(&msg, high_prio_pid);
    msg_send(&msg, low_prio_pid);

    return 0;
}
