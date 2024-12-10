#include <string.h>
#include <stdio.h>
#include "thread.h"
#include "network.h"
#include "MQTTClient.h"

#define MQTT_BROKER "47.102.103.1"
#define MQTT_PORT 1883
#define TOPIC "state"
#define INTERVAL 5

static MQTTClient client;
static Network network;

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen("work");

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    }
}

void *mqtt_thread(void *arg) {
    struct timeval interval;
    interval.tv_sec = INTERVAL;
    interval.tv_usec = 0;

    while (1) {
        publish_state();
        select(0, NULL, NULL, NULL, &interval);
    }

    return NULL;
}

int main(void) {
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, MQTT_BROKER, MQTT_PORT) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN, 0,
                  mqtt_thread, NULL, "mqtt_thread");

    while(1) {
        MQTTYield(&client, 1000);
    }

    return 0;
}