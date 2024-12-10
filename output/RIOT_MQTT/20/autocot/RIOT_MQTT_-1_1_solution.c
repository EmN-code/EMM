#include <stdio.h>
#include <string.h>
#include <time.h>

#include "mqtt.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID   "esp32_client"
#define TOPIC       "device/state"
#define INTERVAL    5

int main(void) {
    printf("Starting MQTT client on ESP32 with RIOT OS\n");

    /* Initialize network and MQTT client */
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* Connect to the MQTT broker */
    int rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to broker\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect MQTT client\n");
        return -1;
    }

    printf("Connected to MQTT broker at %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        /* Publish the state every five seconds */
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen("work");

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published: work\n");
        }

        sleep(INTERVAL);
    }

    /* Disconnect from the MQTT broker */
    rc = MQTTDisconnect(&client);
    if (rc != 0) {
        printf("Failed to disconnect MQTT client\n");
    }

    NetworkDisconnect(&network);

    return 0;
}