#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>  // for sleep function
#include <string.h>
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "MQTTPacket.h"
#include "MQTTClient.h" // Ensure this is the last header if it’s used.

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "ESP32Client"
#define MQTT_TOPIC "device/state"
#define MQTT_MESSAGE "work"

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    // Initialize network connection
    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publish state every 5 seconds
    while (true) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = false;
        message.payload = (void*)MQTT_MESSAGE;
        message.payloadlen = strlen(MQTT_MESSAGE);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", MQTT_MESSAGE);
        }

        sleep(5); // Wait for 5 seconds
    }

    // Disconnect from MQTT broker
    NetworkDisconnect(&network);

    return 0;
}