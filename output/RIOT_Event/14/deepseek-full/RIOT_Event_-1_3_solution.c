
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <mbox.h>
#include <xtimer.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mma8x5x_regs.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static msg_t msg_queue[8];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = (evtimer_callback_t)periodic_callback;
    event.event.arg = NULL;

    while (1) {
        xtimer_usleep(PERIODIC_INTERVAL);
        evtimer_add_msg(&evtimer, &event, sched_active_pid);
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
