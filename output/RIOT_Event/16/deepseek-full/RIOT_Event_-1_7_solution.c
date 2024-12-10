
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <stdatomic.h>
#include <stddef.h>
#include <time_units.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <mutex.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    ztimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    while (1) {
        ztimer_set(ZTIMER_USEC, &timer, PERIODIC_INTERVAL);
        ztimer_sleep(ZTIMER_USEC, PERIODIC_INTERVAL);
    }

    return 0;
}
