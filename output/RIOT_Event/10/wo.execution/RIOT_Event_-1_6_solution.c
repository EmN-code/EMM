#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "netinet/in.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define TIMER_INTERVAL_SEC 1

void event_callback(void *arg) {
    printf("Callback function executed.\n");
}

int main(void) {
    evtimer_t event_timer;
    evtimer_event_t my_event;

    evtimer_init(&event_timer, event_callback);

    my_event.event.next = NULL;  // Initialize the event
    evtimer_add(&event_timer, &my_event);

    while (1) {
        ztimer_sleep(ZTIMER_USEC, ztimer_usec_from_sec(TIMER_INTERVAL_SEC));
        evtimer_start(&event_timer);
    }

    return 0;
}