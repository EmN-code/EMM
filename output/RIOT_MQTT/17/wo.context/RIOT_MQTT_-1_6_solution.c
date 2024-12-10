#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "net/sock/tcp.h"
#include "paho-mqtt/MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32-client"
#define TOPIC "esp32/status"
#define QOS 1
#define TIMEOUT 10000L

// Network initialization
static Network network;
static MQTTClient client;
static unsigned char sendbuf[100], readbuf[100];

// Function to publish a message
void publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    
    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message: %d\n", rc);
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    rc = MQTTConnect(&client, &connectData);

    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        NetworkDisconnect(&network);
        return -1;
    }

    while (1) {
        publish_state("work");
        sleep(5);
    }

    // It will never reach this point due to the infinite loop
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    return 0;
}