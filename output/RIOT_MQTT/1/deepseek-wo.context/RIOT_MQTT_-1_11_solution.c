
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient client;

void messageArrived(MessageData* data) {
    (void)data;
}

int main(void) {
    // Initialize the network
    NetworkInit(&network);

    // Connect to the MQTT broker
    if (NetworkConnect(&network, SERVER_IP, SERVER_PORT) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Set the message handler
    MQTTSetMessageHandler(&client, TOPIC, messageArrived);

    // Connect to the MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = 0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = PAYLOAD_LEN;

        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Failed to publish message");
        }

        xtimer_usleep(INTERVAL);
    }

    return 0;
}
