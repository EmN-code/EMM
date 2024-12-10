#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "net/sock/tcp.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "riot-esp32"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static char mqtt_sendbuf[MQTT_SENDBUF_SIZE];
static char mqtt_readbuf[MQTT_READBUF_SIZE];
static char stack[THREAD_STACKSIZE_MAIN];

static void mqtt_publish(const char *message)
{
    MQTTMessage msg;
    msg.qos = QOS0;
    msg.retained = 0;
    msg.payload = (void *)message;
    msg.payloadlen = strlen(message);

    int rc = MQTTPublish(&client, MQTT_TOPIC, &msg);
    if (rc != 0) {
        printf("Error while publishing message: %d\n", rc);
    }
}

static void *mqtt_thread(void *arg)
{
    (void)arg;

    while (1) {
        mqtt_publish(MQTT_MESSAGE_WORK);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
        mqtt_publish(MQTT_MESSAGE_NOT_WORK);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void)
{
    NetworkInit(&network);
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Error connecting to MQTT broker: %d\n", rc);
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Error connecting to MQTT broker: %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}