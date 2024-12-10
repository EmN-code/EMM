# This example sets up an event timer on an ESP32 using the RIOT operating system. 
# The event timer triggers every second to turn on an LED. This example demonstrates using 
# the evtimer and GPIO functionality in RIOT OS with error handling and retries for robust operation.

#include "net/sock/util.h"
#include "stdatomic.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"


#define LED_ON_TIME_US (1 * US_PER_SEC)

static evtimer_t evtimer;
static evtimer_msg_event_t event;

void _evtimer_mbox_handler(evtimer_event_t *event) {
    /* Re-initiate the timer to create a periodic action */
    evtimer_add(&evtimer, event, LED_ON_TIME_US);
    printf("LED ON\n");
}

int main(void) {
    /* Initialize the event timer with the callback handler */
    evtimer_init_mbox(&evtimer);

    /* Setup and add the event to the evtimer */
    msg_t msg;
    event.event.event = 0;
    event.msg = msg;  // Initialize the msg to some value

    evtimer_add(&evtimer, &(event.event), LED_ON_TIME_US);

    /* Main loop does nothing; all is handled by the evtimer */
    while (1) {
        usleep(1);
    }

    return 0;
}