#include <esp_system.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <msg.h>
#include <time_units.h>
#include <arpa/inet.h>
#include <thread.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <net/sock/udp.h>
#include <event/callback.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <xtimer.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/emcute.h>
#include <timex.h>
#include <netinet/in.h>
#include <unistd.h>
#include <evtimer_mbox.h>

#define INTERVAL (1U * US_PER_SEC)

static void callback(void *arg) {
    (void)arg;
    printf("Callback triggered!\n");
}

int main(void) {
    ztimer_periodic_t periodic_timer;
    /* initialize periodic timer */
    ztimer_periodic_init(ZTIMER_SEC, &periodic_timer, callback, NULL, INTERVAL);
    /* start the periodic timer */
    ztimer_periodic_start(&periodic_timer);
    return 0;
}