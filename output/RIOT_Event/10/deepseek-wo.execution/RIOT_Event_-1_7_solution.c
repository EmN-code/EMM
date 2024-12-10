
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t periodic_timer;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&periodic_timer, periodic_callback, NULL);

    // Create an event structure
    evtimer_event_t event;

    // Add the event to the timer with the specified interval
    xtimer_t timer;
    timer.target = timer.long_target = xtimer_now_usec() + PERIODIC_INTERVAL;
    timer.callback = (xtimer_callback_t)evtimer_add;
    timer.arg = &event;

    while (1) {
        xtimer_set(&timer, PERIODIC_INTERVAL);
        xtimer_spin(&timer);
    }

    return 0;
}
