#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "net/sock/tcp.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "ztimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "riot_client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5U * MS_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];
static char stack[THREAD_STACKSIZE_MAIN];

static void mqtt_publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_MESSAGE_WORK;
    message.payloadlen = strlen(MQTT_MESSAGE_WORK);

    while (1) {
        int rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Error publishing message: %d\n", rc);
        } else {
            printf("Published: %s\n", (char *)message.payload);
        }

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);

        // Toggle message payload
        if (strcmp((char *)message.payload, MQTT_MESSAGE_WORK) == 0) {
            message.payload = (void *)MQTT_MESSAGE_NOT_WORK;
            message.payloadlen = strlen(MQTT_MESSAGE_NOT_WORK);
        } else {
            message.payload = (void *)MQTT_MESSAGE_WORK;
            message.payloadlen = strlen(MQTT_MESSAGE_WORK);
        }
    }
}

int main(void)
{
    int rc;

    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Error connecting to broker: %d\n", rc);
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Error connecting to MQTT: %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  (void *)mqtt_publish_state, NULL, "mqtt_publish");

    return 0;
}