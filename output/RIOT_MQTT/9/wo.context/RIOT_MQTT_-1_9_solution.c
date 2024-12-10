#include <thread.h>
#include <xtimer.h>
#include <stdio.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_mqtt_client"
#define TOPIC "state"
#define QOS 1
#define TIME_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval in microseconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

// Function to publish a message to the specified topic
void publish_state(MQTTClient *client, const char *state) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    MQTTPublish(client, TOPIC, &message);
    printf("Published: %s\n", state);
}

// Main function for MQTT connection and publishing
int main(void) {
    printf("MQTT client is starting...\n");
    NetworkInit(&network);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network.\n");
        return rc;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    rc = MQTTConnect(&client, &connectData);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker.\n");
        return rc;
    }
    printf("Connected to MQTT broker at %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        publish_state(&client, "work");
        xtimer_sleep(5);
    }

    return 0;
}