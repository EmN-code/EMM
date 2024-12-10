
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "net/sock.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "MQTTClient.h"  // Ensure this is the last header file to be included

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL 5  // in seconds

int main(void)
{
    // Initialize the MQTT client
    MQTTClient client;
    Network network;
    unsigned char sendbuf[200], readbuf[100];
    int rc = 0;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    IPAddress broker_addr;
    inet_pton(AF_INET, BROKER_IP, &broker_addr);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_Client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    printf("Connecting to %s:%d\n", BROKER_IP, BROKER_PORT);
    rc = NetworkConnect(&network, broker_addr, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to broker\n");
        return -1;
    }

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = 0;
        message.retained = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        printf("Publishing message: %s\n", PAYLOAD);
        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message\n");
        }

        sleep(INTERVAL);
    }

    // Disconnect from the MQTT broker
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
