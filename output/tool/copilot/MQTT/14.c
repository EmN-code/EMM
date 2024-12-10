#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "net/sock/tcp.h"
#include "paho_mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "riot-esp32-client"
#define MQTT_TOPIC "esp32/state"
#define MQTT_PUBLISH_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static MQTTClient client;
static Network network;
static unsigned char sendbuf[MQTT_SENDBUF_SIZE];
static unsigned char readbuf[MQTT_READBUF_SIZE];

static void *mqtt_thread(void *arg)
{
    (void)arg;
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return NULL;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return NULL;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        MQTTMessage message;
        char payload[20];
        static bool work_state = true;

        snprintf(payload, sizeof(payload), "%s", work_state ? "work" : "not work");
        work_state = !work_state;

        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message: %d\n", rc);
        } else {
            printf("Published message: %s\n", payload);
        }

        xtimer_sleep(MQTT_PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}