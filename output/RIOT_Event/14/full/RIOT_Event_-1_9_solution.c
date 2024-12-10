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

static evtimer_t evtimer;
static evtimer_msg_event_t event;

static void callback(void *arg) {
    (void)arg;
    printf("Callback function triggered\n");
}

int main(void) {
    printf("Starting event timer example\n");

    evtimer_init_msg(&evtimer);

    event.event.period = 1 * US_PER_SEC;
    event.event.callback = callback;

    evtimer_add_msg(&evtimer, &event, KERNEL_PID_MAIN);

    while (1) {
        evtimer_run(&evtimer);
        xtimer_sleep(1);
    }

    return 0;
}