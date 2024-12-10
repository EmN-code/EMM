
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    char payload[32];

    // Register the topic
    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return;
    }

    // Prepare the payload
    snprintf(payload, sizeof(payload), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    // Publish the state
    if (emcute_pub(&topic, payload, strlen(payload), 0) != EMCUTE_OK) {
        puts("Error: unable to publish state");
    } else {
        printf("Published: %s\n", payload);
    }
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error: unable to initialize the network stack");
        return 1;
    }

    // Set the server IP address
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: unable to set server IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    // Run the emcute thread
    emcute_run(SERVER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
