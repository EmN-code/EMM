#include <ztimer.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <event/callback.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time_units.h>
#include <netinet/in.h>
#include <paho_mqtt.h>

#define INTERVAL (1U * US_PER_SEC)

static void event_periodic_callback(void *arg)
{
    (void)arg;
    // Do something every second
    printf("Event triggered\n");
}

int main(void)
{
    evtimer_periodic_t periodic_event;

    evtimer_periodic_init(&periodic_event, event_periodic_callback, NULL);

    ztimer_periodic_start(ZTIMER_MSEC, &periodic_event, INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}