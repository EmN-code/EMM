#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "timex.h"
#include "ztimer.h"
#include "event/thread.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "evtimer_msg.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "MQTTClient.h"

static void my_callback(void *arg) {
    (void)arg;
    printf("Callback triggered!\n");
}

int main(void) {
    puts("RIOT event timer example\n");

    evtimer_t evtimer;
    evtimer_msg_event_t evtimer_event;
    evtimer_event.event.offset = 1 * US_PER_SEC;
    evtimer_event.event.period = 1 * US_PER_SEC;
    evtimer_event.event.callback = my_callback;
    evtimer_event.event.arg = NULL;
    evtimer_event.event.next = NULL;

    evtimer_init(&evtimer);
    evtimer_event_add(&evtimer, &evtimer_event.event);

    while (1) {
        evtimer_run_all(&evtimer);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}