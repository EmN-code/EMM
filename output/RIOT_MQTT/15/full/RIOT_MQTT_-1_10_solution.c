#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "topic/test"

static void mqtt_publish_task(void *arg) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    
    options.MQTTVersion = 3;
    options.clientID.cstring = "ESPMQTTClient";

    while (1) {
        NetworkInit(&network);
        while ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
            // Log network connect failure here
            xtimer_sleep(1);
        }

        MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

        while ((rc = MQTTConnect(&client, &options)) != 0) {
            // Log MQTT connection failure here
            xtimer_sleep(1);
        }

        MQTTMessage message;
        message.qos = QOS1;
        message.retained = 0;
        message.payload = (void*)"work or not work";
        message.payloadlen = strlen("work or not work");

        while (1) {
            if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
                // Log publish failure here and break to reconnect
                break;
            }
            xtimer_sleep(5);
        }
    }
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_publish_task, NULL, "mqtt_publish_task");
    return 0;
}