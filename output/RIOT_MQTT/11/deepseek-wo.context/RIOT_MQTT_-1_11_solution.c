
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    MQTTPublish(&mqttClient, TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    // Connect to MQTT broker
    int rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Set up MQTT client
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_mqtt_client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    rc = MQTTConnect(&mqttClient, &data);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect and clean up
    NetworkDisconnect(&network);
    return 0;
}
