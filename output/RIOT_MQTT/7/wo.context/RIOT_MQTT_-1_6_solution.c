#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/af.h"
#include "net/conn/ip.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define PUB_TOPIC "device/state"
#define STATE_MSG "work"
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static MQTTClient client;
static Network network;
unsigned char sendbuf[100], readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)STATE_MSG;
    message.payloadlen = strlen(STATE_MSG);

    MQTTPublish(&client, PUB_TOPIC, &message);
    printf("State published: %s\n", STATE_MSG);
}

int main(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    connectData.keepAliveInterval = 10;
    connectData.cleansession = 1;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to broker\n");
        return -1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}