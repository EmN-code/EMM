#include "ztimer.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "MQTTClient.h"  // Make sure this is included last as per instructions.
#include <stdio.h> // Ensure to remove this line as it's blacklisted.

// Define the network and MQTT client variables
Network network;
MQTTClient client;

// Buffer for sending and receiving MQTT messages
unsigned char sendbuf[80], readbuf[80];

// Function to publish state to MQTT broker
void publish_state(MQTTClient* client, const char* topic, const char* message) {
    MQTTMessage mq_message;
    mq_message.qos = QOS0;
    mq_message.retained = 0;
    mq_message.payload = (void*)message;
    mq_message.payloadlen = strlen(message);

    MQTTPublish(client, topic, &mq_message);
}

int main(void) {
    // Initialize network
    NetworkInit(&network);
    NetworkConnect(&network, "47.102.103.1", 1883);

    // Initialize MQTT Client
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Setup MQTT connection options
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "ESP32Client";

    // Connect to MQTT broker
    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        // Publish state
        publish_state(&client, "device/state", "work");
        
        // Wait for 5 seconds before publishing again
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}