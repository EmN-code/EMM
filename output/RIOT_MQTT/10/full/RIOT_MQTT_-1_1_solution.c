#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"
#include "Network.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_TOPIC "device/state"
#define CLIENT_ID "esp32_device"

void mqtt_publish_loop(MQTTClient* client) {
    MQTTMessage message;
    char payload[20];

    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;

    while (1) {
        strcpy(payload, "work");  // Set status as "work"
        message.payloadlen = strlen(payload);

        int rc = MQTTPublish(client, PUBLISH_TOPIC, &message);
        if (rc != 0) {
            printf("MQTTPublish failed: %d\n", rc);
        } else {
            printf("State published successfully\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

        if (rc != 0) {
            printf("Network connection failed: %d. Retrying...\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to network successfully.\n");
            break;
        }
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        int rc = MQTTConnect(&client, CLIENT_ID);

        if (rc != 0) {
            printf("MQTT connection failed: %d. Retrying...\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected to MQTT broker successfully.\n");
            break;
        }
    }

    mqtt_publish_loop(&client);

    return 0;
}