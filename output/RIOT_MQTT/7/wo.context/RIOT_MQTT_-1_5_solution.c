#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "net/sock.h"
#include "MQTTPacket.h"
#include "Network.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_thread(void *arg) {
    (void)arg;
    static const char *pub_topic = "state";
    static const char *client_id = "esp32_client";
    static const char *msg_content[2] = {"work", "not work"};
    int status_index = 0;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = (char *)client_id;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)msg_content[status_index];
        message.payloadlen = strlen(msg_content[status_index]);

        if (MQTTPublish(&client, pub_topic, &message) != SUCCESS) {
            printf("Failed to publish message\n");
        } else {
            printf("Published: %s\n", msg_content[status_index]);
        }

        status_index = (status_index + 1) % 2; // Toggle between "work" and "not work"
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}