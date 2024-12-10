#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

// Define constants
#define MQTT_SN_BROKER "47.102.103.1"
#define MQTT_SN_PORT (1883)
#define STATE_PUB_INTERVAL (5U) // 5 seconds interval

// Define MQTT-SN topic
static emcute_topic_t state_topic = {
    .name = "state",
    .id = 0
};

// Function to publish state
void publish_state(emcute_topic_t *topic, const char *state) {
    if (emcute_pub(topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Publication failed\n");
    } else {
        printf("State published: %s\n", state);
    }
}

int main(void) {
    // Initialize emcute
    emcute_run(MQTT_SN_PORT, "esp32-client");

    // Connection endpoint
    sock_udp_ep_t gateway = {
        .family = AF_INET,
        .port = MQTT_SN_PORT
    };
    inet_pton(AF_INET, MQTT_SN_BROKER, &gateway.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gateway, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Could not connect to MQTT-SN broker\n");
        return 1;
    }

    // Register the topic
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Could not register topic\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state(&state_topic, "work");
        xtimer_sleep(STATE_PUB_INTERVAL);
    }

    return 0;
}