#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL_SEC 5
#define BUFFER_SIZE 100

static unsigned char sendbuf[BUFFER_SIZE], readbuf[BUFFER_SIZE];
static MQTTClient client;
static Network network;

void mqtt_message_arrived(MessageData* data) {
    printf("Message arrived: %.*s\n", (int)data->message->payloadlen, (char*)data->message->payload);
}

int main(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "client_id";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connect failed\n");
        return -1;
    }

    while (1) {
        char payload[20];
        sprintf(payload, "work"); // Change payload to "not work" to alternate state

        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)payload;
        message.payloadlen = strlen(payload);

        if (MQTTPublish(&client, "state", &message) != SUCCESS) {
            printf("Failed to publish message\n");
        }

        sleep(PUBLISH_INTERVAL_SEC);
    }

    return 0;
}