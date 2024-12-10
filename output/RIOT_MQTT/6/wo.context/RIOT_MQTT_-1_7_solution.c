#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define BUFFER_SIZE 128
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

static const unsigned char topic[] = "device/state";

static sock_udp_ep_t broker_ep = {
    .family = AF_INET,
    .netif  = SOCK_ADDR_ANY_NETIF,
    .port   = BROKER_PORT
};

int main(void) {
    // Initialize UDP endpoint 
    inet_pton(AF_INET, BROKER_IP, &broker_ep.addr);

    // Buffer for MQTT message
    unsigned char sendbuf[BUFFER_SIZE];
    unsigned char readbuf[BUFFER_SIZE];
    char payload[BUFFER_SIZE];

    // Initialize MQTT client
    MQTTClient client;
    Network network;
    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);

    snprintf(payload, sizeof(payload), "work or not work");
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT-ESP32";

    // Connect to the MQTT broker
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    if (MQTTConnect(&client, &data) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        // Publish the state every 5 seconds
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        if (MQTTPublish(&client, (const char*)topic, &message) != SUCCESS) {
            printf("Failed to publish message\n");
        }
        else {
            printf("Message published: %s\n", payload);
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}