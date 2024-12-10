#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <stddef.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <event/callback.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <thread.h>
#include <arpa/inet.h>
#include <timex.h>

#include "evtimer_msg.h" // Ensure that necessary evtimer functions are available

// Callback function for the evtimer
static void _event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event triggered every second.\n");
}

int main(void)
{
    // Initialize evtimer
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Create an event
    evtimer_event_t event;

    // Set the event configuration
    evtimer.callback = _event_periodic_callback;
    evtimer.arg = NULL;

    // Error corrected: Use evtimer_add_msg instead of evtimer_add_event
    evtimer_add_msg(&evtimer, &event, 1 * US_PER_SEC);

    while(1) {
        // Wait and process the event every second
        evtimer_run(&evtimer);
    }
    return 0;
}