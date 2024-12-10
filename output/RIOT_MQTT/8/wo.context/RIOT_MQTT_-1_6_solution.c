#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/tcp.h>
#include <xtimer.h>
#include <thread.h>
#include "MQTTClient.h"  // Must be the last header included

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "RIOT_Client"
#define TOPIC       "state"
#define QOS         1
#define TIMEOUT     10000L

static char send_buffer[100];
static char read_buffer[100];
static MQTTClient client;
static Network network;

void messageArrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n", 
            (int)data->topicName->lenstring.len, data->topicName->lenstring.data,
            (int)data->message->payloadlen, (char*)data->message->payload);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT,
        send_buffer, sizeof(send_buffer),
        read_buffer, sizeof(read_buffer));

    if (NetworkConnect(&network, ADDRESS, PORT) != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENTID;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    if (MQTTSubscribe(&client, TOPIC, QOS, messageArrived) != 0) {
        printf("Failed to subscribe to topic\n");
        return -1;
    }

    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen("work");

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);

        message.payload = (strcmp((char *)message.payload, "work") == 0) ? "not work" : "work";
        message.payloadlen = strlen((char *)message.payload);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    return 0;
}