#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/sock/tcp.h"
#include "MQTTClient.h"

// Define the MQTT broker address and port
#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883

// MQTT topic
#define MQTT_TOPIC "state"

// MQTT message "work" or "not work"
char *message = "work";

// Function to publish MQTT message
void publish_state(Network *network, MQTTClient *client) {
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32";

    if (MQTTConnect(client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    MQTTMessage pubMessage;
    memset(&pubMessage, 0, sizeof(pubMessage));
    pubMessage.qos = QOS0;
    pubMessage.retained = 0;
    pubMessage.payload = message;
    pubMessage.payloadlen = strlen(message);

    if (MQTTPublish(client, MQTT_TOPIC, &pubMessage) != SUCCESS) {
        printf("Failed to publish message\n");
    }

    MQTTDisconnect(client);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    // Initialize network
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        publish_state(&network, &client);
        sleep(5); // wait for 5 seconds
    }

    return 0;
}