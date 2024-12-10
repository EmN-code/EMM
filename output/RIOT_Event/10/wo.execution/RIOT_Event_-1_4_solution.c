#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "time_units.h"

#define TIMER_INTERVAL_SEC   (1U)

static evtimer_t evtimer;
static evtimer_msg_event_t event;
static msg_t msg_queue[8];

void event_handler(void *arg) {
    (void)arg;
    printf("Timer event triggered\n");
}

int main(void) {
    /* initialize the event timer for message events */
    evtimer_init_mbox(&evtimer);

    /* configure the event */
    event.pid = thread_getpid();
    event.event.handler = event_handler;

    /* start the periodic timer */
    evtimer_add_msg(&evtimer, &event, msg_queue);

    while (1) {
        /* simulate a main loop */
        usleep(1000000);
    }

    return 0;
}