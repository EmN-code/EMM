#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define QOS 1
#define TIMEOUT 10000L

static MQTTClient client;
static Network network;// Connect to MQTT broker
static int connect_to_broker(Network *network) {
    return NetworkConnect(network, BROKER_IP, BROKER_PORT);
}// Publish message to MQTT broker
static void publish_state(MQTTClient* client) {
    MQTTMessage message;
    char* payload = "work or not work";
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS;
    message.retained = 0;
    MQTTPublish(client, "state_topic", &message);
}static void* mqtt_thread(void *arg) {
    (void)arg;
    // Initialize MQTT Client
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to broker
    while (connect_to_broker(&network) != 0) {
        printf("Failed to connect to broker. Retrying...\n");
        thread_sleep(1);
    }

    // Continuously publish state every 5 seconds
    while (1) {
        publish_state(&client);
        printf("Published state.\n");
        thread_sleep(5);
    }    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN + 1,
                  THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_thread");    return 0;
}