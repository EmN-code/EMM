
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/inet_pton.h>  // Include the necessary header for inet_pton
#include <emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static emcute_topic_t t;

void publish_state(void) {
    const char *state = "work";  // or "not work" based on your logic
    emcute_pub(&t, state, strlen(state), 0);
}

int main(void) {
    // Initialize the network stack
    if (netif_default == NULL) {
        puts("No default network interface found.");
        return 1;
    }

    // Resolve the server IP address
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: could not resolve server address.");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker.");
        return 1;
    }

    // Register the topic
    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: could not register topic.");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
