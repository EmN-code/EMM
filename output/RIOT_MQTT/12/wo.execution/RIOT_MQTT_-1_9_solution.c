#include <stdatomic.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD_1 "work"
#define MQTT_PAYLOAD_2 "not work"
#define MQTT_CLIENT_ID "esp32-client"
#define KEEP_ALIVE_INTERVAL 50
#define MQTT_QOS1 1
#define MQTT_PUBLISH_INTERVAL 5

MQTTClient client;
Network network;

void publish_state() {
    MQTTMessage message;
    message.qos = MQTT_QOS1;
    message.retained = 0;

    while (1) {
        message.payload = (void *)MQTT_PAYLOAD_1;
        message.payloadlen = strlen(MQTT_PAYLOAD_1);
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }

        sleep(MQTT_PUBLISH_INTERVAL);

        message.payload = (void *)MQTT_PAYLOAD_2;
        message.payloadlen = strlen(MQTT_PAYLOAD_2);
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }

        sleep(MQTT_PUBLISH_INTERVAL);
    }
}

int main(void) {
    unsigned char sendbuf[128], readbuf[128];
    int rc;

    // Initialize Network
    NetworkInit(&network);
    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return -1;
    }

    // Initialize MQTT Client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = KEEP_ALIVE_INTERVAL;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return -1;
    }

    // Publish state
    publish_state();

    return 0;
}