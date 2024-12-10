#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "ztimer.h"
#include "kernel_defines.h"
#include "event/callback.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "thread.h"
#include "arpa/inet.h"
#include "timex.h"

static void callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event = { .event.callback = callback, .event.arg = NULL };

    /* Initialize the event timer */
    evtimer_init_msg(&evtimer);

    /* Define the periodic time: 1 second */
    event.event.period = 1 * US_PER_SEC;
    event.msg.type = 0;

    /* Add the periodic event to the timer */
    evtimer_add_msg(&evtimer, &event, KERNEL_PID_MAIN);

    /* Let the main thread wait indefinitely */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}