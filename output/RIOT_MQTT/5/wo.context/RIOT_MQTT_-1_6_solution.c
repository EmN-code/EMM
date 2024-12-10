#include <stdio.h>
#include <string.h>
#include "mqtt.h"
#include "network.h"
#include "xtimer.h"
#include "sock_udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "device/state"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void publish_state(void) {
    MQTTMessage message;
    char *state = "work";

    memset(&message, 0, sizeof(message));
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        printf("Failed to publish state\n");
    } else {
        printf("State published: %s\n", state);
    }
}

int main(void) {
    printf("MQTT client is starting\n");

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    NetworkDisconnect(&network);
    return 0;
}