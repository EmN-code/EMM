#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "ztimer.h"
#include "net/sock/tcp.h"
#include "paho_mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "riot-esp32"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5U * MS_PER_SEC)

static MQTTClient client;
static Network network;
static char mqtt_sendbuf[512];
static char mqtt_readbuf[512];
static ztimer_t timer;

static void mqtt_publish_state(void)
{
    static bool work_state = true;
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)(work_state ? MQTT_MESSAGE_WORK : MQTT_MESSAGE_NOT_WORK);
    message.payloadlen = strlen(work_state ? MQTT_MESSAGE_WORK : MQTT_MESSAGE_NOT_WORK);

    int rc = MQTTPublish(&client, MQTT_TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Error publishing message: %d\n", rc);
    } else {
        printf("Published: %s\n", (char *)message.payload);
    }

    work_state = !work_state;
}

static void *mqtt_thread(void *arg)
{
    (void)arg;

    while (1) {
        mqtt_publish_state();
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    printf("MQTT publisher example\n");

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, mqtt_sendbuf, sizeof(mqtt_sendbuf), mqtt_readbuf, sizeof(mqtt_readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Error connecting to broker: %d\n", rc);
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Error connecting to MQTT: %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}