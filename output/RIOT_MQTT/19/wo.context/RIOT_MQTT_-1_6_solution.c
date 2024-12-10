#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "RIOT_ESP32_Client"
#define PUBLISH_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static char sendbuf[128];
static char readbuf[128];

int publish_state(MQTTClient* client, const char* state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)state;
    message.payloadlen = strlen(state);

    return MQTTPublish(client, PUBLISH_TOPIC, &message);
}

int main(void) {
    MQTTClient client;
    Network network;
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    printf("Connecting to MQTT broker at %s:%d...\n", BROKER_IP, BROKER_PORT);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    const char* state = "work";
    while (1) {
        if (publish_state(&client, state) != SUCCESS) {
            printf("Failed to publish message\n");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
        // Toggle state for next message
        state = (strcmp(state, "work") == 0) ? "not work" : "work";
    }

    return 0;
}