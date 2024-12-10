#include <timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>
#include <net/emcute.h>
#include <net.h>
#include <thread.h>
#include <xtimer.h>
#include <mqtt_client.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/state"
#define CLIENT_ID "esp32-client"
#define WORK_STATE "work"
#define NOT_WORK_STATE "not_work"

static char stack[THREAD_STACKSIZE_MAIN];

static void on_message_received(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Message received: %.*s\n", len, (char *)data);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    int rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = WORK_STATE;
    message.payloadlen = strlen(WORK_STATE);

    while (1) {
        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published: %s\n", WORK_STATE);
        }
        xtimer_sleep(5);
    }

    return 0;
}