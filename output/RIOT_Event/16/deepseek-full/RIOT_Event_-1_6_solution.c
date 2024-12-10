
#include "net/gcoap.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    ztimer_t timer;
    ztimer_set(ZTIMER_USEC, &timer, PERIODIC_INTERVAL);

    while (1) {
        if (ztimer_now(ZTIMER_USEC) >= timer.target) {
            periodic_callback(NULL);
            ztimer_set(ZTIMER_USEC, &timer, PERIODIC_INTERVAL);
        }
        xtimer_usleep(100);  // Small sleep to avoid busy-waiting
    }

    return 0;
}
