
#include <stdio.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define TOPIC_NAME "state"
#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw;

static void publish_state(void)
{
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2; // Toggle between "work" and "not work"

    printf("Publishing state: %s\n", state);

    int res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    }
}

int main(void)
{
    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("No default network device found");
        return 1;
    }

    // Initialize the EMCUTE MQTT-SN client
    emcute_init();

    // Set up the gateway endpoint
    gw.family = AF_INET;
    gw.port = MQTT_SERVER_PORT;
    inet_pton(AF_INET, MQTT_SERVER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
