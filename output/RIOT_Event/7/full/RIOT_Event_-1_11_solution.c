#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "msg.h"
#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "dht_params.h"
#include "esp_system.h"
#include "timex.h"
#include "dht.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "evtimer.h"
#include "architecture.h"
#include "mutex.h"
#include "net/emcute.h"
#include "event/callback.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define PERIODIC_EVENT_INTERVAL (1U * US_PER_SEC)  // Interval for periodic event (1 second)

static ztimer_t periodic_timer;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic timer event triggered!\n");
}

int main(void) {
    puts("Periodic event initialization and start example using RIOT OS");

    ztimer_set(ZTIMER_SEC, &periodic_timer, PERIODIC_EVENT_INTERVAL);
    printf("Periodic timer configured for every %lu microseconds\n", PERIODIC_EVENT_INTERVAL);

    for (;;) {
        ztimer_sleep(ZTIMER_USEC, PERIODIC_EVENT_INTERVAL);  // Sleep for the duration of one interval
        periodic_callback(NULL);  // Invoke the callback function
    }

    return 0;
}