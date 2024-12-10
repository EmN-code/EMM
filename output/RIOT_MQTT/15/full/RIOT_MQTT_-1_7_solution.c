#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5 * US_PER_SEC)

static char sendbuf[100];
static char readbuf[100];

void mqtt_task(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying in 1 second...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Network connected\n");

        connectData.MQTTVersion = 3;
        connectData.clientID.cstring = "riot_client";
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying...\n");
            continue;
        }
        printf("MQTT connected\n");

        while (1) {
            MQTTMessage message;
            message.qos = QOS0;
            message.retained = 0;
            message.payload = (void *)"work";
            message.payloadlen = strlen("work");

            rc = MQTTPublish(&client, "state", &message);
            if (rc != 0) {
                printf("Publish failed. Reconnecting...\n");
                break;
            }
            printf("Published state: work\n");

            xtimer_sleep(MQTT_PUBLISH_INTERVAL);
        }
        MQTTDisconnect(&client);
    }
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, mqtt_task, NULL, "mqtt_task");
    return 0;
}