
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 7

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static kernel_pid_t pid_high, pid_low;

static void *sender_thread(void *arg)
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

static void *receiver_thread(void *arg)
{
    (void)arg;

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
