#include <string.h>  // For memset and string manipulation
#include <stdio.h>   // For printf functionality
#include <stdlib.h>  // For standard library functions
#include <unistd.h>  // For sleep function
#include "MQTTClient.h"  // Must be included last for proper type definitions

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TIMER_INTERVAL_MS 5000

static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void mqtt_publish_state(MQTTClient *client) {
    MQTTMessage message;
    char payload[20];
    
    strcpy(payload, "work or not work");
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);
    
    MQTTPublish(client, "state/topic", &message);
}

int main(void) {
    // Initialize network and MQTT client
    Network network;
    MQTTClient client;
    int rc;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
        printf("Error connecting to network: %d\n", rc);
        return rc;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_Client";

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Error connecting to MQTT broker: %d\n", rc);
        return rc;
    }

    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    // Main loop publishing every 5 seconds
    while (1) {
        mqtt_publish_state(&client);
        sleep(TIMER_INTERVAL_MS / 1000);
    }

    return 0;
}