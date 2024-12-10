#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "Network.h"
#include "MQTTClient.h"

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL 5 // seconds
#define MQTT_TOP_NAME_ID "device/state"

void publish_state(MQTTClient* client) {
    static int state = 0; // example state that toggles
    MQTTMessage message;
    char payload[30];
    state = !state; // toggle state
    snprintf(payload, sizeof(payload), "State is: %s", state ? "work" : "not work");

    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    if (MQTTPublish(client, MQTT_TOP_NAME_ID, &message) != SUCCESS) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published: %s\n", payload);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    int rc;

    NetworkInit(&network);
    if ((rc = NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT)) != SUCCESS) {
        printf("Failed to connect to network: %d\n", rc);
        return -1;
    }

    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 4;
    connectData.clientID.cstring = "esp32-client";

    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish_state(&client);
        sleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}