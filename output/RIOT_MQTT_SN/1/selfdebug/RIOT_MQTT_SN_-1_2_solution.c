#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883

#define TOPIC_NAME "device/state"
#define MSG_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static char *topics[] = {TOPIC_NAME};

static void _publish(const char *state) {
    unsigned flags = EMCUTE_QOS_0;  
    if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
        printf("error: unable to publish data\n");
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        printf("error: unable to parse IPv4 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway at %s:%d\n", SERVER_ADDR, SERVER_PORT);
        return 1;
    }

    topic.name = TOPIC_NAME;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return 1;
    }

    while (1) {
        _publish("work");
        printf("published state: work\n");
        xtimer_sleep(MSG_INTERVAL);
    }

    return 0;
}