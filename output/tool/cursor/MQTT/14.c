#include <stdio.h>
#include <string.h>
#include "paho_mqtt.h"
#include "net/sock/tcp.h"
#include "ztimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5U * 1000U) // 5 seconds in milliseconds

static Network network;
static MQTTClient client;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void mqtt_publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    int rc = MQTTPublish(&client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("Error publishing message: %d\n", rc);
    }
}

int main(void) {
    int rc;

    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Error connecting to broker: %d\n", rc);
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Error connecting to MQTT: %d\n", rc);
        return -1;
    }

    while (1) {
        mqtt_publish_state(MQTT_MESSAGE_WORK);
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return 0;
}