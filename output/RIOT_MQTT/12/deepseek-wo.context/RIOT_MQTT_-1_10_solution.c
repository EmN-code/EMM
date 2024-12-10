
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt_client.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    MQTTPublish(&mqtt_client, TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT example\n");

    NetworkInit(&network);
    MQTTClientInit(&mqtt_client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_mqtt_client";
    data.keepAliveInterval = 30;
    data.cleansession = 1;

    rc = MQTTConnect(&mqtt_client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
