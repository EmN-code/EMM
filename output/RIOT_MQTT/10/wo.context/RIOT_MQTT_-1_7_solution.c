#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // Assuming Timer can be aligned with standard time functionalities
#include <netdb.h> // For network-related structures and functions
#include <pthread.h> // For thread handling, assuming POSIX threads
#include <unistd.h>  // For close() function

#include "MQTTClient.h" // Last to be included

// Definitions for MQTT broker details
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5
#define CLIENT_ID "esp32_device"
#define TOPIC "device/state"
#define QOS 1
#define RETAINED false

void* mqtt_task(void *arg) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    int rc = 0;

    NetworkInit(&network);
    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to broker\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        char *payload = "work";
        message.qos = QOS;
        message.retained = RETAINED;
        message.dup = 0;
        message.payload = (void*) payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message\n");
        }

        sleep(PUBLISH_INTERVAL);
    }

    NetworkDisconnect(&network);
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, mqtt_task, NULL);
    pthread_join(tid, NULL);

    return 0;
}