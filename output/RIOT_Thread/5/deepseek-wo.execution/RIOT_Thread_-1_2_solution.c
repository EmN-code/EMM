
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "ztimer.h"
#include "msg.h"
#include "thread.h"
#include "evtimer_msg.h"

#define STACK_SIZE_HIGH (1024)
#define STACK_SIZE_LOW (512)

static char stack_high[STACK_SIZE_HIGH];
static char stack_low[STACK_SIZE_LOW];

static kernel_pid_t pid_high, pid_low;

static msg_t msg_queue_high[8];
static msg_t msg_queue_low[8];

void *thread_high(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue_high, 8);
    msg_init_queue(msg_queue_low, 8);

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 2,
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
