#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/sock/mqttsn.h"  // Assuming this is correct for network functionality
#include "MQTTClient.h"  // Paho MQTT Client header

// Define network parameters and client parameters
#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define MQTT_CLIENT_ID    "esp32_client"
#define PUBLISH_INTERVAL  5  // in seconds

// Define message parameters
const char *topic = "device/state";
const char *message = "work";

void publish_state(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    // Initialize the network
    NetworkInit(&network);
    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != SUCCESS) {
        printf("Failed to connect to network: %d\n", rc);
        return;
    }

    // Initialize the MQTT Client
    unsigned char sendbuf[80], readbuf[80];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    // Connect to the broker
    if ((rc = MQTTConnect(&client, &connectData)) != SUCCESS) {
        printf("Failed to connect to broker: %d\n", rc);
        return;
    }

    // Publish the message
    while (1) {
        MQTTMessage pubmsg;
        pubmsg.payload = (void *)message;
        pubmsg.payloadlen = strlen(message);
        pubmsg.qos = QOS0;
        pubmsg.retained = 0;
        if ((rc = MQTTPublish(&client, topic, &pubmsg)) != SUCCESS) {
            printf("Failed to publish message: %d\n", rc);
        }
        else {
            printf("Message published successfully\n");
        }
        sleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    publish_state();
    return 0;
}
