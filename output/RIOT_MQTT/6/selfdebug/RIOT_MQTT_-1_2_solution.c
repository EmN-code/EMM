#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "mqttsn_paho.h"

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc = 0;

    // Initialize network
    network.mqttread = NULL; // replace with actual function
    network.mqttwrite = NULL; // replace with actual function

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Configure connection data
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_Client";

    // Connect to MQTT Broker
    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return rc;
    }
    printf("Connected to broker\n");

    // Publish loop
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen((char*)message.payload);

        rc = MQTTPublish(&client, "state", &message);
        if (rc != 0) {
            printf("Failed to publish, return code %d\n", rc);
        }
        else {
            printf("Message published\n");
        }

        // Delay 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}