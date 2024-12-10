#include <stdlib.h>
#include <stddef.h>
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "xtimer.h"
#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "dht.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "event/callback.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "mutex.h"
#include "stdbool.h"
#include "stdatomic.h"
#include <stdio.h>
#include <unistd.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Callback triggered\n");
    /* Add event handling code here */
}

int main(void) {
    evtimer_t periodic_timer;
    evtimer_msg_event_t periodic_event;

    msg_t queue[8];
    msg_init_queue(queue, 8);

    evtimer_init_mbox(&periodic_timer);

    /* set a periodic event every second */
    periodic_event.event.period = INTERVAL;
    periodic_event.event.callback = periodic_callback;

    evtimer_add_mbox(&periodic_timer, &periodic_event, NULL);

    while (1) {
        msg_t m;
        msg_receive(&m);
    }

    return 0;
}