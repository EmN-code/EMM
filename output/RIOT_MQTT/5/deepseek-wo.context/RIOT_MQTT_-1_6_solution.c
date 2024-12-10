
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
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
    message.payloadlen = PAYLOAD_LEN;

    MQTTPublish(&mqtt_client, TOPIC, &message);
}

int main(void) {
    printf("Starting MQTT client...\n");

    NetworkInit(&mqtt_network);
    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&mqtt_network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return 1;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_mqtt_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
