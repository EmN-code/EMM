#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for sleep function

#include "mqtt_client.h"
#include "network.h"
#include "timer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "/esp32/state"

void publish_state(MQTTClient* client) {
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen((char *)message.payload);

    int rc = MQTTPublish(client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code: %d\n", rc);
    } else {
        printf("Message published successfully\n");
    }
}

int main() {
    Network network;
    MQTTClient client;
    int rc;

    // Initialize network interface
    NetworkInit(&network);
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying in 1 second\n");
            sleep(1);
            continue;
        }
        printf("Network connected successfully\n");
        break;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    while (1) {
        rc = MQTTStartTask(&client);
        if (rc != 0) {
            printf("Failed to start MQTT task, retrying in 1 second\n");
            sleep(1);
            continue;
        }
        printf("MQTT task started successfully\n");
        break;
    }

    // Connect to the MQTT broker
    // Add implementing for handling connection


    // Periodically publish the state every 5 seconds
    while (1) {
        publish_state(&client);
        sleep(5);
    }

    // MQTT disconnect and network cleanup would go here
    // But for a continuous running application on an embedded system, main might not return.

    return 0;
}