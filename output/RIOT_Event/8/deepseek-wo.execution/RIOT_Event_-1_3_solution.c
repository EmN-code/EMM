
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "timex.h"
#include "mbox.h"
#include "event/thread.h"
#include "time_units.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static evtimer_t evtimer;

static void _periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_init_mbox(&evtimer);

    /* Set up a periodic event every second */
    evtimer_add_periodic(&evtimer, ztimer_now(ZTIMER_MSEC) + ZTIMER_SEC(1), ZTIMER_SEC(1), _periodic_callback, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        /* Handle messages if needed */
    }

    return 0;
}
