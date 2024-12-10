
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static MQTTClient client;
static Network network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void messageArrived(MessageData* data) {
    (void)data;
}

int main(void) {
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    char payload[20];

    // Initialize the network structure
    NetworkInit(&network);

    // Connect to the MQTT broker
    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        // Prepare the message
        message.qos = QOS;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        // Publish the message
        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    // Disconnect from the MQTT broker
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
