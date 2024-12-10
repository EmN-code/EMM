#include <xtimer.h>
#include <string.h>
#include "mqttsn_publisher.h"

// Define MQTT broker address and port
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

// Function to initialize MQTT connection
static int mqtt_connect(Network *network, MQTTClient *client) {
    int rc = 0;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_client";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    rc = mqtt_connect(client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    }
    return rc;
}

// MQTT publish function
void publish(MQTTClient *client) {
    char payload[20];
    MQTTMessage message;

    snprintf(payload, sizeof(payload), "state: work");

    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(client, "status", &message);
    if (rc != SUCCESS) {
        printf("Error publishing message: %d\n", rc);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    // Initialize network for MQTT
    NetworkInit(&network);
    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Network connection failed\n");
        return -1;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    rc = mqtt_connect(&network, &client);
    if (rc != SUCCESS) {
        return -1;
    }

    while (1) {
        publish(&client);  // Publish message every 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
