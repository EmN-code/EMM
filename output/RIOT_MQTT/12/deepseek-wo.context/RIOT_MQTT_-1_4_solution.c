
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

static void connectToBroker(void) {
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    printf("Connecting to %s:%d\n", BROKER_IP, BROKER_PORT);
    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return;
    }

    rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }

    printf("Connected to MQTT broker\n");
}

static void publishState(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&mqttClient, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Published: %s to topic %s\n", PAYLOAD, TOPIC);
    }
}

int main(void) {
    printf("Starting MQTT client example\n");

    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);
    connectToBroker();

    while (1) {
        publishState();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
