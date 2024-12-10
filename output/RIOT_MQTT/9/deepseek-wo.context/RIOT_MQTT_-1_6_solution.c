
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

void mqtt_publish(MQTTClient* client, const char* topic, const char* payload) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(client, topic, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize network
    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    // Initialize MQTT client
    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqttClient, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish(&mqttClient, TOPIC, PAYLOAD);
        printf("Published: %s\n", PAYLOAD);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
