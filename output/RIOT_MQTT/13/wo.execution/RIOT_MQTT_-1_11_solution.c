#include "paho_mqtt.h"  // Ensure this is the last included if it's necessary
#include "net/sock/util.h"
#include "mutex.h"
#include <stdio.h>

// Definitions for MQTT server and message
#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define PUBLISH_INTERVAL_SEC 5

static MQTTClient client;
static Network network;

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)("work");
    message.payloadlen = strlen("work");
    
    if (MQTTPublish(&client, "state/topic", &message) != 0) {
        printf("Failed to publish message\n");
    }
    else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    
    // Connect to the network
    if (NetworkConnect(&network, MQTT_SERVER_IP, MQTT_SERVER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    // Set MQTT options
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "ESP32Client";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }
    
    while (1) {
        publish_state();
        sleep(PUBLISH_INTERVAL_SEC);
    }

    NetworkDisconnect(&network);
    return 0;
}