#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "emcute.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC) // 5 seconds

static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication: %.*s\n", (int)len, (char *)data);
}

static int mqtt_connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, MQTT_BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to the broker.");
        return 1;
    }
    puts("Successfully connected to the broker.");

    return 0;
}

static int mqtt_publish(const char *state) {
    topic.name = "state";
    topic.id = 0; // should be set by emcute

    emcute_topic_t *t = &topic;
    if (emcute_pub(t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Failed to publish data: %s\n", state);
        return 1;
    }
    printf("Published: %s\n", state);

    return 0;
}

int main(void) {
    puts("MQTT-SN Application Start");

    if (mqtt_connect() != 0) {
        return 1;
    }

    const char *states[] = {"work", "not work"};
    size_t index = 0;

    while (1) {
        mqtt_publish(states[index]);
        index = (index + 1) % 2;
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}