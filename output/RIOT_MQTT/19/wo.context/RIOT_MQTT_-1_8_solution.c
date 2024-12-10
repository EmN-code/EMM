#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define QOS 1
#define TIME_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[128], readbuf[128];
static char client_id[] = "ESP32_Client";

void connect_to_broker(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = client_id;

    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("Failed to connect to broker\n");
    }
    else {
        printf("Connected to broker\n");
    }
}

void publish_state(const char* state) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    if (MQTTPublish(&client, TOPIC, &message) != SUCCESS) {
        printf("Failed to publish message\n");
    }
    else {
        printf("Published message: %s\n", state);
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state("work");
        xtimer_sleep(TIME_INTERVAL);
        publish_state("not work");
        xtimer_sleep(TIME_INTERVAL);
    }

    return 0;
}