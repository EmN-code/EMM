
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    // Register the topic if not already registered
    if (topic.id == 0) {
        if (emcute_reg(&topic) != EMCUTE_OK) {
            puts("Error: could not register topic");
            return;
        }
    }

    // Publish the state
    const char *state = states[state_index];
    if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: could not publish message");
    } else {
        printf("Published: %s\n", state);
    }

    // Toggle state
    state_index = 1 - state_index;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_BROKER_PORT };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Run the emcute thread
    emcute_run(MQTT_SN_BROKER_PORT, CLIENT_ID);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(MSG_INTERVAL);
    }

    return 0;
}
