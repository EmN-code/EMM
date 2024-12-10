
#include "ztimer.h"
#include "mqtt.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "pthread.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "stdlib.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "esp_system.h"

void periodic_callback(void *arg) {
    printf("Callback function executed\n");
    /* add logic for periodic tasks here */
}

void connect_network() {
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Failed to connect, retrying\n");
            sleep(1);
        } else {
            printf("Network connected\n");
            break;
        }
    }
}

int main(void) {
    printf("System started\n");
    evtimer_t evtimer;
    evtimer_mbox_event_t event;
    mbox_t mbox;
    msg_t queue[8];

    msg_init_queue(queue, 8);
    mbox_init(&mbox, queue, 8);
    evtimer_init_mbox(&evtimer);

    event.callback = periodic_callback;
    event.arg = NULL;
    evtimer_add_mbox(&evtimer, &event, &mbox);

    connect_network();

    while (1) {
        msg_t msg;
        if (_mbox_get(&mbox, &msg, 1)) {
            evtimer_mbox_event_t *evt = (evtimer_mbox_event_t *)msg.content.ptr;
            evt->callback(evt->arg);
        }
    }
    return 0;
}
