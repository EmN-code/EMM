#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include "MQTTClient.h"  // Ensure this is the last header file

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT (1883)
#define PUB_TOPIC "sensor/state"

static void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = "work";
    message.payloadlen = strlen("work");

    int rc = MQTTPublish(client, PUB_TOPIC, &message);
    if (rc != 0) {
        printf("Error while publishing: %d\n", rc);
    }
    else {
        printf("State published: work\n");
    }
}

int main(void) {
    MQTTClient client;
    Network network;
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    int rc;

    // Connect to network
    rc = NetworkInit(&network);
    if (rc != SUCCESS) {
        printf("Network initialization failed\n");
        return rc;
    }

    // Connect to MQTT broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;    
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return rc;
    }    

    while (1) {
        publish_state(&client);
        sleep(5);  // Sleep for 5 seconds before publishing again
    }

    // Optionally disconnect
    MQTTDisconnect(&client);
    return 0;
}