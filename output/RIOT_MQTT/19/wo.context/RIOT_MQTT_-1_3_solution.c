#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <kernel_types.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

// Define network struct if not defined
struct Network {
    int (*mqttread)(Network*, unsigned char* read_buffer, int, int);
    int (*mqttwrite)(Network*, unsigned char* send_buffer, int, int);
};

void NetworkInit(Network *n) {
    // Initialize network callbacks
    n->mqttread = NULL; // Assign actual read function here
    n->mqttwrite = NULL; // Assign actual write function here
}

int NetworkConnect(Network *n, char* addr, int port) {
    // Connect to MQTT broker (dummy implementation)
    printf("Connecting to broker %s:%d\n", addr, port);
    return 0; // Return success
}

int main(void) {
    Network network;
    unsigned char mqtt_send_buf[128];
    unsigned char mqtt_read_buf[128];
    MQTTClient client;

    NetworkInit(&network);
    int rc = NetworkConnect(&network, "47.102.103.1", 1883);
    if (rc != SUCCESS) {
        printf("Failed to connect to network\n");
        return rc;
    }

    MQTTClientInit(&client, &network, 1000, mqtt_send_buf, sizeof(mqtt_send_buf), mqtt_read_buf, sizeof(mqtt_read_buf));

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "mqtt_client_id";

    if ((rc = MQTTConnect(&client, &connect_data)) != SUCCESS) {
        printf("Failed to connect to MQTT broker, return code: %d\n", rc);
        return rc;
    }

    // Data to publish
    char* payload = "work";
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    while (1) {
        // Publish every 5 seconds
        printf("Publishing message\n");
        rc = MQTTPublish(&client, "esp32/state", &message);
        if (rc != SUCCESS) {
            printf("Publish failed, return code: %d\n", rc);
        }
        xtimer_sleep(5);
    }

    return 0;
}