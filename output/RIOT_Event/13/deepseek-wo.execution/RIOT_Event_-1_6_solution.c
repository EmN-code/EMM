
#include <stdlib.h>
#include <net/emcute.h>
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

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void event_periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_t evtimer;
    evtimer_init(&evtimer, event_periodic_callback);

    // Set the periodic event
    evtimer_add_event(&evtimer, TIMER_INTERVAL, NULL);

    // Main loop to keep the thread running
    while (1) {
        // Yield to allow other threads to run
        thread_yield();
    }

    return 0;
}
