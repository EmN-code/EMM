#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv6.h>
#include <net/emcute.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_TOPIC "device/state"
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

static void mqtt_publish(Network *network, MQTTClient *client) {
    MQTTMessage message;
    char payload[20];    
    snprintf(payload, sizeof(payload), "work");
    
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);
    
    int rc = MQTTPublish(client, PUBLISH_TOPIC, &message);
    if (rc != SUCCESS) {
        printf("ERROR: Unable to publish message\n");
    } else {
        printf("Published: %s\n", payload);
    }
}

int main(void) {
    Network network;
    MQTTClient client;

    // Initialize Network and MQTT Client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Connect to the MQTT broker
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) == FAILURE) {
        puts("ERROR: Unable to connect to the broker\n");
        return -1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_device";

    if (MQTTConnect(&client, &data) != SUCCESS) {
        puts("ERROR: Unable to connect to MQTT\n");
        return -1;
    }

    while (1) {
        mqtt_publish(&network, &client);
        xtimer_sleep(5);
    }

    NetworkDisconnect(&network);
    return 0;
}