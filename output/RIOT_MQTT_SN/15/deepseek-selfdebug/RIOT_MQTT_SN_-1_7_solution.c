
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/inet_pton.h>  // Include the inet_pton header
#include <emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static emcute_topic_t t;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;  // Suppress unused parameter warning
    (void)data;   // Suppress unused parameter warning
    (void)len;    // Suppress unused parameter warning
    // This function is intentionally left empty to avoid the unused function warning
}

int main(void)
{
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error initializing network stack");
        return 1;
    }

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return 1;
    }

    // Register the topic
    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error registering topic");
        return 1;
    }

    // Publish state every 5 seconds
    char state[] = "work";
    while (1) {
        if (emcute_pub(&t, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("State published");
        }
        xtimer_sleep(5);
    }

    // Disconnect from the MQTT-SN broker
    emcute_discon();

    return 0;
}
