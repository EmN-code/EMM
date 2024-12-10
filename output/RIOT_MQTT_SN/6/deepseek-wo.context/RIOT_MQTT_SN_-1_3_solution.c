
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t broker = { .port = MQTT_BROKER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    char msg[20];
    strncpy(msg, MQTT_MESSAGE, sizeof(msg));
    msg[sizeof(msg) - 1] = '\0';

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    // Initialize the network stack
    if (netdev_default == NULL) {
        puts("Error: no network devices available");
        return 1;
    }

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4) <= 0) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Initialize emcute
    emcute_run(MQTT_BROKER_PORT, MQTT_CLIENT_ID);

    // Register the topic
    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Main loop
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
