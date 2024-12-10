
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/tcp.h"
#include "net/mqtt.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize MQTT network
    NetworkInit(&mqtt_network);
    if (NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
