#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <xtimer.h>
#include "net/mqtt.h"
#include "net/sock/udp.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5000000U) // 5 seconds in microseconds
#define MQTT_TOPIC "device/state"
#define MQTT_STATE_WORK "work"
#define MQTT_STATE_NOT_WORK "not work"

static sock_udp_ep_t broker_endpoint;

static int publish_state(mqtt_client_t *client) {
    mqtt_publish(client, MQTT_TOPIC, MQTT_STATE_WORK, strlen(MQTT_STATE_WORK), 0);
    printf("Published state: work\n");
    return 0;
}

int main(void) {
    // Define broker address and port
    broker_endpoint.family = AF_INET;
    broker_endpoint.port = BROKER_PORT;
    if (inet_pton(AF_INET, BROKER_IP, &broker_endpoint.addr.ipv4) != 1) {
        puts("Error: unable to parse broker IP");
        return -1;
    }

    // Initialize MQTT Client
    mqtt_client_t client;
    mqtt_client_init(&client, broker_endpoint);

    // Connect to MQTT Broker
    if (mqtt_connect(&client, broker_endpoint) != MQTT_OK) {
        puts("Error: unable to connect to the broker");
        return -1;
    }

    while (1) {
        // Publish state
        publish_state(&client);
        // Wait for the next publish interval
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}