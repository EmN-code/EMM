#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32client"
#define TOPIC "state"

void network_connect(Network *network) {
    int rc;
    while (1) {
        rc = NetworkConnect(network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected\n");
            break;
        }
    }
}

void mqtt_connect(MQTTClient *client) {
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    while (1) {
        rc = MQTTConnect(client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("MQTT connected\n");
            break;
        }
    }
}

void publish_state(MQTTClient *client, const char *message) {
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS1;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void *)message;
    mqtt_message.payloadlen = strlen(message);

    int rc;
    while (1) {
        rc = MQTTPublish(client, TOPIC, &mqtt_message);
        if (rc != 0) {
            printf("Publish failed, retrying...\n");
        } else {
            printf("Message published: %s\n", message);
            break;
        }
        xtimer_sleep(1);
    }
}

int main(void) {
    Network network;
    MQTTClient client;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, buffer, sizeof(buffer), read_buffer, sizeof(read_buffer));

    network_connect(&network);
    mqtt_connect(&client);

    while (1) {
        publish_state(&client, "work");
        xtimer_sleep(5);
        publish_state(&client, "not work");
        xtimer_sleep(5);
    }

    return 0;
}