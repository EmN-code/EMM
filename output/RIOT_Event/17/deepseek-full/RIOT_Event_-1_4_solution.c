
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    evtimer_t evtimer;
    ztimer_now_t now = ztimer_now(ZTIMER_USEC);
    timex_t interval = timex_set(0, PERIODIC_INTERVAL);

    evtimer_init_mbox(&evtimer);

    while (1) {
        ztimer_set(ZTIMER_USEC, &evtimer, interval.ticks);
        periodic_callback(NULL);
        ztimer_sleep(ZTIMER_USEC, interval.ticks);
    }

    return 0;
}
