#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/nanocoap.h"
#include "kernel_types.h"
#include "thread.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL 5

static char stack[THREAD_STACKSIZE_MAIN];

void mqtt_publish_thread(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*) ((rand() % 2 == 0) ? "work" : "not work");
        message.payloadlen = strlen((char*)message.payload);
        
        if (MQTTPublish(&client, "esp32/status", &message) != SUCCESS) {
            printf("Failed to publish message\n");
        }

        xtimer_sleep(MQTT_PUBLISH_INTERVAL);
    }
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
}

int main(void) {
    puts("ESP32 MQTT Publisher Example");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_publish_thread, NULL, "mqtt_pub");
    return 0;
}