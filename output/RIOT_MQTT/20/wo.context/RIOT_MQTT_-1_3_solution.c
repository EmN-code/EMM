#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "ESP32Client"
#define TOPIC "device/state"
#define QOS 1
#define TIMEOUT 10000L

static MQTTClient client;
static Network network;

void connect_to_broker() {
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Connected to broker\n");
}

void publish_state(const char* state) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, TOPIC, &message);
    printf("Published: %s\n", state);
}

int main(void) {
    int rc;
    unsigned char sendbuf[100], readbuf[100];
    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network, return code %d\n", rc);
        return rc;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connect_to_broker();

    while (1) {
        publish_state("work");
        sleep(5);
    }

    return 0;
}