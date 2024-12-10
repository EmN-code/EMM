#include <stddef.h> // For NULL, size_t and others
#include <stdio.h> // Standard I/O operations
#include <string.h> // For strlen and string operations
#include <xtimer.h> // For timer functions and definitions
#include "MQTTClient.h" // MQTT client functionalities

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/status"
#define MQTT_CLIENT_ID "esp32_client"
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // Publish every 5 seconds

MQTTClient client;  // MQTT Client structure
Network network;  // Network structure
unsigned char sendbuf[128], readbuf[128];  // Buffers for MQTT

void publish_state(void) {
    MQTTMessage message;
    char* payload = "work";
    message.qos = QOS0;  // Quality of Service Level 0
    message.retained = 0;  // Do not retain the message
    message.payload = (void*) payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(&client, MQTT_TOPIC, &message);
    printf("Published: %s\n", payload);
}

int main(void) {
    NetworkInit(&network);  // Initialize network settings
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);  // Connect to MQTT Broker

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT Connection failed\n");
        return -1;
    }
    printf("MQTT Connected\n");

    while (1) {
        publish_state();  // Publish state
        xtimer_sleep(PUBLISH_INTERVAL);  // Wait before next publish
    }

    return 0;
}