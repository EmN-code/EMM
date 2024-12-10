#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mqtt.h"
#include "network.h"
#include "thread.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5  // in seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];

void publish_state(void) {
    MQTTMessage message;
    char* payload = "work or not work";

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(&client, "state/topic", &message);
    printf("State published: %s\n", payload);
}

void mqtt_thread(void* arg) {
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    printf("MQTT Broker connected\n");
    
    while (1) {
        MQTTYield(&client, 1000);
        publish_state();
        sleep(PUBLISH_INTERVAL);
    }

    NetworkDisconnect(&network);
}

int main(void) {
    thread_create(NULL, 0, PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}