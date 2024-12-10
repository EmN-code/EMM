#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/sock/tcp.h"
#include "net/mqttsn.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

static const char *client_id = "esp32-mqtt-client";
static const char *topic_name = "device/state";

static void publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    MQTTPublish(client, topic_name, &message);
    printf("Published: %s\n", state);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    // Initialize network
    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != SUCCESS) {
        printf("Network connection failed\n");
        return -1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = (char *)client_id;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connection failed\n");
        return -1;
    }
    printf("Connected to MQTT broker\n");

    // Main loop to publish data
    while (1) {
        publish_state(&client, "work");
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
        publish_state(&client, "not work");
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}