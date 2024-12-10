#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqtt.h"  // Ensure this precedes MQTTClient.h to avoid build issues
#include "MQTTClient.h"

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "ESP32_Client"
#define TOPIC "state"
#define QOS QOS0
#define PUBLISH_INTERVAL_MS 5000

static void publish_state(MQTTClient *client) {
    const char *state = "work";
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    if (MQTTPublish(client, TOPIC, &message) != SUCCESS) {
        printf("Failed to publish message\n");
    }
    else {
        printf("Successfully published: %s\n", state);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    MQTTClientInit(&client, &network, MQTT_BROKER_PORT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_BROKER_PORT) != SUCCESS) {
        printf("Network connection failed\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connection failed\n");
        return -1;
    }

    while (1) {
        publish_state(&client);
        sleep(PUBLISH_INTERVAL_MS / 1000);  // Convert ms to seconds
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    return 0;
}