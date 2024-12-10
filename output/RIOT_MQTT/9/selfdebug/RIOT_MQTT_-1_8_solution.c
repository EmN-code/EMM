#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "MQTTClient.h"
#include "Network.h"
#include "thread.h"

#define MQTT_SERVER_IP      "47.102.103.1"
#define MQTT_SERVER_PORT    1883
#define MQTT_CLIENT_ID      "ESP32Client"
#define PUB_TOPIC           "state/status"

static char stack[THREAD_STACKSIZE_LARGE];

void *mqtt_handler(void *arg) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_SERVER_IP, MQTT_SERVER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Error connecting to MQTT broker\n");
        return NULL;
    }

    while (1) {
        MQTTMessage message;
        char *content = "Work";
        message.qos = QOS0;
        message.retained = 0;
        message.payload = content;
        message.payloadlen = strlen(content);
        
        if (MQTTPublish(&client, PUB_TOPIC, &message) != SUCCESS) {
            printf("Error publishing message\n");
        }

        thread_sleep(5000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_handler, NULL, "mqtt");
    return 0;
}