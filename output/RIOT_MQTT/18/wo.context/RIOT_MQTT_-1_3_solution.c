#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "mqtt_client.h" // Correctly referenced as the last header file

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_STATE_WORK "work"
#define MQTT_STATE_NOT_WORK "not work"
#define MQTT_PUBLISH_INTERVAL 5

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100];
    unsigned char readbuf[100];
    
    // Initialize network and client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Error: Could not connect to the MQTT broker.\n");
        exit(EXIT_FAILURE);
    }
    
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";
    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Error: Could not connect to MQTT.\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Publish "work or not work" state to the broker
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)MQTT_STATE_WORK;
        message.payloadlen = strlen(MQTT_STATE_WORK);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Error: Could not publish message.\n");
        }

        sleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}