#include <xtimer.h>
#include <stdio.h>
#include "MQTTClient.h"  // Must be last in the import list

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "RIOT_ESP32_Client"
#define PUBLISH_INTERVAL (5U)  // publish every 5 seconds

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    // Initialize network and client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Prepare connection data
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    // Establish network connection
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    // Connect to MQTT broker
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed: %d\n", rc);
            xtimer_sleep(1);
            continue;
        } else {
            printf("MQTT connected successfully.\n");
            break;
        }
    }

    // Loop to publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (char *)"work";
        message.payloadlen = strlen("work");

        rc = MQTTPublish(&client, "state", &message);
        if (rc != 0) {
            printf("Failed to publish message: %d\n", rc);
        } else {
            printf("Message published successfully.\n");
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    // Disconnect from MQTT (this block is never reached in an infinite loop)
    NetworkDisconnect(&network);
    printf("Disconnected from network.\n");

    return 0;
}