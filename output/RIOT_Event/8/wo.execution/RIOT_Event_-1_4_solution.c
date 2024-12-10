#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <timex.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>

#define INTERVAL_SEC 1

static void event_handler(void)
{
    printf("Event triggered!\n");
}

int main(void)
{
    mbox_t mbox;
    mbox_init(&mbox, mbox_array, sizeof(mbox_array));

    evtimer_mbox_event_t evtimer_event;
    evtimer_t evtimer;

    evtimer_event.event.handler = event_handler;
    evtimer_event.event.expiration = xtimer_now() + xtimer_usec_from_ticks(xtimer_ticks_from_sec(INTERVAL_SEC));

    evtimer_init_mbox(&evtimer);
    evtimer_add_mbox(&evtimer, &evtimer_event, &mbox);

    while (1) {
        msg_t msg;
        mbox_receive(&mbox, &msg);
        msg.content.value;
    }

    return 0;
}