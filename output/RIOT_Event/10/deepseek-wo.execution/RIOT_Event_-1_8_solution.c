
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
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

static void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    xtimer_init(&timer);

    while (1) {
        xtimer_set(&timer, PERIODIC_INTERVAL);
        periodic_callback(NULL);
        xtimer_usleep(PERIODIC_INTERVAL);
    }

    return 0;
}
