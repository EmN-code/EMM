#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mqtt.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32-client"
#define TOPIC "device/state"

static mqtt_client_t client;
static emcute_topic_t topic;

void publish_state(char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    
    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        printf("Error publishing message\n");
    } else {
        printf("Message published: %s\n", state);
    }
}

void connect_to_broker(void) {
    Network network;
    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Error connecting to broker\n");
        exit(EXIT_FAILURE);
    }
    MQTTClientInit(&client, &network, 10000, NULL, 0, NULL, 0);
    if (MQTTConnect(&client) != 0) {
        printf("Error establishing MQTT connection\n");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        // Simulate getting a state
        char *state = (rand() % 2 == 0) ? "work" : "not work";
        publish_state(state);

        // Sleep for 5 seconds
        sleep(5);
    }

    return 0;
}