#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static emcute_topic_t topic;

static int initialize_mqtt(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };

    uint8_t addr[4] = {47, 102, 103, 1};
    ipv6_addr_set(gw.addr.ipv6, 0, addr);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("MQTT-SN connection failed");
        return 1;
    }
    puts("MQTT-SN connected");

    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }
    return 0;
}

static void publish_state(void) {
    const char *data = "work";

    if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error publishing data");
    } else {
        puts("Data published");
    }
}

int main(void) {
    puts("RIOT MQTT-SN example");

    if (initialize_mqtt() != 0) {
        return 1;
    }

    // Publish loop
    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}