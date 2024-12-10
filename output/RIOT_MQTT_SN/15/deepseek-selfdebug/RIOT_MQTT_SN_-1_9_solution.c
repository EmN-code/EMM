
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work or not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = SERVER_PORT };

void publish_state(void) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "%s", STATE_MESSAGE);
    emcute_pub(&topic, buf, strlen(buf), 0);
}

int main(void) {
    // Initialize the network interface
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    // Set the default gateway IP address
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("inet_pton failed");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Failed to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Failed to register topic");
        return 1;
    }
    puts("Topic registered");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
