#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MSG "work"
#define DELAY 5

static emcute_topic_t topic;

static int publish_state(const char *state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = MQTT_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("[error] unable to register topic\n");
        return 1;
    }

    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        printf("[error] unable to publish data\n");
        return 1;
    }

    printf("[info] data published: %s\n", state);
    return 0;
}

int main(void) {
    puts("MQTT-SN client example\n");

    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP) == NULL) {
        printf("[error] unable to parse broker address\n");
        return 1;
    }

    emcute_con(&gw, false, NULL, NULL, 0, 0);

    while (1) {
        publish_state(MQTT_MSG);
        xtimer_sleep(DELAY);
    }

    emcute_discon();
    return 0;
}