#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "MQTTClient.h"  // Make sure this is included last

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "device/state"
#define QOS 1
#define TIMEOUT 10000L

void setup_network(Network *network) {
    // Initialize the network structure
    network->mqttread = NULL; // Replace with the actual read function
    network->mqttwrite = NULL; // Replace with the actual write function
}

int connect_to_broker(MQTTClient *client, Network *network) {
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    setup_network(network);
    NetworkInit(network);

    MQTTClientInit(client, network, TIMEOUT, NULL, 0, NULL, 0);
    if ((rc = NetworkConnect(network, BROKER_IP, BROKER_PORT)) != 0) {
        printf("Failed to connect to network, return code: %d\n", rc);
        return rc;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    if ((rc = MQTTConnect(client, &connectData)) != 0) {
        printf("Failed to connect to broker, return code: %d\n", rc);
        return rc;
    }

    printf("Connected to MQTT broker\n");
    return 0;
}

void publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    int rc = MQTTPublish(client, TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Failed to publish message, return code: %d\n", rc);
    } else {
        printf("Message published: %s\n", state);
    }
}

int main(void) {
    MQTTClient client;
    Network network;
    const char *state = "work";

    if (connect_to_broker(&client, &network) == 0) {
        while (1) {
            publish_state(&client, state);
            sleep(5); // Wait for 5 seconds before publishing again
        }
    }

    return 0;
}