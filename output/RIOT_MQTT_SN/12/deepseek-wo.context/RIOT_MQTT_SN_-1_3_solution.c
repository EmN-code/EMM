
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void publish_state(void)
{
    const char *state = "work"; // or "not work" based on your logic
    emcute_pub(&topic, state, strlen(state), 0);
}

int main(void)
{
    // Initialize the MQTT-SN topic
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the gateway

    // Set up the gateway endpoint
    gw.family = AF_INET;
    gw.port = MQTT_SN_BROKER_PORT;
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Could not connect to MQTT-SN broker");
        return 1;
    }

    // Register the topic with the broker
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Could not register topic");
        emcute_discon();
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the broker (this line will never be reached in this example)
    emcute_discon();

    return 0;
}
