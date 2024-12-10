#include <ringbuffer.h>              // Ensure alphabetical descending order
#include <net/sock/udp.h>             // Required for socket operations
#include "MQTTClient.h"              // Should be last as per user guidance

#include <stdio.h>                    // Standard I/O operations
#include <string.h>                   // String manipulation functions

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENT_ID   "RIOT_Client_001"
#define TOPIC       "device/state"
#define PAYLOAD     "work"
#define QOS         1

Network network;
MQTTClient client;
MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
unsigned char sendbuf[80], readbuf[80];

void init_network(void) {
    // Initialize network structure
    network.mqttread = /*function_pointer for reading from network*/;
    network.mqttwrite = /*function_pointer for writing to network*/;

    if (NetworkConnect(&network, ADDRESS, PORT) != SUCCESS) {
        printf("Network connection failed!");
        // Handle connection failure
    }
}

void init_mqtt_client(void) {
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connection failed!");
        // Handle failure
    }
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != SUCCESS) {
        printf("Failed to publish message!");
        // Handle failure
    } else {
        printf("Message published!");
    }
}

int main(void) {
    init_network();
    init_mqtt_client();

    while (1) {
        publish_state();
        sleep(5);  // Wait for 5 seconds before next publish
    }

    return 0;
}