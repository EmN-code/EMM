#include <esp_system.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <event/thread.h>
#include <event/callback.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <unistd.h>
#include <mutex.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <msg.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <kernel_defines.h>

#define EVTIMER_INTERVAL (1U * (ztime_t)ZTIME_PER_SEC)

static void evtimer_callback(void *arg) {
    (void)arg;
    printf("Event timer callback triggered\n");
}

int main(void) {
    printf("Initializing event timer with periodic callback every second\n");

    evtimer_t evtimer;
    evtimer_msg_event_t evtimer_event;

    evtimer_init_msg(&evtimer);

    evtimer_event.event.callback = evtimer_callback;
    evtimer_event.event.period = EVTIMER_INTERVAL;

    evtimer_add_msg(&evtimer, &evtimer_event, KERNEL_PID_MAIN);

    while (1) {
        evtimer_run(&evtimer);
        usleep(EVTIMER_INTERVAL);
    }

    return 0;
}