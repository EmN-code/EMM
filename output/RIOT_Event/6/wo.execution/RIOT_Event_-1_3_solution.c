#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <msg.h>

#define INTERVAL (1U * US_PER_SEC)

static void periodic_event(void *arg)
{
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void)
{
    evtimer_t evtimer;
    msg_t queue[8];
    msg_init_queue(queue, 8);

    evtimer_msg_handler_init(&evtimer, queue);
    evtimer_msg_add_timer(&evtimer, periodic_event, NULL, INTERVAL, true);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}