#include <ztimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000U)

static emcute_topic_t topic;
static char topic_name[] = "device/state";

// Function to connect to the MQTT-SN broker
int connect_broker_with_retry(sock_udp_ep_t *gw){
    int attempts = 3;
    while (attempts--) {
        if (emcute_con(gw, true, NULL, NULL, 0, 0) == EMCUTE_OK) {
            return 0; // Success
        }
        puts("Error: Unable to connect to MQTT-SN broker, retrying...");
        ztimer_sleep(ZTIMER_MSEC, 5000U);  // wait and retry
    }
    return -1; // Fail after retries
}

// Function to publish a state message
void publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    emcute_pub(&topic, state, strlen(state), flags);
}

int main(void) {
    sock_udp_ep_t gw_endpoint;
    gw_endpoint.port = MQTT_BROKER_PORT;

    // Convert IP string to network address
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw_endpoint.addr.ipv4) != 1) {
        puts("Error: Unable to parse broker IP.");
        return 1;
    }

    // Initialize EMCUTE
    if (emcute_run(MQTT_BROKER_PORT, "esp32_client") != EMCUTE_OK) {
        puts("Error: Unable to initialize emcute");
        return 1;
    }

    // Connect to the MQTT-SN broker with retry mechanism
    if (connect_broker_with_retry(&gw_endpoint) != 0) {
        puts("Could not establish connection.");
        return 1;
    }

    // Register the topic
    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Unable to register topic");
        return 1;
    }

    // Periodically publish the state
    while (1) {
        publish_state("work");
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
        publish_state("not work");
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return 0;
}