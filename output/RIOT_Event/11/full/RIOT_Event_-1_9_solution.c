#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "msg.h"
#include "mbox.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "xtimer.h"
#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "unistd.h"
#include <stddef.h>
#include <stdatomic.h>

#define PERIODIC_INTERVAL    (1000000U) // 1 second in microseconds

static void _callback(void *arg)
{
    (void)arg;
    printf("Periodic callback executed.\n");
    // Reset the timer
    evtimer_mbox_event_t event;
    event.event.period = PERIODIC_INTERVAL;
    event.event.callback = _callback;
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_mbox_event_t event;

    event.event.period = PERIODIC_INTERVAL;
    event.event.callback = _callback;

    // Initialize the event timer with mbox handling
    evtimer_mbox_t mbox;
    evtimer_mbox_init(&evtimer, &mbox, 10);

    // Add event to mbox handling
    evtimer_mbox_event_add(&evtimer, &event);

    // Start the event loop
    while (1) {
        evtimer_mbox_run(&evtimer);
        usleep(PERIODIC_INTERVAL);
    }

    return 0;
}