#include <timex.h>
#include <thread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mqtt.h"
#include "network.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID        "esp32-client"
#define TOPIC            "device/state"
#define MSG_WORK         "work"
#define MSG_NOWORK       "not work"
#define PUB_INTERVAL     (5U * US_PER_SEC) // 5 seconds

static char sendbuf[256], readbuf[256]; // Buffers for MQTT
static MQTTClient client;
static Network network;

void publish_state(void) {
    static int state = 0; // Toggle state
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (state == 0) ? MSG_WORK : MSG_NOWORK;
    message.payloadlen = strlen((char*)message.payload);
    MQTTPublish(&client, TOPIC, &message);
    state = !state; // Toggle state for next publish
}

int main(void) {
    // Initialize network and MQTT clients
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish_state();
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC); // Sleep for interval duration
    }

    // Disconnect (although unreachable in this loop)
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}