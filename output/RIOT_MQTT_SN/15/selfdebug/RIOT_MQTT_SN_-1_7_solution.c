#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/emcute.h>

#define EMCUTE_PORT        (1883U)
#define MQTT_BUFFER_SIZE   (128U)
#define MQTT_TOPIC_NAME    "state"
#define PUBLISH_INTERVAL   (5U * 1000000U) // 5 seconds

static emcute_topic_t topic;

static void publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0; // Use QoS level 0
    if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
        printf("error: unable to publish data
");
    } else {
        printf("Successfully published: %s\n", state);
    }
}

int main(void) {
    sock_udp_ep_t gw;
    gw.port = EMCUTE_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway\n");
        return 1;
    }
    printf("Connected to MQTT-SN broker\n");

    topic.name = MQTT_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic
");
        return 1;
    }

    while (1) {
        const char *state = "work"; // Can be dynamically changed as needed
        publish_state(state);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}