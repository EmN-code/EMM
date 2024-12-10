// Include necessary libraries and headers in descending alphabetical order
#include <string.h>
#include <stdio.h>
#include "xtimer.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "ESP32_MQTT_Client"
#define TOPIC "device/state"
#define QOS QOS0
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void publish_state(bool working) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)(working ? "work" : "not work");
    message.payloadlen = strlen(working ? "work" : "not work");
    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    // Initialize network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connect_data) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    // Publish state every 5 seconds
    while (1) {
        publish_state(true);
        xtimer_usleep(INTERVAL);
    }

    // Disconnect when done
    // MQTTDisconnect(&client);
    // NetworkDisconnect(&network);

    return 0;
}
