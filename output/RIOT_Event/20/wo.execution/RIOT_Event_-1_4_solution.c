#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "thread.h"
#include "timex.h"
#include "unistd.h"
#include "msg.h"
#include "paho_mqtt.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "evtimer_mbox.h"

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

void event_callback(void *arg) {
    (void)arg;
    printf("Event triggered every second.\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Setup the periodic event to trigger event_callback every second
    evtimer_set(&event, INTERVAL, event_callback, NULL);
    evtimer_add_periodic(&evtimer, &event);

    // Keep the main thread alive to process the timers
    while (1) {
        evtimer_run(&evtimer);
        thread_yield();
    }

    return 0;
}