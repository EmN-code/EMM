// Importing necessary headers for MQTT and network operations
#include "net/mqttsn.h"
#include "net/network.h"
#include "msg.h"
#include <stdio.h>

#define MQTT_BROKER_ADDRESS_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_riot"
#define PUBLISH_INTERVAL 5

static MQTTClient client;
static Network network;

int main(void) {
    // Buffers for MQTT messages
    unsigned char sendbuf[128], readbuf[128];

    // Initialize Network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    if (NetworkConnect(&network, MQTT_BROKER_ADDRESS_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return -1;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Successfully connected to MQTT broker\n");

    // Publish loop
    while (1) {
        const char *topic = "topic/state";
        const char *message = "work";  // or "not work" depending on the application logic
        MQTTMessage pubMessage;
        pubMessage.qos = QOS0;
        pubMessage.retained = 0;
        pubMessage.payload = (void *)message;
        pubMessage.payloadlen = strlen(message);

        if (MQTTPublish(&client, topic, &pubMessage) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published: %s\n", message);
        }

        // Wait for the next publish interval
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    // Disconnect from broker
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}