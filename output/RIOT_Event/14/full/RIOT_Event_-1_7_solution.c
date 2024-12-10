#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "event/callback.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "thread.h"
#include "arpa/inet.h"
#include "timex.h"
#include "sys/socket.h"
#include "net/gcoap.h"

#include "evtimer.h"
#include "xtimer.h"

static evtimer_t evtimer;
static evtimer_event_t event;

static void callback(void *arg)
{
    (void)arg;
    puts("Periodic callback executed.");
}

int main(void)
{
    evtimer_init(&evtimer, callback);

    event.period = 1 * US_PER_SEC;
    
    evtimer_add(&evtimer, &event);

    while (1) {
        evtimer_loop(&evtimer);
        xtimer_sleep(1);
    }

    return 0;
}
