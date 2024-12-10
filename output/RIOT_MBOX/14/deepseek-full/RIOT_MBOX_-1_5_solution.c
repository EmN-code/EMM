
#include "thread.h"
#include "xtimer.h"
#include "stdio.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "mbox_put.h"
#include "evtimer_init_mbox.h"
#include "mbox_get.h"
#include "xtimer_now_usec.h"
#include "xtimer_ticks_from_usec.h"
#include "paho_mqtt.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    evtimer_mbox_event_t event;
    event.event.callback = event_callback;
    event.event.arg = NULL;
    event.mbox = &mbox;

    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000); // 1 second
    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received!\n");
    }

    return 0;
}
