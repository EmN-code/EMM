#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "paho.mqtt.embedded-c/MQTTClient.h"  // Ensure this is the last header if it is required.

#define ADDRESS_IP "47.102.103.1"
#define PORT_NUMBER 1883
#define CLIENT_ID "mqtt_client"
#define TOPIC "device/state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

Network network;
MQTTClient client;
unsigned char sendbuf[80], readbuf[80];
MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

void publish_state(MQTTClient *client, char *state) {
   MQTTMessage message;
   message.qos = QOS;
   message.retained = 0;
   message.payload = (void*) state;
   message.payloadlen = strlen(state);

   MQTTPublish(client, TOPIC, &message);
}

int main(void) {
    printf("Initializing MQTT Client\n");
    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    printf("Connecting to %s:%d\n", ADDRESS_IP, PORT_NUMBER);
    if (NetworkConnect(&network, ADDRESS_IP, PORT_NUMBER) != SUCCESS) {
        printf("Network connection failed!\n");
        return -1;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connection failed!\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish_state(&client, PAYLOAD);
        sleep(5);  // Wait for 5 seconds before publishing again
    }

    return 0;
}