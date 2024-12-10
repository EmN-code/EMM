#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "net/mqttsn.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "MQTTClient.h"

#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"

static const char *broker_ip = "47.102.103.1";
static const uint16_t broker_port = 1883;
static sock_udp_ep_t gw = { .family = AF_INET6 };

void on_mqttsn_pub(emcute_sub_t *sub, const emcute_topic_t *topic, void *data, size_t len) {
    (void)sub;
    (void)len;

    if (topic->id == sub->topic.id) {
        printf("Published: Topic: %s, Message: %s\n", topic->name, (char*) data);
    }
}

int main(void) {
    gw.port = broker_port;
    gw.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET6, broker_ip, &gw.addr.ipv6);

    emcute_sub_t subscription;
    subscription.topic.name = "device/status";
    subscription.cb = on_mqttsn_pub;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker\n");
        return 1;
    }

    if (emcute_reg(&subscription.topic) != EMCUTE_OK) {
        puts("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        emcute_topic_t t;
        t.name = "device/status";
        t.id = subscription.topic.id;
        const char *state = (some_condition) ? STATE_WORK : STATE_NOT_WORK;
        if (emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message\n");
        }
        xtimer_sleep(5);
    }

    if (emcute_discon() != EMCUTE_OK) {
        puts("Error: unable to disconnect from broker\n");
    }
    return 0;
}
