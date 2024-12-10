#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "mbox.h"
#include "msg.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "MQTTClient.h"
#include "time_units.h"

#define PERIODIC_INTERVAL (1LU * US_PER_SEC)

static evtimer_t evtimer;
static evtimer_mbox_event_t evtimer_mbox_event;
static mbox_t mbox;
static char mbox_stack[THREAD_STACKSIZE_MAIN];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
    /* Add custom logic here */
}

void *mbox_thread(void *arg)
{
    msg_t msg;
    while (1) {
        if (mbox_receive(&mbox, &msg) == 1) {
            periodic_callback(NULL);
        }
    }
    return NULL;
}

int main(void)
{
    mbox_init(&mbox, mbox_stack, sizeof(mbox_stack));

    evtimer_init(&evtimer, EVTIMER_PRIO_LOWEST);
    evtimer_set_periodic(&evtimer, &evtimer_mbox_event.event, PERIODIC_INTERVAL, &mbox);

    evtimer_update(&evtimer);

    kernel_pid_t mbox_pid = thread_create(mbox_stack, sizeof(mbox_stack), THREAD_PRIORITY_MAIN - 1, 0, mbox_thread, NULL, "mbox_thread");
    if (mbox_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating mbox thread\n");
        return 1;
    }

    while (1) {
        thread_sleep();
    }

    return 0;
}