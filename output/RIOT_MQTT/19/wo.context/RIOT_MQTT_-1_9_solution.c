#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "ESP32_MQTT_Client"
#define MSG_WORK_STATE "work"
#define MSG_NOT_WORK_STATE "not work"
#define PUBLISH_TOPIC "device/state"
#define PUBLISH_INTERVAL 5

static MQTTClient client;
static Network network;

void publish_state(bool work) {
    MQTTMessage message;
    char *msg = work ? MSG_WORK_STATE : MSG_NOT_WORK_STATE;

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)msg;
    message.payloadlen = strlen(msg);

    MQTTPublish(&client, PUBLISH_TOPIC, &message);
    printf("Published: %s\n", msg);
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &connect_data) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");
    bool work = true;
    while (1) {
        publish_state(work);
        work = !work;
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}