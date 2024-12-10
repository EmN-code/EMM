#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "event/thread.h"
#include "MQTTClient.h"  // Ensure this is the last header file

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "ESP32Client"
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL_SEC (5U)
#define MQTT_BUF_SIZE 128

char mqtt_client_stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_client_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[MQTT_BUF_SIZE], readbuf[MQTT_BUF_SIZE];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc = 0;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 3000, sendbuf, MQTT_BUF_SIZE, readbuf, MQTT_BUF_SIZE);

    char address[100];
    snprintf(address, sizeof(address), "tcp://%s:%d", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    
    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Failed to connect to %s, return code %d\n", address, rc);
        return NULL;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("MQTT connect failed, return code %d\n", rc);
        return NULL;
    }

    printf("MQTT Connected\n");

    char payload[20];

    while (1) {
        snprintf(payload, sizeof(payload), "work");

        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = payload;
        message.payloadlen = strlen(payload);

        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
            printf("Publish failed, return code %d\n", rc);
        }
        else {
            printf("Published work state\n");
        }

        xtimer_sleep(PUBLISH_INTERVAL_SEC);
    }
    return NULL;
}

int main(void) {
    printf("Starting MQTT example\n");
    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_client_thread, NULL, "mqtt_client_thread");

    return 0;
}