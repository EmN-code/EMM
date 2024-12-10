#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "paho_mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "esp32/state"
#define DEVICE_ID "ESP32_Device"

#define PUBLISH_INTERVAL_SEC (5U)  // Publish interval of 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_publisher(void *arg) {
    (void)arg;
    char payload[20];

    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    // Initialize network and client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Error: Network connection failed\n");
        return NULL;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = DEVICE_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Error: MQTTPacket connection failed\n");
        return NULL;
    }
    printf("MQTT Connected\n");

    while (1) {
        snprintf(payload, sizeof(payload), "State: %s", "work");
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);
        
        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Error: MQTT publish failed\n");
        } else {
            printf("Published: %s\n", payload);
        }
        
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
    }

    // Disconnect and cleanup
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return NULL;
}

int main(void) {
    /* create the publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_publisher, NULL, "mqtt_pub");
    
    /* we don't want main to return, so enter an eternal loop */
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}