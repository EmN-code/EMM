#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <xtimer.h>
#include <msg.h>
#include <mbox.h>
#include <event/thread.h>
#include <event/callback.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define SLEEP_INTERVAL 1

static void _event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_msg_t evtimer_msg;

    evtimer_init_msg(&evtimer);

    evtimer_add_periodic(&evtimer, &evtimer_msg.super, SLEEP_INTERVAL * US_PER_SEC, _event_callback, NULL);

    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
