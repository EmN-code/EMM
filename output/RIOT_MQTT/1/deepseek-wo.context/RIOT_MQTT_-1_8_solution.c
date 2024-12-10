
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/tcp.h"
#include "net/mqtt_client.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqttClient;
static Network network;

void connect_to_broker(void) {
    int rc;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_ESP32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to broker: %d\n", rc);
        return;
    }

    rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return;
    }

    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int rc = MQTTPublish(&mqttClient, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message: %d\n", rc);
    } else {
        printf("Published: %s to topic: %s\n", PAYLOAD, TOPIC);
    }
}

int main(void) {
    printf("Starting MQTT client on ESP32\n");

    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
