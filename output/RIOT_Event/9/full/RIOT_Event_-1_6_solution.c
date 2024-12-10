#include "ztimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "msg.h"
#include "timex.h"
#include "thread.h"
#include "unistd.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "net/emcute.h"
#include "net/gcoap.h"

#define INTERVAL 1
#define QUEUE_SIZE 8

static mbox_t mbox;
static msg_t msg_queue[QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_mbox_event_t evtimer_event;

void callback(void *arg) {
    (void)arg;
    printf("Event triggered every second\n");
}

void network_connect() {
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying in 1 second\n");
            usleep(1000000);
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);
    evtimer_event.callback = callback;
    evtimer_event.mbox = &mbox;

    evtimer_add_mbox(&evtimer, &evtimer_event, INTERVAL);

    network_connect();

    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1)) {
            evtimer_process(&evtimer, &msg);
        }
    }

    return 0;
}