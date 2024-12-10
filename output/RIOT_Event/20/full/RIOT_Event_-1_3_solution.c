#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"
#include "stddef.h"
#include "unistd.h"
#include "stdatomic.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "stdlib.h"
#include "stdbool.h"
#include "evtimer_mbox.h"
#include "time_units.h"

#define INTERVAL (1U * US_PER_SEC)

static evtimer_event_t event;
static char print_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[4];

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void)
{
    puts("Starting periodic event example\n");

    msg_init_queue(msg_queue, 4);

    evtimer_init_msg(&event, &event_periodic_callback, NULL);

    evtimer_add_msg(&event, EVENT_PRIO_LOWEST, INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}