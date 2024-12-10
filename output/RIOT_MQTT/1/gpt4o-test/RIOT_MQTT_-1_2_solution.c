#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <stdlib.h>
#include <paho_mqtt.h>
#include <unistd.h>
#include <stdio.h>
#include "MQTTClient.h"

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "RIOT_Client"
#define TOPIC       "work_status"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     10000L

void messageHandler(MessageData* data) {
    // This function must be defined for receiving messages, but is not used for publishing only.
}

int main() {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    int rc;

    // Initialize network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Establish a network connection
    if ((rc = NetworkConnect(&network, ADDRESS, PORT)) != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENTID;

    // Connect to the MQTT broker
    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("MQTT connect failed, return code %d\n", rc);
        return -1;
    }

    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        // Publish the message every five seconds
        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("MQTT publish failed, return code %d\n", rc);
        }
        sleep(5); // Delay for 5 seconds
    }

    // Disconnect is implicit since the code will run indefinitely
    // NetworkDisconnect(&network);

    return 0;
}