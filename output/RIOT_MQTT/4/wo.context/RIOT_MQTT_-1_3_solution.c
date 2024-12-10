#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include "MQTTPacket.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];
static int mqtt_connected = 0;

void connect_to_broker(void) {
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    printf("Connecting to broker at %s:%d...\n", BROKER_IP, BROKER_PORT);
    if ((MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect to broker.\n");
    } else {
        mqtt_connected = 1;
        printf("Connected to broker.\n");
    }
}

void publish_state(void) {
    if (!mqtt_connected) {
        connect_to_broker();
    }
    if (mqtt_connected) {
        const char *topic = "device/state";
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (char *)"work";
        message.payloadlen = strlen((char *)message.payload);

        if (MQTTPublish(&client, topic, &message) != SUCCESS) {
            printf("Failed to publish message.\n");
            mqtt_connected = 0;
        } else {
            printf("Published 'work' to topic %s.\n", topic);
        }
    }
}

int main(void) {
    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Network connection failed.\n");
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}