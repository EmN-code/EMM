
#include "evtimer_mbox.h"
#include "msg.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "time_units.h"
#include "event/thread.h"
#include "xtimer.h"
#include "thread.h"

#define STACKSIZE 512
#define NUM_MESSAGES 16

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
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
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1,
                                          THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1,
                                         THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    msg_t msg;
    msg.type = 0;

    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, high_pid);
        } else {
            msg_send(&msg, low_pid);
        }
        ztimer_sleep(ZTIMER_MSEC, 100);
    }

    return 0;
}
