#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;

    // Initialize the evtimer with the ztimer backend
    static ztimer_t timer;
    evtimer.msg = &event;

    // Set the alarm callback function
    evtimer.callback = alarm_callback;

    // Set the timer to trigger the event after 5 seconds
    ztimer_set(ZTIMER_MSEC, &timer, 5000);

    printf("RTC alarm application started. Alarm in 5 seconds...\n");

    // Take the action when event triggers
    while(1) {
        evtimer_msg_event(&evtimer);
    }

    return 0;
}
