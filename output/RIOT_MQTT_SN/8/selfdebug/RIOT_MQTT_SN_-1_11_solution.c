#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "xtimer.h"

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         1883U
#define EMCUTE_TOPIC_NAME   "state"
#define MQTT_BROKER_IP      "47.102.103.1"
#define PUBLISH_INTERVAL    5

static emcute_topic_t topic;
static sock_udp_ep_t remote;

void publish_state(const char *state) {
    int res;
    res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("[ERROR] Unable to publish data: %d\n", res);
    }
}

int setup_connection(void) {
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = EMCUTE_PORT;

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) != 1) {
        printf("[ERROR] Unable to parse IP address\n");
        return 1;
    }

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[ERROR] Unable to connect to MQTT-SN broker\n");
        return 1;
    }

    topic.name = EMCUTE_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[ERROR] Unable to register topic\n");
        return 1;
    }

    return 0;
}

int main(void) {
    if (setup_connection() != 0) {
        return 1;
    }

    while (1) {
        publish_state("work");
        xtimer_sleep(PUBLISH_INTERVAL);
        publish_state("not work");
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}