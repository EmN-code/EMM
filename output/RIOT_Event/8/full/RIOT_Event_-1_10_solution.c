#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "kernel_defines.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "event/callback.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "msg.h"
#include "mutex.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "unistd.h"
#include "thread.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)

static void _periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    printf("RIOT Periodic Timer Example\n");

    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_init_msg(&evtimer);
    evtimer_add_msg(&evtimer, &event, KERNEL_PID_MAIN);

    ztimer_wait(ZTIMER_USEC, TIMER_INTERVAL);
    while (1) {
        _periodic_callback(NULL);
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);
    }

    return 0;
}