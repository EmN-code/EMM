#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <mqtt.h>
#include "mqtt/MQTTClient.h"  // Ensure this is the last to be included

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define SUBSCRIBE_TOPIC "esp32/state"
#define PUB_INTERVAL (5U * US_PER_SEC)  // Publish every 5 seconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

static void publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    if (MQTTPublish(&client, SUBSCRIBE_TOPIC, &message) != SUCCESS) {
        printf("Publish failed\n");
    }
    else {
        printf("Publish success: %s\n", state);
    }
}

int main(void) {
    printf("Starting MQTT example\n");

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    printf("Connecting to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != SUCCESS) {
        printf("Network connection failed\n");
        return 1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connection failed\n");
        return 1;
    }

    printf("MQTT Connected\n");

    while (1) {
        publish_state("working");  // Publish the state
        xtimer_sleep(PUB_INTERVAL);
    }
    
    return 0;
}