
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
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char state_message[20];
static emcute_topic_t topic;

static void publish_state(void) {
    static int state = 0;
    const char *states[] = {"work", "not work"};

    // Toggle state
    state = !state;
    strcpy(state_message, states[state]);

    // Publish the state
    emcute_pub(&topic, state_message, strlen(state_message), 0);
    printf("Published: %s\n", state_message);
}

int main(void) {
    sock_udp_ep_t gw = { .port = MQTT_SN_BROKER_PORT };
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &gw.addr.ipv4);

    // Initialize the EMCUTE stack
    emcute_run(MQTT_SN_BROKER_PORT, CLIENT_ID);

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    puts("Topic registered");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}