#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include "net/emcute.h"  // include emcute header

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT    1883
#define TOPIC_NAME     "device/state"
#define QOS_LEVEL      0

static emcute_topic_t topic;

static void publish_state(const char *state) {
    if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("error: unable to publish data \n");
    }
}

int main(void) {
    struct sockaddr_in sin = { .sin_family = AF_INET };

    // Convert IP address to binary form
    if (inet_pton(AF_INET, BROKER_ADDRESS, &sin.sin_addr.s_addr) != 1) {
        printf("Error: Invalid broker address\n");
        return 1;
    }

    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    memcpy(&gw.addr, &sin.sin_addr.s_addr, sizeof(gw.addr));

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Unable to connect to the broker\n");
        return 1;
    }

    topic.name = TOPIC_NAME;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: Unable to register topic\n");
        return 1;
    }

    while (1) {
        publish_state("work");
        sleep(5);
        publish_state("not work");
        sleep(5);
    }

    return 0;
}