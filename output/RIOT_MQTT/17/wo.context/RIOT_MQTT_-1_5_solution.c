#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "MQTTPacket.h"
#include "MQTTClient.h"
#include "kernel_defines.h"

static void messageArrived(MessageData* data);

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc = 0;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_ESP32_Client";

    if ((rc = NetworkConnect(&network, "47.102.103.1", 1883)) != 0) {
        printf("Failed to connect network, return code %d\n", rc);
        return rc;
    }

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect MQTT, return code %d\n", rc);
        return rc;
    }

    printf("MQTT Connected\n");
    MQTTMessage message;
    char payload[20];

    while (1) {
        sprintf(payload, "Work state: %s", "work");
        message.qos = QOS0;
        message.retained = 0;
        message.payload = payload;
        message.payloadlen = strlen(payload);

        if ((rc = MQTTPublish(&client, "esp32/state", &message)) != 0) {
            printf("Failed to publish, return code %d\n", rc);
        }
        else {
            printf("Message published: %s\n", payload);
        }

        sleep(5);
    }

    if ((rc = MQTTDisconnect(&client)) != 0) {
        printf("Failed to disconnect MQTT, return code %d\n", rc);
        return rc;
    }

    NetworkDisconnect(&network);
    return 0;
}

static void messageArrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data, data->message->payloadlen, data->message->payload);
}