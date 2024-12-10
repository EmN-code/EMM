#include <stdio.h>
#include <string.h>
#include "paho_mqtt.h"
#include "thread.h"
#include "xtimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static MQTTClient client;
static Network network;

void *mqtt_thread(void *arg) {
    (void)arg;
    int rc;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return NULL;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)MQTT_MESSAGE_WORK;
        message.payloadlen = strlen(MQTT_MESSAGE_WORK);

        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        } else {
            printf("Message published: %s\n", (char *)message.payload);
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}