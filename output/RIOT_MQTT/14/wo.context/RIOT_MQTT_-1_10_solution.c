#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define MQTT_CLIENT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // Publish every 5 seconds

static char mqtt_client_thread_stack[MQTT_CLIENT_THREAD_STACKSIZE];

static void *mqtt_client_thread(void *arg) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return NULL;
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return NULL;
    }
    printf("MQTT client connected to broker\n");

    while (1) {
        MQTTMessage message;
        char *payload = "work or not work";

        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, "state", &message);
        if (rc != 0) {
            printf("Failed to publish message: %d\n", rc);
        } else {
            printf("Message published successfully\n");
        }

        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    puts("RIOT MQTT simple publisher\n");
    msg_init_queue(0, 0);
    thread_create(mqtt_client_thread_stack, sizeof(mqtt_client_thread_stack),
                  MQTT_CLIENT_THREAD_PRIORITY, 0, mqtt_client_thread, NULL, "mqtt_client_thread");
    return 0;
}