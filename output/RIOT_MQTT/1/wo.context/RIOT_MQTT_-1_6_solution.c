#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <net/sock/udp.h>
#include <net/protnum.h>
#include <timex.h>
#include <time.h>
#include <xtimer.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_KEEPALIVE 60
#define MQTT_TOPIC "device/state"
#define MQTT_MSG "work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;

static void publish_message(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_MSG;
    message.payloadlen = strlen(MQTT_MSG);

    MQTTPublish(client, MQTT_TOPIC, &message);
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.keepAliveInterval = MQTT_KEEPALIVE;
    connectData.cleansession = 1;

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connection failed\n");
        return -1;
    }

    while (1) {
        publish_message(&client);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}