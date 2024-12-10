#include "net/emcute.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include <stdio.h>
#include <string.h>

static sock_udp_ep_t gw = {
    .family = AF_INET,
    .port = 1883,
};

static emcute_topic_t topic;

void setup_mqtt(void) {
    if (emcute_con(&gw, 1, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to the gateway\n");
        return;
    }

    topic.name = "status";

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return;
    }
}

void publish_state(const char *state) {
    if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    }
}

int main(void) {
    printf("Setting up MQTT-SN\n");

    setup_mqtt();

    while (1) {
        publish_state("work");
        xtimer_sleep(5);
    }

    return 0;
}