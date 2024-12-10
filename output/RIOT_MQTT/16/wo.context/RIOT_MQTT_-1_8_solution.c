#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "net/sock/udp.h"
#include "kernel_types.h"
#include "thread.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U * US_PER_SEC)
#define MQTT_TOPIC "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

void *mqtt_thread(void *arg) {
    (void) arg;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    // Initialize and connect network
    NetworkInit(&network);
    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_PORT);
    if (rc != 0) {
        printf("Network connection failed: %d\n", rc);
        return NULL;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set connection parameters
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";
    connectData.keepAliveInterval = 20;

    // Connect to MQTT broker
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("MQTT connection failed: %d\n", rc);
        return NULL;
    }
    printf("MQTT connected\n");

    // Publish loop
    MQTTMessage message;
    char payload[20];
    sprintf(payload, "state=%s", "work");
    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;

    while (1) {
        message.payloadlen = strlen(payload);
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != SUCCESS) {
            printf("MQTT publish failed: %d\n", rc);
        } else {
            printf("MQTT message published\n");
        }
        xtimer_sleep(5); // sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}