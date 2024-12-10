
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static Network network;
static MQTTClient client;

void messageArrived(MessageData* md) {
    MQTTMessage* message = md->message;
    printf("Message arrived: %.*s\n", (int)message->payloadlen, (char*)message->payload);
}

int main(void) {
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    // Initialize the network
    NetworkInit(&network);
    rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return -1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return -1;
    }

    printf("Connected to MQTT broker\n");

    // Set the message handler
    MQTTSetMessageHandler(&client, TOPIC, messageArrived);

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.payload = PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message: %d\n", rc);
        } else {
            printf("Published: %s\n", PAYLOAD);
        }

        xtimer_sleep(5);
    }

    return 0;
}
