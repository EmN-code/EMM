
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "event/periodic_callback.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "MQTTClient.h"

#define STACKSIZE 512
#define NUM_MESSAGES 8

static char stack_low_prio[STACKSIZE];
static char stack_high_prio[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

static void *low_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

static void *high_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t low_prio_pid, high_prio_pid;
    msg_t msg;

    msg_init_queue(msg_queue, NUM_MESSAGES);

    low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                 THREAD_PRIORITY_MAIN - 1, 0, low_prio_thread, NULL, "low_prio");

    high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                  THREAD_PRIORITY_MAIN - 2, 0, high_prio_thread, NULL, "high_prio");

    msg.type = 1;
    msg.content.value = 1234;

    msg_send(&msg, low_prio_pid);
    msg_send(&msg, high_prio_pid);

    return 0;
}
