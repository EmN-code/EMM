
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&mqtt_client, TOPIC, &message);
}

int main(void) {
    puts("MQTT example");

    // Initialize MQTT network
    NetworkInit(&mqtt_network);
    NetworkConnect(&mqtt_network, SERVER_IP, SERVER_PORT);

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "riot_mqtt_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        puts("MQTT connection failed");
        return 1;
    }

    puts("MQTT connected");

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
