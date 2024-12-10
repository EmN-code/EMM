#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xtimer.h>
#include <net/netif.h>
#include "MQTTClient.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5000U * US_PER_MS) // Publish every 5 seconds

static char sendbuf[128];
static char readbuf[128];

static MQTTClient client;
static Network network;

void init_network(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
}

void init_mqtt(void) {
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;    
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connection failed\n");
        exit(1);
    }

    printf("MQTT connected\n");
}

void publish_state(void) {
    MQTTMessage message;
    char* state = "work"; // Change this to "not work" as needed
    message.qos = QOS0;
    message.retained = false;
    message.payload = (void*)state;
    message.payloadlen = strlen(state);

    if (MQTTPublish(&client, "state/topic", &message) != SUCCESS) {
        printf("Publish failed\n");
    } else {
        printf("Message published: %s\n", state);
    }
}

int main(void) {
    init_network();
    init_mqtt();
    
    while (true) {
        publish_state();
        xtimer_sleep(5); // sleep for 5 seconds
    }

    return 0;
}