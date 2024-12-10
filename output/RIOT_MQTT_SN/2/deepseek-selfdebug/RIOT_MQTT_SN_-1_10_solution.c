
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void publish_state(void)
{
    const char *state = (rand() % 2 == 0) ? "work" : "not work";
    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish message: %d\n", res);
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void)
{
    // Initialize the topic structure
    topic.name = TOPIC_NAME;
    topic.id = 0; // ID will be assigned by the gateway

    // Register the topic with the gateway
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    // Set up the gateway endpoint
    gw.family = AF_INET;
    gw.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to broker: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the broker
    emcute_discon();

    return 0;
}
