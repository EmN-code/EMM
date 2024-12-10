#include "ztimer.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "timex.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "ztimer.h"
#include "ztimer_init_periodic"
#include "ztimer_start_periodic"
#include "evtimer_msg.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define INTERVAL_US 1000000

static void _callback(void *arg) {
    (void)arg;
    printf("Periodic event callback triggered.\n");
}

int main(void) {
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, _callback, NULL, INTERVAL_US);
    ztimer_periodic_start(&periodic_timer);

    while (1) {
        /* Let the CPU sleep until the next timer interrupt occurs */
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_US);
    }

    return 0;
}