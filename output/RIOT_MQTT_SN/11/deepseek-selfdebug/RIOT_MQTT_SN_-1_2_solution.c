
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"

static sock_udp_ep_t broker = SOCK_IPV4_EP_ANY;
static emcute_topic_t topic;

void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    char payload[16];
    snprintf(payload, sizeof(payload), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    emcute_pub(&topic, payload, strlen(payload), 0);
}

int main(void) {
    // Initialize UDP socket for MQTT-SN
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 1883;
    sock_udp_t sock;
    sock_udp_create(&sock, &local, NULL, 0);

    // Set broker endpoint
    broker.family = AF_INET;
    broker.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_ADDR, &broker.addr.ipv4);

    // Connect to the MQTT-SN broker
    emcute_con(&broker, true, NULL, NULL, 0, 0);

    // Register topic
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    // Main loop to publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
