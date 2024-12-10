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
#define MQTT_TOPIC "esp32/state"
#define MQTT_KEEP_ALIVE_INTERVAL 60
#define PUBLISH_INTERVAL (5 * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];
static char stack[THREAD_STACKSIZE_MAIN];

static void mqtt_publish_state(void)
{
    MQTTMessage message;
    char payload[20];
    static bool work_state = true;

    snprintf(payload, sizeof(payload), "%s", work_state ? "work" : "not work");
    work_state = !work_state;

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != SUCCESS) {
        printf("Failed to publish message\n");
    } else {
        printf("Published: %s\n", payload);
    }
}

static void *mqtt_thread(void *arg)
{
    (void)arg;
    int rc;

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = MQTT_KEEP_ALIVE_INTERVAL;

    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return NULL;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish_state();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}