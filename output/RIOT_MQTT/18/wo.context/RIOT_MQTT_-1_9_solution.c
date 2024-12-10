#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds

static char sendbuf[256];
static char readbuf[256];

int main(void) {
    printf("Starting MQTT client example\n");

    Network network;
    MQTTClient client;
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) == 0) {
        printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    } else {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("MQTT connect failed\n");
        return 1;
    }

    printf("MQTT client connected\n");

    while (1) {
        const char* message = "work";
        MQTTMessage pubMessage;
        pubMessage.qos = QOS0;
        pubMessage.retained = 0;
        pubMessage.payload = (void*)message;
        pubMessage.payloadlen = strlen(message);

        if (MQTTPublish(&client, TOPIC, &pubMessage) != 0) {
            printf("Error publishing message\n");
        } else {
            printf("Published message: %s\n", message);
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}