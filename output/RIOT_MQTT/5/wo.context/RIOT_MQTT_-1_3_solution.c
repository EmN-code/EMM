#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>
#include <string.h>
#include "MQTTClient.h"  // Ensure this is the last to be included

#define MQTT_ADDRESS     "47.102.103.1"
#define MQTT_PORT        1883
#define MQTT_CLIENT_ID   "esp32Client"
#define MQTT_TOPIC       "device/state"
#define MQTT_DATA        "work or not work"
#define MQTT_PUB_INTERVAL (5U)  // 5 seconds

static MQTTClient client;
static Network network;

void messageArrived(MessageData* md) {
    printf("Message arrived on topic %.*s: %.*s\n", md->topicName->lenstring.len, md->topicName->lenstring.data,
           md->message->payloadlen, (char*)md->message->payload);
}

int main(void) {
    int rc = 0;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);

    if ((rc = NetworkConnect(&network, MQTT_ADDRESS, MQTT_PORT)) != SUCCESS) {
        printf("Failed to connect to network, return code %d\n", rc);
        return rc;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Failed to connect to MQTT Broker, return code %d\n", rc);
        return rc;
    }
    printf("Connected to MQTT Broker\n");

    if ((rc = MQTTSubscribe(&client, MQTT_TOPIC, QOS0, messageArrived)) != SUCCESS) {
        printf("Failed to subscribe, return code %d\n", rc);
        return rc;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)MQTT_DATA;
        message.payloadlen = strlen(MQTT_DATA);

        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != SUCCESS) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            printf("Message published: %s\n", MQTT_DATA);
        }

        xtimer_sleep(MQTT_PUB_INTERVAL);
    }

    return 0;
}