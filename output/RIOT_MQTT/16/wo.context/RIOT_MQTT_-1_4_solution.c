#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "MQTTClient.h"

#define MQTT_BROKER_ADDRESS  "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define CLIENT_ID            "esp32_client"
#define PUBLISH_TOPIC        "device/state"
#define PUBLISH_INTERVAL_MS  (5000U)

static char stack[THREAD_STACKSIZE_MAIN];

static void* mqtt_client_thread(void* arg) {
    (void)arg;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT) != 0) {
        printf("Error connecting to MQTT broker\n");
        return NULL;
    }

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connect_data) != 0) {
        printf("Error connecting to MQTT broker\n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen("work");

        if (MQTTPublish(&client, PUBLISH_TOPIC, &message) != 0) {
            printf("Error publishing message\n");
        } else {
            printf("Published message to topic: %s\n", PUBLISH_TOPIC);
        }

        xtimer_sleep(PUBLISH_INTERVAL_MS / 1000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_client_thread, NULL, "mqtt");
    return 0;
}