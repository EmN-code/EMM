#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/mqttsn.h"
#include "msg.h"
#include "kernel_types.h"
#include "kernel.h"
#include "MQTTClient.h"  // Ensure this is the last header file if it's used

int main(void) {
    // Set up the network and client configuration
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize the network
    NetworkInit(&network);
    int rc = NetworkConnect(&network, "47.102.103.1", 1883);
    if (rc != SUCCESS) {
        printf("Unable to connect to network.\n");
        return rc;
    }

    // Initialize the MQTT Client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set MQTT connection options
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_Client";

    // Connect to the MQTT broker
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("MQTT connection failed with rc: %d\n", rc);
        return rc;
    }
    printf("MQTT connection success.\n");

    // Publish loop
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*) "work";
        message.payloadlen = strlen("work");

        // Publish the "work or not work" state every 5 seconds
        rc = MQTTPublish(&client, "state", &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message.\n");
        } else {
            printf("Message published.\n");
        }

        msg_t m;
        msg_receive(&m);  // Blocking delay simulation of 5 seconds using message receives
    }

    return 0;
}