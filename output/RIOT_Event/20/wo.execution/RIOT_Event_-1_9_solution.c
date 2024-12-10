#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <msg.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <string.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <mbox.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <thread.h>

#define INTERVAL_SEC (1U * US_PER_SEC)

static evtimer_t evtimer;
static evtimer_event_t periodic_event;

static void periodic_callback(void *arg) {
    (void)arg;
    puts("Periodic callback triggered");
}

int main(void) {
    puts("RIOT application for ESP32 with evtimer started");
    
    evtimer_init_msg(&evtimer);

    periodic_event.callback = periodic_callback;
    periodic_event.event.offset = INTERVAL_SEC;
    evtimer_add(&evtimer, &periodic_event);

    while (1) {
        uint32_t now = ztimer_now(ZTIMER_USEC);
        evtimer_run(&evtimer, now);
        xtimer_sleep(1);
    }

    return 0;
}