#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "stdbool.h"

#define MQTT_BROKER_IP "broker_ip"
#define MQTT_BROKER_PORT 1883
#define EVENT_PERIOD_US (1000000U)

static void network_connect_retry(void) {
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connect failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        } else {
            printf("Network connect successful\n");
            break;
        }
    }
}

static void callback(void *arg) {
    (void)arg;
    printf("Callback triggered\n");
    // Add event handling logic here
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    mbox_t mbox;
    msg_t queue[8];

    network_connect_retry();

    mbox_init(&mbox, queue, 8);

    evtimer_add_mbox(&evtimer, &event, &mbox);
    evtimer_msg_event_t evtimer_event = {
        .event.offset = EVENT_PERIOD_US,
        .mbox = &mbox,
        .event.callback = callback
    };

    evtimer_msg_queue_init(&evtimer, &mbox);
    evtimer_add(&evtimer, &evtimer_event.event);

    while (1) {
        evtimer_msg_queue_process(&mbox);
    }

    return 0;
}
