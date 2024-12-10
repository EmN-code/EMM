#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "net/mqtt/client.h"

// MQTT broker configuration
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"

// Function prototypes
void mqtt_event_cb(mqtt_client_t *client, void *param, mqtt_event_t event, void *data);
void publish_state(mqtt_client_t *client, const char *state);

// Main thread
int main(void) {
    // Initialize the network
    sock_udp_ep_t broker = {
        .port = MQTT_BROKER_PORT,
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr);

    // Create the MQTT client
    mqtt_client_t client;
    mqtt_client_init(&client, &broker, 10000, NULL, 0, NULL, 0, mqtt_event_cb, NULL);

    // Connect to the MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    mqtt_client_connect(&client, &options, NULL, 0);

    // Main loop
    while (1) {
        // Publish the state
        char state[] = "work";
        publish_state(&client, state);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}

// MQTT event callback
void mqtt_event_cb(mqtt_client_t *client, void *param, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECT:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECT:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISH:
            printf("Published message\n");
            break;
        case MQTT_EVENT_SUBSCRIBE:
            printf("Subscribed to topic\n");
            break;
        case MQTT_EVENT_UNSUBSCRIBE:
            printf("Unsubscribed from topic\n");
            break;
        default:
            break;
    }
}

// Function to publish the state
void publish_state(mqtt_client_t *client, const char *state) {
    mqtt_message_t message = {
        .qos = MQTT_QOS_0_AT_MOST_ONCE,
        .retained = 0,
        .dup = 0,
        .payload = (uint8_t *)state,
        .payload_len = strlen(state),
    };

    mqtt_client_publish(client, MQTT_TOPIC, &message, NULL, 0);
}