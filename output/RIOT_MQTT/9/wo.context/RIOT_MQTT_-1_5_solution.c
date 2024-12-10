#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <stdbool.h>
#include <timer.h>  // Ensure Timer type is defined
#include <network.h>  // Ensure Network type is defined
#include <MQTTClient.h>  // Include MQTTClient header last

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL 5

int main() {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize network and MQTT client
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Setup connection data
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    // Connect to the MQTT broker
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Publish messages periodically
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen("work");

        if (MQTTPublish(&client, MQTT_TOPIC, &message) == SUCCESS) {
            printf("Published message: work\n");
        } else {
            printf("Failed to publish message\n");
        }

        sleep(PUBLISH_INTERVAL);
    }

    return 0;
}