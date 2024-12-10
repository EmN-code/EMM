#include <xtimer.h>  // Timer functions
#include <stdio.h>    // Standard Input/Output
#include <stdlib.h>   // Standard utility functions
#include <string.h>   // String utility functions needed for MQTT
#include <net/sock/udp.h>   // Socket functions and definitions
#include <MQTTClient.h>   // MQTT Client library

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/status"
#define INTERVAL (5U * US_PER_SEC)  // Five seconds interval in microseconds

static void publish_state(MQTTClient *client) {
    MQTTMessage message;
    char *state = "work";
    
    // Set the message content
    message.qos = QOS0;
    message.retained = 0;
    message.payload = state;
    message.payloadlen = strlen(state);

    // Publish the message
    if (MQTTPublish(client, TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published: %s\n", state);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];

    // Initialize the network
    NetworkInit(&network);
    
    // Connect to the broker
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Failed to connect to broker\n");
        return 1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Data for connection
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32-client";

    // Connect to MQTT broker
    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Main loop
    while (1) {
        publish_state(&client);
        xtimer_sleep(INTERVAL / US_PER_SEC);  // Sleep for the set interval
    }

    // Disconnect from the broker
    MQTTDisconnect(&client);
    return 0;
}