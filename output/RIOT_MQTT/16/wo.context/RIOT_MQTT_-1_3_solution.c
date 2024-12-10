#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"

#define ADDRESS     "47.102.103.1:1883"
#define CLIENTID    "ESP32Client"
#define TOPIC       "device/status"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     10000L

void messageArrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, 
            data->topicName->lenstring.data, 
            data->message->payloadlen, 
            (char*)data->message->payload);
}

int main() {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = NetworkConnect(&network, ADDRESS, 1883)) != 0) {
        printf("Return code from network connect is %d\n", rc);
        return -1;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENTID;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Return code from MQTT connect is %d\n", rc);
        return -1;
    }
    printf("MQTT Connected\n");

    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.payload = (void*) PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("Return code from MQTT publish is %d\n", rc);
        }
        sleep(5);
    }

    return 0;
}