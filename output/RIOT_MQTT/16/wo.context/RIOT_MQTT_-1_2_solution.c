#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "MQTTClient.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "device/state"

MQTTClient client;

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    const char *state = "work";  // Replace with actual state determination logic
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    MQTTPublish(client, TOPIC, &message);
    printf("State published: %s\n", state);
}

int main() {
    Network network;
    MQTTClientInit(&client, &network, 1000, malloc(100), 100, malloc(100), 100);

    char address[100];
    snprintf(address, sizeof(address), "%s:%d", BROKER_ADDRESS, BROKER_PORT);

    // Establish network connection
    if (NetworkConnect(&network, BROKER_ADDRESS, BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to broker\n");
        return -1;
    }

    // MQTT Connect
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    // Regular publishing every 5 seconds
    while (1) {
        publish_state(&client);
        sleep(5);
    }

    return 0;
}