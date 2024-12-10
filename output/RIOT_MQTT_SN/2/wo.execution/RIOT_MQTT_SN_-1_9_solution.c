#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include <MQTTClient.h>
#include <esp_wifi.h>
#include <string.h>
#include <stdatomic.h>
#include <time_units.h>
#include <timex.h>
#include <stddef.h>

#define MAIN_QUEUE_SIZE     (8)
#define EMCUTE_ID          (0)
#define BROKER_PORT         (1883)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static emcute_sub_t subscriptions[1];
static char topic_name[] = "state";

void connect_to_broker(sock_udp_ep_t *gw)
{
    gw->family = AF_INET;
    gw->port = BROKER_PORT;
    inet_pton(AF_INET, "47.102.103.1", &gw->addr.ipv4);

    if (emcute_con(gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker\n");
        return;
    }
}

int main(void)
{
    static sock_udp_ep_t gw = {.family = AF_INET, .port = BROKER_PORT};
    emcute_topic_t t;
    t.name = topic_name;
    t.id = EMCUTE_ID;

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    connect_to_broker(&gw);

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        const char *data = "work";
        if (emcute_pub(&t, data, strlen(data), EMCUTE_QOS_1) != EMCUTE_OK) {
            puts("Error: unable to publish data\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}