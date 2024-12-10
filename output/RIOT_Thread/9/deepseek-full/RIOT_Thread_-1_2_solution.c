
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static kernel_pid_t pid_high, pid_low;

static msg_t msg_queue_high[8];
static msg_t msg_queue_low[8];

void *receiver_thread(void *arg)
{
    msg_t msg;
    kernel_pid_t sender_pid = (kernel_pid_t)arg;

    while (1) {
        msg_receive(&msg);
        printf("Receiver thread received message from PID %d\n", sender_pid);
    }

    return NULL;
}

void *sender_thread(void *arg)
{
    msg_t msg;
    kernel_pid_t target_pid = (kernel_pid_t)arg;

    while (1) {
        msg.content.value = 123;
        msg_send(&msg, target_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue_high, 8);
    msg_init_queue(msg_queue_low, 8);

    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, receiver_thread,
                             (void *)pid_high, "receiver_high");

    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, sender_thread,
                            (void *)pid_high, "sender_low");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
