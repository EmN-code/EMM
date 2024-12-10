
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    const char *message = STATE_MESSAGE;
    emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
}

int main(void) {
    // Initialize the UDP endpoint for the gateway
    ipv4_addr_t gw_addr = IPV4_ADDR(47, 102, 103, 1);
    gw.addr.ipv4 = gw_addr;

    // Initialize the topic
    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Connect to the MQTT-SN broker
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    // Register the topic
    emcute_reg(&topic);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
