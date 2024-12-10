
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t periodic_timer;
static evtimer_event_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
    evtimer_add(&periodic_timer, &periodic_event);
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&periodic_timer, periodic_callback);

    // Set up the periodic event
    periodic_event.event.offset = 0;
    periodic_event.event.interval = PERIODIC_INTERVAL;

    // Add the event to the timer
    evtimer_add(&periodic_timer, &periodic_event);

    // Main loop to keep the thread alive
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
