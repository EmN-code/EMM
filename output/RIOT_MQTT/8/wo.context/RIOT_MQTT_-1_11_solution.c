#include <stdio.h>
#include <string.h>
#include <unistd.h> // For using sleep function
#include <net/sock/udp.h>
#include "MQTTClient.h" // Ensure this is the last header

// Network parameters
#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "ESP32_Client"
#define TOPIC       "state"
#define QOS         1
#define TIMEOUT     10000L

// Function to publish a message
void publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    MQTTPublish(client, TOPIC, &message);
    printf("Message published: %s\n", state);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc;

    // Initialize the MQTT network structure
    NetworkInit(&network);

    // Connect to the network
    rc = NetworkConnect(&network, ADDRESS, PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network, return code: %d\n", rc);
        return -1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // MQTT connection data structure
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENTID;

    // Connect to MQTT Broker
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker, return code: %d\n", rc);
        return -1;
    }

    printf("Successfully connected to MQTT broker\n");

    // Publish state message every 5 seconds
    while (1) {
        publish_state(&client, "work");
        sleep(5);
    }

    return 0;
}