#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "xtimer.h"  // For time management
#include "ztimer.h"  // For timer utilities
#include "net/sock/udp.h" // For network operations
#include "net/sock/util.h" // For utility functions
#include "MQTTClient.h"  // Include this as last header for MQTT functions

// Predefined constants for MQTT broker
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "device/state"
#define PUB_INTERVAL (5U * MS_PER_SEC)  // 5 seconds interval

// MQTT setup
static MQTTClient client;
static Network network;

// Function to connect to MQTT broker
tatic int connect_to_broker(void) {
    int rc = 0;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "client_id";

    NetworkInit(&network);
    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    if (rc != SUCCESS) {
        printf("Failed to connect to broker: %d\n", rc);
        return rc;
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("MQTT connect failed: %d\n", rc);
    }

    return rc;
}

// Main function
int main(void) {
    int rc;
    rc = connect_to_broker();
    if (rc != SUCCESS) {
        return -1;
    }

    while (1) {
        MQTTMessage message;
        char payload[] = "work or not work";
        message.qos = QOS0;
        message.retained = false;
        message.payload = payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, PUB_TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish: %d\n", rc);
        }

        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}