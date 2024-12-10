#include <paho_mqtt.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <msg.h>
#include <netinet/in.h>
#include <mbox.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <thread.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define EVENT_QUEUE_SIZE (8)

static evtimer_t evtimer;
static msg_t msg_queue[EVENT_QUEUE_SIZE];

// Callback function to handle periodic events
tatic void event_callback(void *arg)
{
    (void)arg;
    puts("Event triggered every second.");
}

int main(void)
{
    // Initialize message queue
    msg_init_queue(msg_queue, EVENT_QUEUE_SIZE);

    // Initialize event timer
evtimer_init_mbox(&evtimer);

    // Setup periodic timer event
evtimer_event_periodic_t periodic_event;
    periodic_event.interval = INTERVAL;
    periodic_event.callback = event_callback;

    // Attach event timer to message queue
    evtimer_add_periodic(&evtimer, &periodic_event, NULL);

    // Event dispatch loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}