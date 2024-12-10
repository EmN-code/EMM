
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/tcp.h"
#include "net/mqtt.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

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
    printf("Starting MQTT client...\n");

    NetworkInit(&network);
    MQTTClientInit(&mqtt_client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network: %d\n", rc);
        return 1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = CLIENT_ID;
    data.keepAliveInterval = 10;
    data.cleansession = 1;

    rc = MQTTConnect(&mqtt_client, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
