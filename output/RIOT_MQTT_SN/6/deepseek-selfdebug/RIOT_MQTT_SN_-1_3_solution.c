
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define TOPIC_NAME "state"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // This function is intentionally left empty to avoid the unused function warning.
}

int main(void) {
    // Initialize the network stack
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP) == NULL) {
        puts("Error: unable to parse broker IP address");
        return 1;
    }

    // Initialize EMCUTE
    emcute_init();

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    puts("Topic registered");

    // Publish state every 5 seconds
    char state[] = "work";
    while (1) {
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("State published");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
