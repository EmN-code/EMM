#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "net/sock/tcp.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];
static char stack[THREAD_STACKSIZE_MAIN];

static void mqtt_publish_state(MQTTClient *client, const char *message)
{
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void *)message;
    mqtt_message.payloadlen = strlen(message);

    MQTTPublish(client, MQTT_TOPIC, &mqtt_message);
}

static void *mqtt_thread(void *arg)
{
    (void)arg;
    int rc;

    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "riot_client";

    rc = MQTTConnect(&client, &connect_data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        mqtt_publish_state(&client, MQTT_MESSAGE_WORK);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
        mqtt_publish_state(&client, MQTT_MESSAGE_NOT_WORK);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}