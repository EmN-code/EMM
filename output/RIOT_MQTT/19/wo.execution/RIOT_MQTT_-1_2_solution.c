#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <timex.h>
#include <string.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <pthread.h>
#include "MQTTClient.h"

#define QOS               1
#define TIMEOUT           10000L

void publish_state(MQTTClient client) {
    int rc;
    MQTTMessage message;
    char payload[20];
    snprintf(payload, sizeof(payload), "State: work");

    message.qos = QOS;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    if ((rc = MQTTPublish(client, "state", &message)) != SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    }
    else {
        printf("Message published: %s\n", payload);
    }
}

void* mqtt_thread(void *arg) {
    MQTTClient client = (MQTTClient)arg;
    while (1) {
        publish_state(client);
        sleep(5);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    MQTTClient client;
    Network network;
    int rc;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_ESP32_Client";

    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return rc;
    }
    printf("Connected to MQTT broker\n");

    pthread_t thread;
    if (pthread_create(&thread, NULL, mqtt_thread, &client) != 0) {
        printf("Failed to create MQTT thread\n");
        return -1;
    }

    pthread_join(thread, NULL);

    return 0;
}