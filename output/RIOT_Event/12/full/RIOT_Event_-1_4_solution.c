#include <ztimer.h>
#include <unistd.h>
#include <thread.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <event/callback.h>
#include <time_units.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "192.168.1.1"
#define MQTT_BROKER_PORT 1883

static msg_t msg_queue[8];
static evtimer_msg_event_t event;
static void on_event(void *arg)
{
    printf("Event triggered; executing periodic actions\n");
}

static void mqtt_connect(Network *network)
{
    int rc;
    while (1) {
        rc = NetworkConnect(network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed; retrying in 1 second...\n");
            xtimer_sleep(1);
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }
}

int main(void)
{
    Network network;
    mqtt_connect(&network);

    msg_init_queue(msg_queue, 8);

    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    event.event.callback = on_event;
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t m;
        msg_receive(&m);  // Blocking to handle the event
    }
    return 0;
}
