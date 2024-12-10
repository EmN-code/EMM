#include <unistd.h>   // Provides sleep() functionality
#include <string.h>    // For string operations
#include <stdio.h>     // For standard input and output functions
#include "MQTTClient.h"  // Include MQTTClient.h as the last header file

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "ESP32Client"
#define TOPIC       "device/state"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     10000L

void send_message(struct Network* network, MQTTClient* client) {
    MQTTMessage message;
    char payload[] = PAYLOAD;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(client, TOPIC, &message);
}

int main() {
    Network network;
    MQTTClient client;
    int rc;

    NetworkInit(&network);
    rc = NetworkConnect(&network, ADDRESS, PORT);
    if (rc != SUCCESS) {
        printf("Connection to network failed\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;    
    MQTTPacket_connectData(&client, &network, 1000, NULL, 0, NULL, 0);
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Connecting to MQTT broker failed\n");
        return -1;
    }

    while (1) {
        send_message(&network, &client);
        printf("Published message: %s\n", PAYLOAD);
        sleep(5);  // Wait for 5 seconds
    }

    return 0;
}