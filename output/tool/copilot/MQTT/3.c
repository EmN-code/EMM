#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "net/sock/tcp.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "riot-esp32-client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

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
            printf("Published message: %s\n", (char *)message.payload);
        }

        // Toggle the message payload
        if (strcmp((char *)message.payload, MQTT_MESSAGE_WORK) == 0) {
            message.payload = (void *)MQTT_MESSAGE_NOT_WORK;
            message.payloadlen = strlen(MQTT_MESSAGE_NOT_WORK);
        } else {
            message.payload = (void *)MQTT_MESSAGE_WORK;
            message.payloadlen = strlen(MQTT_MESSAGE_WORK);
        }

        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
}

static void *mqtt_thread(void *arg)
{
    (void)arg;

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    int rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Error connecting to MQTT broker: %d\n", rc);
        return NULL;
    }

    mqtt_publish_state();

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}