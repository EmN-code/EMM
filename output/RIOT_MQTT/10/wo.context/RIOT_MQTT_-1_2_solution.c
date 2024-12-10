#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "MQTTClient.h"  // Must be the last header based on the instruction

#define MQTT_SENDBUF_SIZE 128
#define MQTT_READBUF_SIZE 128

static char sendbuf[MQTT_SENDBUF_SIZE];
static char readbuf[MQTT_READBUF_SIZE];

static MQTTClient client;
static Network network;

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen("work");

    printf("Publishing message...\n");
    MQTTClientPublish(&client, "esp32/state", &message);
}

int main(void) {
    int rc;

    printf("Setting up the MQTT connection...\n");
    
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, MQTT_SENDBUF_SIZE, readbuf, MQTT_READBUF_SIZE);

    char* address = "47.102.103.1";
    int port = 1883;
    rc = NetworkConnect(&network, address, port);
    if (rc != SUCCESS) {
        printf("Failed to connect to the network.");
        return rc;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32";

    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("Failed to connect to the MQTT broker.");
        return rc;
    }

    printf("Successfully connected to MQTT broker.");

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}