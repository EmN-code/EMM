
#include "evtimer_mbox.h"
#include "msg.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "xtimer.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static kernel_pid_t pid_high, pid_low;

static void *thread_high(void *arg)
{
    msg_t msg;
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

static void *thread_low(void *arg)
{
    msg_t msg;
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;

    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low");

    msg.content.value = 1;
    msg_send(&msg, pid_high);

    msg.content.value = 2;
    msg_send(&msg, pid_low);

    return 0;
}
