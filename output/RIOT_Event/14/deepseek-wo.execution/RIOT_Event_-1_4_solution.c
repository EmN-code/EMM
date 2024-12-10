
#include <stdlib.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg_queue[8];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    evtimer_init_mbox(&evtimer);

    timex_t interval = timex_set(PERIODIC_INTERVAL, 0);
    evtimer_add_msg(&evtimer, interval, periodic_callback, NULL);

    evtimer_start(&evtimer);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
