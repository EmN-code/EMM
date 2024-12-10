
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <net/ipv4/tcp.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

void network_init(void) {
    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
}

void mqtt_init(void) {
    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    MQTTPublish(&mqttClient, TOPIC, &message);
}

int main(void) {
    network_init();
    mqtt_init();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
