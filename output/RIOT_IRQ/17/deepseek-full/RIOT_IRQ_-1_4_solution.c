
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <event/thread.h>
#include <stddef.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define STACKSIZE 512
#define PRIORITY 5

static volatile bool interrupt_handled = false;
static volatile bool atomic_var_verified = false;

static void *interrupt_handler(void *arg)
{
    (void)arg;
    interrupt_handled = true;
    return NULL;
}

static void *thread_function(void *arg)
{
    (void)arg;
    while (1) {
        if (interrupt_handled) {
            atomic_var_verified = true;
            interrupt_handled = false;
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    msg_t msg_queue[8];
    mbox_t mbox;
    evtimer_t evtimer;
    evtimer_mbox_t evtimer_mbox;

    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_init(&evtimer);
    evtimer_mbox_init(&evtimer_mbox, &mbox);

    char stack[STACKSIZE];
    thread_create(stack, sizeof(stack), PRIORITY, 0, thread_function, NULL, "thread");

    ztimer_set(ZTIMER_MSEC, &evtimer, 1000);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        if (atomic_var_verified) {
            printf("Atomic variable verified: Interrupts are working correctly.\n");
            atomic_var_verified = false;
        }
    }

    return 0;
}
