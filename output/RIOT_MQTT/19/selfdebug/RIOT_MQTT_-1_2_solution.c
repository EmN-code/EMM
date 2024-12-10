#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTClient.h>
#include <net/gnrc.h>

// MQTT connection parameters
#define ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID    "ESP32Client"
#define TOPIC       "esp32/state"
#define QOS         1
#define TIMEOUT     10000L

void publish_state(MQTTClient client) {
    // Create a message
    char* payload = (rand() % 2 == 0) ? "work" : "not work";
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    // Publish the message
    int rc = MQTTPublish(client, TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    }
}

int main(void) {
    printf("Starting MQTT Example\n");

    MQTTClient client;
    Network network;
    char clientID[] = CLIENTID;
    unsigned char sendbuf[80], readbuf[80];
    int rc = 0;

    // Initialize network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set connection parameters
    MQTTPacket_connectData connData = MQTTPacket_connectData_initializer;
    connData.MQTTVersion = 3;
    connData.clientID.cstring = clientID;

    // Connect to the MQTT broker
    if ((rc = NetworkConnect(&network, "47.102.103.1", 1883)) != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return rc;
    }

    if ((rc = MQTTConnect(&client, &connData)) != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return rc;
    }
    printf("Connected to MQTT broker\n");

    // Main loop for publishing state every 5 seconds
    while (1) {
        publish_state(client);
        sleep(5);
    }

    // Disconnect when finished
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}