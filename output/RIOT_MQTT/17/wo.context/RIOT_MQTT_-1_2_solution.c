#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/sock.h>
#include <net/mqttsn.h>
#include "paho.mqtt.embedded-c/MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define BUFFER_SIZE 100
#define PUBLISH_INTERVAL 5 

static Network network;
static MQTTClient client;
static unsigned char sendbuf[BUFFER_SIZE], readbuf[BUFFER_SIZE];

void publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, MQTT_TOPIC, &message);
    printf("Published state: %s\n", state);
}

int main(void) {
    printf("Initializing MQTT client...\n");

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, BUFFER_SIZE, readbuf, BUFFER_SIZE);

    printf("Connecting to MQTT broker %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.keepAliveInterval = 10;
    connectData.cleansession = 1;
    connectData.clientID.cstring = "esp32client";

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("MQTT connect failed\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        publish_state("work");
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL * US_PER_SEC);
    }

    return 0;
}