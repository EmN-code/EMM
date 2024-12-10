#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "xtimer.h"
#include "net/mqttsn.h"
#include "net/emcute.h"
#include "net/af.h"
#include "net/netif.h"

#define MQTT_SN_SERVER      "47.102.103.1"
#define MQTT_SN_PORT        1883
#define STATE_TOPIC         "device/state"
#define PUBLISH_INTERVAL    (5 * US_PER_SEC)

static emcute_topic_t state_topic;

static void connect_mqttsn(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_SN_PORT };
    ipv6_addr_t addr;

    if (ipv6_addr_from_str(&addr, MQTT_SN_SERVER) == NULL) {
        printf("Error parsing gateway address\n");
        return;
    }
    memcpy(&gw.addr.ipv6, &addr, sizeof(addr));

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to MQTT-SN gateway\n");
        return;
    }

    printf("Connected to MQTT-SN gateway\n");
}

void publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return;
    }

    if (emcute_pub(&state_topic, state, strlen(state), flags) != EMCUTE_OK) {
        printf("Failed to publish data\n");
    }
    else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    printf("Starting MQTT-SN example\n");

    connect_mqttsn();
    state_topic.name = STATE_TOPIC;

    while (1) {
        publish_state("work");
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}