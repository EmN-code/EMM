#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <mqttclient.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <mbox.h>
#include <thread.h>
#include <time_units.h>

// Callbacks and handlers
void _evtimer_mbox_handler(void *arg) {
    // Handle mbox event
    // This is the handler function that will be called when the event is triggered
}

// Main function
int main(void) {
    evtimer_t evtimer;
    evtimer_mbox_event_t mbox_event;
    mbox_t mbox;

    // Initialize the mbox and evtimer
    mbox_init(&mbox, mbox_array, MBOX_SIZE_MAX);
    evtimer_init_mbox(&evtimer);

    // Fill the evtimer_mbox_event_t struct with relevant data
    mbox_event.mbox = &mbox;
    mbox_event.event.period = 1 * US_PER_SEC; // Set the event period to 1 second
    mbox_event.event.callback = _evtimer_mbox_handler;

    // Add the event to the evtimer
    evtimer_add_mbox(&evtimer, &mbox_event, &mbox);

    // Run an infinite loop, you might want to put the MCU to sleep to save power
    while(1) {
        xtimer_sleep(1);
    }

    return 0;
}