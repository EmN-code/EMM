#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <stddef.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <event/callback.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <arpa/inet.h>
#include <timex.h>
#include <esp_system.h>
#include <sys/socket.h>
#include <net/gcoap.h>

void timer_callback(void *arg) {
    printf("Timer callback triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer_init(&evtimer, timer_callback);

    uint32_t period = 1 * US_PER_SEC;
    evtimer_set(&evtimer, &event, period);

    while (1) {
        evtimer_dispatch(&evtimer);
        xtimer_sleep(period);
    }
    return 0;
}