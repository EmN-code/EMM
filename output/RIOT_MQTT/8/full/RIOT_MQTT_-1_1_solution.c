#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "mqtt.h"
#include "net/network.h"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_PERIOD (5U * US_PER_SEC)

void mqtt_publish(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen(message.payload);

    int rc = MQTTPublish(client, "device/state", &message);
    if (rc != 0) {
        printf("Failed to publish message, rc=%d\n", rc);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    printf("Connecting to network...\n");
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1s\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected\n");
            break;
        }
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying in 1s\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("MQTT connected\n");
            break;
        }
    }

    while (1) {
        printf("Publishing state...\n");
        mqtt_publish(&client);
        xtimer_usleep(PUBLISH_PERIOD);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}