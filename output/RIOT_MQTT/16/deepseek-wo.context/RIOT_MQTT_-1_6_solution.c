
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&mqtt_client, TOPIC, &message);
}

int main(void) {
    // Initialize the MQTT network
    NetworkInit(&mqtt_network);

    // Connect to the MQTT broker
    if (NetworkConnect(&mqtt_network, BROKER_IP, BROKER_PORT) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_mqtt_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
