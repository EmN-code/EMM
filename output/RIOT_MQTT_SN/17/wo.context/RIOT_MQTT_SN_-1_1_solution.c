#include <xtimer.h>  // for time-related functions
#include <stdio.h>   // for standard input/output functions
#include <string.h>  // for string manipulation functions
#include "net/emcute.h"  // for emcute-related functions for MQTT-SN

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define INTERVAL 5  // publish every 5 seconds

static emcute_con_t connection;
static emcute_topic_t topic;

void on_msg(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received message: %.*s\n", (int)len, (char *)data);
}

int mqtt_init(void) {
    // Initialize the emcute stack
    if (emcute_init() != EMCUTE_OK) {
        printf("emcute initialization failed\n");
        return -1;
    }

    // Setup the connection to the broker
    memset(&connection, 0, sizeof(connection));
    connection.port = BROKER_PORT;
    ipv6_addr_from_str(&connection.addr.ipv6, BROKER_IP);

    if (emcute_con(&connection, false, on_msg) != EMCUTE_OK) {
        printf("connection to broker failed\n");
        return -1;
    }

    // Setup the topic
    topic.name = MQTT_TOPIC;
    topic.id = 0;  // Let the broker assign an ID

    if (emcute_sub(&topic, 0) != EMCUTE_OK) {
        printf("unable to subscribe to topic\n");
        return -1;
    }

    return 0;
}

void publish_state(void) {
    const char *states[] = {"work", "not work"};
    static int state_index = 0;

    // Publish the state
    if (emcute_pub(&topic, states[state_index], strlen(states[state_index]), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("publish failed\n");
    } else {
        printf("published: %s\n", states[state_index]);
    }

    // Toggle state for next publish
    state_index = (state_index + 1) % 2;
}

int main(void) {
    if (mqtt_init() != 0) {
        printf("failed to initialize MQTT\n");
        return 1;
    }

    // Main loop
    while (1) {
        publish_state();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}