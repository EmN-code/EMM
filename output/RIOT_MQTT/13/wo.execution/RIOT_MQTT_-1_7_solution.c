#include "stdint.h"
#include "stddef.h"
#include "stdio.h"  // for printf
#include "MQTTClient.h"

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "ESP32Client"
#define TOPIC       "esp32/state"
#define QOS         1
#define TIMEOUT     10000L

MQTTClient client;
Network network;
unsigned char sendbuf[100], readbuf[100];
char* state = "work or not work";

void messageArrived(MessageData* data) {
    printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data,
           data->message->payloadlen, (char*)data->message->payload);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENTID;

    printf("Connecting to %s:%d\n", ADDRESS, PORT);
    if (NetworkConnect(&network, ADDRESS, PORT) != 0) {
        printf("Failed to connect to network.\n");
        return -1;
    }

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to broker.\n");
        NetworkDisconnect(&network);
        return -1;
    }

    printf("Connected to broker.\n");

    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)state;
    message.payloadlen = strlen(state);

    while (1) {
        message.id = rand();  // Unique message ID
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message.\n");
        } else {
            printf("Message published: %s\n", state);
        }

        int interval_ms = 5000;  // 5000 ms = 5 seconds
        struct timespec ts;
        ts.tv_sec = interval_ms / 1000;
        ts.tv_nsec = (interval_ms % 1000) * 1000000L;
        nanosleep(&ts, NULL);
    }

    return 0;
}