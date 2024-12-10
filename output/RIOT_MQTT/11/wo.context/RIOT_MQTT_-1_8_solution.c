#include <ztimer.h>  // Ensure proper timing functions
#include <stdio.h>    // For input/output operations
#include "MQTTClient.h"  // Include MQTTClient.h last

#define MQTT_SERVER_HOST "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds
#define MQTT_PUB_TOPIC "device/state"
#define MQTT_CLIENT_ID "mqtt_riot_esp32"

static unsigned char sendbuf[100], readbuf[100];

int main(void) {
    Network network;
    MQTTClient client;
    int rc;

    printf("Connecting to MQTT Broker at %s:%d...\n", MQTT_SERVER_HOST, MQTT_SERVER_PORT);
    NetworkInit(&network);  // Initialize the Network object
    rc = NetworkConnect(&network, MQTT_SERVER_HOST, MQTT_SERVER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to the network: %d\n", rc);
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return -1;
    }
    printf("Connected to MQTT broker\n");

    char* payload = "work";  // State to publish
    while(1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void*)payload;
        message.payloadlen = strlen(payload);

        rc = MQTTPublish(&client, MQTT_PUB_TOPIC, &message);
        if (rc != SUCCESS) {
            printf("Failed to publish message: %d\n", rc);
        }
        else {
            printf("Message published: %s\n", (char*)message.payload);
        }

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);  // Wait for 5 seconds
    }

    NetworkDisconnect(&network);
    return 0;
}