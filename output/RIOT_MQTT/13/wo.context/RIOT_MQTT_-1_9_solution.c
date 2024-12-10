#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "MQTTClient.h"

static int mqtt_message_arrived(MessageData* data) {
    printf("Message Arrived: %.*s\n", data->message->payloadlen, (char*)data->message->payload);
    return 1;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    int rc = 0;

    NetworkInit(&network);
    rc = NetworkConnect(&network, "47.102.103.1", 1883);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    char* topic = "state";
    char* message = "work";
    MQTTMessage pubMessage;
    pubMessage.qos = QOS0;
    pubMessage.retained = 0;
    pubMessage.payload = message;
    pubMessage.payloadlen = strlen(message);

    while (1) {
        rc = MQTTPublish(&client, topic, &pubMessage);
        if (rc != SUCCESS) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published: %s\n", message);
        }

        // Toggle message for demonstration
        if (strcmp(message, "work") == 0) {
            message = "not work";
        } else {
            message = "work";
        }
        pubMessage.payload = message;
        pubMessage.payloadlen = strlen(message);

        sleep(5);
    }

    return 0;
}