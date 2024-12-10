#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include "MQTTClient.h" // Ensure this is the last header

#define MQ_BROKER_IP "47.102.103.1"
#define MQ_BROKER_PORT 1883
#define MQ_TOPIC "device/state"
#define TIMER_INTERVAL (5U * US_PER_SEC) // 5 seconds

static void publish_state(void) {
    MQTTMessage message;
    char payload[20];
    int state = rand() % 2; // Random state: 0 or 1

    sprintf(payload, "State: %s", (state ? "Work" : "Not Work"));
    message.payload = payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS0;
    message.retained = 0;

    int rc = MQTTPublish(&mqtt_client, MQ_TOPIC, &message);
    if (rc != SUCCESS) {
        printf("[ERROR] Failed to publish message. Code: %d\n", rc);
    }
    else {
        printf("Message published: %s\n", payload);
    }
}

int main(void) {
    Network network;
    MQTTClient mqtt_client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&mqtt_client, &network, 30000, sendbuf, 100, readbuf, 100);

    if (NetworkConnect(&network, MQ_BROKER_IP, MQ_BROKER_PORT) != SUCCESS) {
        printf("[ERROR] Failed to connect to network.\n");
        return 1;
    }

    connectData.clientID.cstring = "ESP32_RIOT_Client";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &connectData) != SUCCESS) {
        printf("[ERROR] Failed to connect to MQTT broker.\n");
        return 1;
    }

    printf("Connected to MQTT Broker.\n");

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}