#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "paho_mqtt.h"

#define TOPIC "device/state"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define MSG_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static MQTTClient client;
static Network network;

void publish_state(void) {
    MQTTMessage message;
    char payload[20];
    static bool state = false;
    state = !state;  // Toggle state
    snprintf(payload, sizeof(payload), "{\"state\":\"%s\"}", state ? "work" : "not work");
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);
    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    unsigned char sendbuf[128], readbuf[128];
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    // Initialize network and client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to broker
    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network\n");
        return EXIT_FAILURE;
    }

    data.MQTTVersion = 3;
    data.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return EXIT_FAILURE;
    }

    printf("Connected to MQTT broker successfully\n");

    while (1) {
        publish_state();
        xtimer_sleep(5);  // Delay 5 seconds
    }

    return EXIT_SUCCESS;
}