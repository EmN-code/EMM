
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <stdio.h>
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "event/callback.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_mbox.h"

#define INTERVAL (1U * US_PER_SEC)

static evtimer_t my_evtimer;

void my_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void)
{
    evtimer_init_msg(&my_evtimer);

    evtimer_event_t event;
    event.callback = my_callback;

    evtimer_add_msg(&my_evtimer, &event, thread_getpid());

    while (1)
    {
        xtimer_sleep(INTERVAL);
        evtimer_periodic_wakeup(&my_evtimer);
    }

    return 0;
}